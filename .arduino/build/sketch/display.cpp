#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\display.cpp"
#include "display.h"
#include "config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void initDisplay() {
    tft.initR(INITR_MINI160x80);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}

void showStartupScreen() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1);

    tft.setCursor(10, 10);
    tft.println("Plotronix");

    tft.setCursor(10, 25);
    tft.println("VI Characteristics Lab");

    tft.setCursor(10, 45);
    tft.println("Built by");

    tft.setCursor(10, 60);
    tft.println("CloudTronix");

    tft.setCursor(10, 68);
    tft.println("Initializing...");
}

void updateDisplay(ComponentInfo comp, SensorData data) {

    static unsigned long lastUpdate = 0;

    if (millis() - lastUpdate < DISPLAY_REFRESH) return;
    lastUpdate = millis();

    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(0, 10);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_GREEN);

    tft.print("Comp: ");
    tft.println(comp.name);

    tft.setCursor(0, 40);
    tft.print("V: ");
    tft.print(data.voltage, 2);
    tft.println(" V");

    tft.setCursor(0, 70);
    tft.print("I: ");
    tft.print(data.current * 1000, 2);
    tft.println(" mA");
}
