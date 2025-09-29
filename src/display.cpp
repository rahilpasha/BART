#include "display.h"

TFT_eSPI tft = TFT_eSPI();

void displayInit() {
	tft.init();
	tft.setRotation(1);
	tft.fillScreen(0x4208);
}

void displayHeader(ScreenType screen) {
	tft.fillScreen(0x4208);

	tft.setTextColor(TFT_BLACK);

	tft.fillRoundRect(SCREEN_BTN_X_MIN, SCREEN_BTN_Y_MIN, SCREEN_BTN_X_MAX - SCREEN_BTN_X_MIN, SCREEN_BTN_Y_MAX - SCREEN_BTN_Y_MIN, 10, TFT_LIGHTGREY);

	if (screen == Transit) {
		tft.setTextSize(2);
		tft.setCursor(115, 15);
		tft.println("Transit");
	} else if (screen == Todoist) {
		tft.setTextSize(2);
		tft.setCursor(115, 15);
		tft.println("Todoist");
	}

	tft.fillRoundRect(REFRESH_BTN_X_MIN, REFRESH_BTN_Y_MIN, REFRESH_BTN_X_MAX - REFRESH_BTN_X_MIN, REFRESH_BTN_Y_MAX - REFRESH_BTN_Y_MIN, 10, TFT_LIGHTGREY);

	drawRefreshSymbol(REFRESH_BTN_X_MIN + 2, REFRESH_BTN_Y_MIN + 2, TFT_BLACK);

}

void drawRefreshSymbol(int x, int y, uint16_t color) {
  // Upper arc (now on the right side)
  tft.drawCircleHelper(x + 10, y + 10, 7, 2, color);
  tft.drawCircleHelper(x + 10, y + 10, 6, 2, color);
  
  // Lower arc (now on the left side)
  tft.drawCircleHelper(x + 10, y + 10, 7, 8, color);
  tft.drawCircleHelper(x + 10, y + 10, 6, 8, color);
  
  // Top arrow (pointing left)
  tft.fillTriangle(x + 4, y + 4, x + 1, y + 7, x + 7, y + 7, color);
  
  // Bottom arrow (pointing right)
  tft.fillTriangle(x + 16, y + 16, x + 19, y + 13, x + 13, y + 13, color);
}

void displayTransitSetup(void) {

	tft.fillRoundRect(10, 50, 300, 75, 10, 0x04d9);

	tft.fillRoundRect(10, 145, 300, 75, 10, 0x036a);
}

void displayBARTData(TransitData BARTData) {

	tft.fillRoundRect(10, 50, 300, 75, 10, 0x04d9);

	tft.setTextSize(2.5);
	tft.setCursor(20, 60);
	tft.println("BART");

	tft.setTextSize(2);
	tft.setCursor(20, 90);
	tft.println(BARTData.route);

	tft.setCursor(150, 65);
	tft.println("Minutes: " + BARTData.minutes);
	tft.setCursor(150, 90);
	tft.println("Delay: " + BARTData.delay);

}

void displayBusData(TransitData busData) {

	tft.fillRoundRect(10, 145, 300, 75, 10, 0x036a);

	tft.setTextSize(2.5);
	tft.setCursor(20, 155);
	tft.println("AC Transit");

	tft.setTextSize(2);
	tft.setCursor(20, 185);
	tft.println("Route: " + busData.route);

	tft.setCursor(150, 160);
	tft.println("Minutes: " + busData.minutes);
	tft.setCursor(150, 185);
	tft.println("Delay: " + busData.delay);

}

void displayTasks(std::vector<String> tasks) {
	tft.fillRoundRect(10, 50, 300, 170, 10, 0x04d9);

	tft.setTextSize(2.5);
	tft.setCursor(20, 60);

	tft.setTextSize(2);
	int y = 60;
	for (String task : tasks) {
		tft.setCursor(20, y);
		tft.println("- " + task);
		y += 25;
		if (y > 200) {
			break; // Prevent overflow
		}
	}
}