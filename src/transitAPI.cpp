#include "transitAPI.h"

String callTransitAPI(String api_url) {
	HTTPClient http;
	
	http.begin(api_url.c_str());
    
	// Send HTTP GET request
	int httpResponseCode = http.GET();

	if (httpResponseCode>0) {
#if DEBUG
		Serial.print("HTTP Response code: ");
		Serial.println(httpResponseCode);
#endif
		String payload = http.getString();
#if DEBUG
		Serial.println(payload);
#endif

		http.end();
		return payload;
	}
	else {
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);

		http.end();
		return "API Request Error";
	}
	
}

TransitData updateBARTData(String station) {
  String BART_ulr = "https://api.bart.gov/api/etd.aspx?cmd=etd&orig=" + station + "&key=" + BART_API_KEY + "&json=y";
  String BART_response = callTransitAPI(BART_ulr);
  if (BART_response != "API Request Error") {
    JSONVar BART_json = JSON.parse(BART_response);
    JSONVar destinations = BART_json["root"]["station"][0]["etd"];
    for (int i = 0; i < destinations.length(); i++) {
      String destination = destinations[i]["abbreviation"];
      String minutes = destinations[i]["estimate"][0]["minutes"];
      String delay = destinations[i]["estimate"][0]["delay"];

#if DEBUG
      Serial.println("Destination: " + destination + ", Minutes: " + minutes + ", Delay: " + delay);
#endif

      if (destination == "BERY") {
        TransitData data = {destinations[i]["estimate"][0]["color"], minutes, delay};
        return data;
      }

    }
  }

  TransitData emptyData = {"", "", ""};
  return emptyData;
}

TransitData updateBusData(String station, std::set<String> routes) {
  String AC_transit_url = "https://api.actransit.org/transit/stops/" + station +"/predictions/?token=" + AC_TRANSIT_API_KEY;
  String AC_transit_response = callTransitAPI(AC_transit_url);
  if (AC_transit_response != "API Request Error") {
    JSONVar AC_Transit_json = JSON.parse(AC_transit_response);
    for (int i = 0; i < AC_Transit_json.length(); i++) {
      String route = AC_Transit_json[i]["RouteName"];
      String departure = AC_Transit_json[i]["PredictedDeparture"];
      String delay = JSON.stringify(AC_Transit_json[i]["PredictedDelayInSeconds"]);

#if DEBUG
      Serial.println("Route: " + route + ", Departure Time: " + departure + ", Delay: " + delay);
#endif
      if (routes.find(route) != routes.end()) {
        String minutes = departureToMinutes(departure);
#if DEBUG
        Serial.println("Minutes: " + minutes);
#endif
        TransitData data = {route, minutes, delay};
        return data;
      }
    }
  }

  TransitData emptyData = {"", "", ""};
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
