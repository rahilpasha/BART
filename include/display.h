#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <TFT_eSPI.h>
#include "TransitData.h"

typedef enum {
  Transit,
  Todoist
} ScreenType;

#define BUTTON_X_MIN 100
#define BUTTON_X_MAX 220
#define BUTTON_Y_MIN 10
#define BUTTON_Y_MAX 35

void displayInit();
void displayHeader(ScreenType screen);
void displayTransitSetup(void);
void displayBARTData(TransitData BARTData);
void displayBusData(TransitData BusData);
void displayTasks(std::vector<String> tasks);

#endif