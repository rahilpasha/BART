#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <ctime>
#include "credentials.h"
#include "transitAPI.h"

TFT_eSPI tft = TFT_eSPI();

struct transitData {
  String route;
  String minutes;
  String delay;
};

void connectWiFi(void);
transitData updateBARTData(String station);
transitData updateBusData(String station);
String departureToMinutes(String departure);
void setupTime();

void setup() {
  Serial.begin(115200);
  connectWiFi();
  setupTime();

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);


}

void loop() {
  Serial.println(WiFi.status());

  transitData bartData = updateBARTData("dbrk");
  transitData busData = updateBusData("59555");
  

  delay(10000);
}

void connectWiFi(void) {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}

transitData updateBARTData(String station) {
  String BART_ulr = "https://api.bart.gov/api/etd.aspx?cmd=etd&orig=" + station + "&key=" + BART_API_KEY + "&json=y";
  String BART_response = callAPI(BART_ulr);
  if (BART_response != "API Request Error") {
    JSONVar BART_json = JSON.parse(BART_response);
    JSONVar destinations = BART_json["root"]["station"][0]["etd"];
    for (int i = 0; i < destinations.length(); i++) {
      String destination = destinations[i]["abbreviation"];
      String minutes = destinations[i]["estimate"][0]["minutes"];
      String delay = destinations[i]["estimate"][0]["delay"];
      Serial.println("Destination: " + destination + ", Minutes: " + minutes + ", Delay: " + delay);

      if (destination == "BERY") {
        transitData data = {destinations[i]["estimate"][0]["color"], minutes, delay};
        return data;
      }

    }
  }

  transitData emptyData = {"", "", ""};
  return emptyData;
}

transitData updateBusData(String station) {
  String AC_transit_url = "https://api.actransit.org/transit/stops/" + station +"/predictions/?token=" + AC_TRANSIT_API_KEY;
  String AC_transit_response = callAPI(AC_transit_url);
  if (AC_transit_response != "API Request Error") {
    JSONVar AC_Transit_json = JSON.parse(AC_transit_response);
    for (int i = 0; i < AC_Transit_json.length(); i++) {
      String route = AC_Transit_json[i]["RouteName"];
      String departure = AC_Transit_json[i]["PredictedDeparture"];
      String delay = JSON.stringify(AC_Transit_json[i]["PredictedDelayInSeconds"]);
      Serial.println("Route: " + route + ", Departure Time: " + departure + ", Delay: " + delay);

      if (route == "51B" || route == "79") {
        String minutes = departureToMinutes(departure);
        Serial.println("Minutes: " + minutes);
        transitData data = {route, minutes, delay};
        return data;
      }
    }
  }

  transitData emptyData = {"", "", ""};
  return emptyData;
}

String departureToMinutes(String departure) {
  String minutes = "";
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo)) {
      Serial.println("Failed to obtain current time");
      return "";
  }

  // Convert current time to time_t
  time_t currentTime = mktime(&timeInfo);

  // Parse target time (2025-03-29T16:46:00) into struct tm
  struct tm targetTm = {};
  sscanf(departure.c_str(), "%d-%d-%dT%d:%d:%d", 
          &targetTm.tm_year, &targetTm.tm_mon, &targetTm.tm_mday,
          &targetTm.tm_hour, &targetTm.tm_min, &targetTm.tm_sec);

  targetTm.tm_year -= 1900;  // tm_year is years since 1900
  targetTm.tm_mon -= 1;      // tm_mon is 0-based (Jan = 0)
  
  // Convert target time to time_t
  time_t targetTime = mktime(&targetTm);

  // Compute time difference
  double diffSeconds = difftime(targetTime, currentTime);
  int diffMinutes = floor(diffSeconds / 60);

  return String(diffMinutes);
}

void setupTime() {
  // Set time using NTP (Pacific Time Zone with DST)
  configTime(-7 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time to sync
  struct tm timeInfo;
  while (!getLocalTime(&timeInfo)) {
      Serial.println("Waiting for time...");
      delay(1000);
  }
}