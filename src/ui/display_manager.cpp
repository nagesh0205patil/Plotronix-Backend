#include "display_manager.h"

/**
 * Initialize display subsystem and show the startup splash screen.
 */
void DisplayManager::init() {
    driver.init();
    showStartupScreen();
}

/**
 * Show the startup screen with the product name and initialization message.
 */
void DisplayManager::showStartupScreen() {
    clearAndReset();

    auto centerX = [](const char* text, uint8_t size) {
        int16_t length = strlen(text);
        return (128 - (length * 6 * size)) / 2;
    };

    driver.setTextColor(ST77XX_WHITE);

    driver.setTextSize(1);
    driver.setCursor(centerX("Welcome To", 1), 25);
    driver.println("Welcome To");

    driver.setTextSize(2);
    driver.setCursor(centerX("Plotronix", 2), 45);
    driver.println("Plotronix");

    driver.setTextSize(1);
    driver.setCursor(centerX("Powered By", 1), 80);
    driver.println("Powered By");

    driver.setCursor(centerX("CloudTronix", 1), 98);
    driver.println("CloudTronix");

    driver.setCursor(centerX("Solutions Pvt Ltd", 1), 110);
    driver.println("Solutions Pvt Ltd");
}

/**
 * Update the measurement screen with the latest component and sensor data.
 * Rate-limits refreshes to avoid flicker and excessive redraw.
 */
void DisplayManager::updateMeasurementDisplay(const ComponentInfo& comp, const SensorData& data) {
    // Rate limit display updates to avoid excessive redraw
    if (millis() - lastDisplayUpdate < DISPLAY_REFRESH) return;
    lastDisplayUpdate = millis();

    clearAndReset();

    // Display component name
    driver.setCursor(0, 10);
    driver.setTextSize(1);
    driver.setTextColor(0x07E0);  // Green in RGB565
    driver.print("Comp: ");
    driver.println(comp.name);

    // Display voltage reading
    driver.setCursor(0, 40);
    driver.print("V: ");
    driver.print(data.voltage, 2);
    driver.println(" V");

    // Display current reading (in mA)
    driver.setCursor(0, 70);
    driver.print("I: ");
    driver.print(data.current * 1000, 2);
    driver.println(" mA");
}

/**
 * Clear the display and restore the default text style.
 */
void DisplayManager::clearAndReset() {
    driver.fillScreen(ST77XX_BLACK);
    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    driver.setTextSize(1);
}
