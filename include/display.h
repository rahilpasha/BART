#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "TransitData.h"

void displaySetup(void);
void displayBARTData(TransitData BARTData);
void displayBusData(TransitData BusData);

#endif