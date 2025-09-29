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
#if DEBUG
    Serial.println("HTTP Response code: " + String(httpResponseCode));
#endif
  } else {
    Serial.println("Error on HTTP request: " + String(httpResponseCode));
  }
  
  http.end();
  return response;
}

std::vector<String> processJsonResponse(String jsonResponse) {

  JSONVar todoist_json = JSON.parse(jsonResponse);
  std::vector<String> tasks;
  for (int i = 0; i < todoist_json.length(); i++) {
    tasks.push_back(todoist_json[i]["content"]);
  }
  return tasks;
}