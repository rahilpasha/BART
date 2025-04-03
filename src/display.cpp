#include "display.h"

TFT_eSPI tft = TFT_eSPI();

void displaySetup(void) {
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(0x4208);
	tft.setTextColor(TFT_WHITE);

	tft.fillRoundRect(10, 30, 300, 75, 10, 0x04d9);

	tft.fillRoundRect(10, 125, 300, 75, 10, 0x036a);
}

void displayBARTData(TransitData BARTData) {

	tft.fillRoundRect(10, 30, 300, 75, 10, 0x04d9);

	tft.setTextSize(2.5);
	tft.setCursor(20, 40);
	tft.println("BART");

	tft.setTextSize(2);
	tft.setCursor(20, 70);
	tft.println(BARTData.route);

	tft.setCursor(150, 45);
	tft.println("Minutes: " + BARTData.minutes);
	tft.setCursor(150, 70);
	tft.println("Delay: " + BARTData.delay);

}

void displayBusData(TransitData busData) {

	tft.fillRoundRect(10, 125, 300, 75, 10, 0x036a);

	tft.setTextSize(2.5);
	tft.setCursor(20, 135);
	tft.println("AC Transit");

	tft.setTextSize(2);
	tft.setCursor(20, 165);
	tft.println("Route: " + busData.route);

	tft.setCursor(150, 140);
	tft.println("Minutes: " + busData.minutes);
	tft.setCursor(150, 165);
	tft.println("Delay: " + busData.delay);

}