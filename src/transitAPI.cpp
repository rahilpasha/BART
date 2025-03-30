#include "transitAPI.h"

String callAPI(String api_url) {
	HTTPClient http;
	
	http.begin(api_url.c_str());
    
	// Send HTTP GET request
	int httpResponseCode = http.GET();

	if (httpResponseCode>0) {
		Serial.print("HTTP Response code: ");
		Serial.println(httpResponseCode);

		String payload = http.getString();
		Serial.println(payload);

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

