#ifndef DISPLAY_H
#define DISPLAY_H

#include <vector>
#include <TFT_eSPI.h>
#include "transitAPI.h"

typedef enum {
  Transit,
  Todoist
} ScreenType;

#define SCREEN_BTN_X_MIN 100
#define SCREEN_BTN_X_MAX 220
#define SCREEN_BTN_Y_MIN 10
#define SCREEN_BTN_Y_MAX 35

#define REFRESH_BTN_X_MIN 290
#define REFRESH_BTN_X_MAX 315
#define REFRESH_BTN_Y_MIN 10
#define REFRESH_BTN_Y_MAX 35

void displayInit();
void displayHeader(ScreenType screen);
void drawRefreshSymbol(int x, int y, uint16_t color);
void displayTransitSetup(void);
void displayBARTData(TransitData BARTData);
void displayBusData(TransitData BusData);
void displayTasks(std::vector<String> tasks);

#endif