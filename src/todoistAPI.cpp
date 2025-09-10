#include "todoistAPI.h"

String callTodoistAPI(String endpoint) {
  HTTPClient http;
  http.begin(String(todoistAPIBase) + endpoint);
  
  // Set headers
  http.addHeader("Authorization", "Bearer " + String(todoistToken));
  http.addHeader("Content-Type", "application/json");
  
  int httpResponseCode = http.GET();
  String response = "";
  
  if (httpResponseCode > 0) {
    response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
  } else {
    Serial.println("Error on HTTP request: " + String(httpResponseCode));
  }
  
  http.end();
  return response;
}