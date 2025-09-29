#include <TFT_eSPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ctime>

#include "credentials.h"
#include "transitAPI.h"
#include "todoistAPI.h"
#include "display.h"

#include <XPT2046_Bitbang.h>

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN  25
#define CS_PIN   33

XPT2046_Bitbang touchscreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);
int screenButtonEvents = 0;
ScreenType screen = Transit;

void connectWiFi(void);
void setupTime(void);
void switchScreen(ScreenType screen);
TransitData updateBARTData(String station);
TransitData updateBusData(String station, std::set<String> routes);
String departureToMinutes(String departure);

void setup() {
  Serial.begin(115200);
  connectWiFi();
  setupTime();

  touchscreen.begin();

  displayInit();
  switchScreen(screen);
}

void loop() {

  TouchPoint touch = touchscreen.getTouch();

  // Display touches that have a pressure value (Z)
  if (touch.zRaw > 200) { // 200 filters out noise

#if DEBUG
    Serial.print("Touch at X: ");
    Serial.print(touch.x);
    Serial.print(", Y: ");
    Serial.println(touch.y);
    Serial.print(", Z: ");
    Serial.println(touch.zRaw);
#endif
    if (touch.x > SCREEN_BTN_X_MIN && touch.x < SCREEN_BTN_X_MAX && touch.y > SCREEN_BTN_Y_MIN && touch.y < SCREEN_BTN_Y_MAX) {
      screenButtonEvents++;
      if (screenButtonEvents == 5) {
        screen = screen == Transit ? Todoist : Transit;
        screenButtonEvents = 0;

        switchScreen(screen);

        delay(1000);
      }
    } else if(touch.x > 270 && touch.y < 40) {
      switchScreen(screen);
      screenButtonEvents = 0;
    } else {
      screenButtonEvents = 0;
    }

  }
  delay(10);
}

void connectWiFi(void) {
  #if WIFI_TYPE == WIFI_EDUROAM
    WiFi.begin(ssid, WPA2_AUTH_PEAP, username, username, password);
  #else
    WiFi.begin(ssid, password);
  #endif

  while(WiFi.status() != WL_CONNECTED) {
#if DEBUG
    delay(500);
    Serial.print(".");
#endif
  }
#if DEBUG
  Serial.println("Connected");
#endif
}

void setupTime() {
  // Set time using NTP (Pacific Time Zone with DST)
  configTime(-7 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Wait for time to sync
  struct tm timeInfo;
  while (!getLocalTime(&timeInfo)) {
#if DEBUG
      Serial.println("Waiting for time...");
      delay(500);
#endif
  }
}

void switchScreen(ScreenType screen) {
  displayHeader(screen);

  if (screen == Transit) {

    displayTransitSetup();

    TransitData bartData = updateBARTData("dbrk");
    TransitData busData = updateBusData("55452", {"18", "6", "F"});
    
    displayBARTData(bartData);
    displayBusData(busData);

  } else if (screen == Todoist) {

    String todoistResponse = callTodoistAPI("/tasks?filter=today");

    std::vector<String> tasks = processJsonResponse(todoistResponse);

#if DEBUG
    for (String task : tasks) {
      Serial.println(task);
    }
#endif
    displayTasks(tasks);

  }

}
