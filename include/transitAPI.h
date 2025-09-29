#ifndef TRANSITAPI_H
#define TRANSITAPI_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <vector>
#include <set>

#include "credentials.h"

struct TransitData {
	String route;
	String minutes;
	String delay;
  };

String callTransitAPI(String api_url);
TransitData updateBARTData(String station);
TransitData updateBusData(String station, std::set<String> routes);
String departureToMinutes(String departure);

#endif