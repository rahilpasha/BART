#ifndef TODOISTAPI_H
#define TODOISTAPI_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <vector>

#include "credentials.h"

static String todoistAPIBase = "https://api.todoist.com/rest/v2";

String callTodoistAPI(String api_url);
std::vector<String> processJsonResponse(String jsonResponse);

#endif