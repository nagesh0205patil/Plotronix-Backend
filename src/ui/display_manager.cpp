#include "display_manager.h"

/**
 * Initialize display subsystem and show the startup sequence.
 */
void DisplayManager::init() {
    driver.init();
    currentScreen = SCREEN_WELCOME;
    lastDetectedType = UNKNOWN;
    screenStartTime = millis();
    lastDisplayUpdate = 0;
    showWelcomeScreen();
}

/**
 * Update the active screen based on startup timers and detected component state.
 */
void DisplayManager::updateMeasurementDisplay(const ComponentInfo& comp, const SensorData& data) {
    const unsigned long now = millis();
    const bool componentDetected = (comp.type != UNKNOWN);

    if (currentScreen == SCREEN_WELCOME) {
        if (now - screenStartTime >= WELCOME_SCREEN_DURATION) {
            currentScreen = SCREEN_INITIALIZING;
            screenStartTime = now;
            showInitializingScreen();
        }
        return;
    }

    if (currentScreen == SCREEN_INITIALIZING) {
        if (now - screenStartTime >= INITIALIZATION_SCREEN_DURATION) {
            currentScreen = SCREEN_WAITING;
            screenStartTime = now;
            showWaitingScreen();
        }
        return;
    }

    if (!componentDetected) {
        if (currentScreen != SCREEN_WAITING) {
            currentScreen = SCREEN_WAITING;
            showWaitingScreen();
        }
        return;
    }

    // If a component is detected, draw static layout once and update values incrementally
    if (componentDetected) {
        if (comp.type != lastDetectedType) {
            // New component selected: draw full layout
            currentScreen = SCREEN_COMPONENT;
            lastDetectedType = comp.type;
            componentLayoutDrawn = false;
            drawComponentLayout(comp);
            updateMeasurementValues(data);
            lastSensorData = data;
            lastDisplayUpdate = now;
        } else {
            // Same component: update only measurement values when they change
            updateMeasurementValues(data);
            lastSensorData = data;
        }
    }
}

/**
 * Render the welcome screen with the product branding.
 */
void DisplayManager::showWelcomeScreen() {
    clearAndReset();
    Adafruit_ST7735 &tft = driver.getTFT();

    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

    struct LineSpec {
        const char* text;
        uint8_t size;
    } lines[] = {
        {"Welcome To", 1},
        {"Plotronix", 2},
        {"Powered By", 1},
        {"CloudTronix", 1},
        {"Solutions Pvt Ltd", 1}
    };

    const uint8_t lineGap = 6;
    uint8_t totalHeight = 0;
    for (uint8_t i = 0; i < 5; ++i) {
        totalHeight += (lines[i].size == 2 ? 16 : 8) + lineGap;
    }
    totalHeight -= lineGap;

    int16_t y = (tft.height() - totalHeight) / 2;
    for (uint8_t i = 0; i < 5; ++i) {
        const uint8_t size = lines[i].size;
        const int16_t lineHeight = (size == 2 ? 16 : 8);
        const int16_t width = (int16_t)strlen(lines[i].text) * 6 * size;
        const int16_t x = (tft.width() - width) / 2;

        driver.setTextSize(size);
        driver.setCursor(x, y);
        driver.println(lines[i].text);

        y += lineHeight + lineGap;
    }
}

/**
 * Render the initialization screen.
 */
void DisplayManager::showInitializingScreen() {
    clearAndReset();
    Adafruit_ST7735 &tft = driver.getTFT();
    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    driver.setTextSize(1);

    const char* msg = "Initializing...";
    int16_t x = (tft.width() - (int16_t)strlen(msg) * 6) / 2;
    int16_t y = (tft.height() - 8) / 2;
    driver.setCursor(x, y);
    driver.println(msg);
}

/**
 * Render the waiting screen while no valid component has been detected.
 */
void DisplayManager::showWaitingScreen() {
    clearAndReset();
    Adafruit_ST7735 &tft = driver.getTFT();
    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    driver.setTextSize(1);

    const char* line1 = "Waiting for";
    const char* line2 = "Component Selection";
    int16_t y = (tft.height() - 8 - 8 - 6) / 2;

    int16_t x1 = (tft.width() - (int16_t)strlen(line1) * 6) / 2;
    int16_t x2 = (tft.width() - (int16_t)strlen(line2) * 6) / 2;

    driver.setCursor(x1, y);
    driver.println(line1);
    driver.setCursor(x2, y + 12);
    driver.println(line2);
}

/**
 * Render the detected component information screen.
 */
void DisplayManager::showComponentScreen(const ComponentInfo& comp, const SensorData& data) {
    // Full layout drawing replaced by drawComponentLayout + incremental updates
    drawComponentLayout(comp);
    updateMeasurementValues(data);
}

void DisplayManager::drawComponentLayout(const ComponentInfo& comp) {
    clearAndReset();
    Adafruit_ST7735 &tft = driver.getTFT();

    const int16_t screenWidth = tft.width();
    const int16_t screenHeight = tft.height();
    const int16_t headerY = 4;
    const int16_t dividerY = 20;
    const int16_t contentTop = 32; // increased gap after component name
    const int16_t rowHeight = max(14, (screenHeight - contentTop - 8) / 4);
    const int16_t labelX = 3;
    const int16_t valueX = screenWidth / 2 + 2;

    driver.setTextColor(0x07E0, ST77XX_BLACK);
    driver.setTextSize(2);
    int16_t nx = (screenWidth - (int16_t)strlen(comp.name) * 6 * 2) / 2;
    if (nx < 0) nx = 0;
    driver.setCursor(nx, headerY);
    driver.println(comp.name);

    tft.drawFastHLine(0, dividerY, screenWidth, ST77XX_WHITE);

    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    driver.setTextSize(1);

    const char* labels[] = {
        "Input Voltage : ",
        "Input Current : ",
        "DUT Voltage   : ",
        "DUT Current   : "
    };

    for (uint8_t i = 0; i < 4; ++i) {
        const int16_t y = contentTop + i * rowHeight;
        driver.setCursor(labelX, y);
        driver.print(labels[i]);
        driver.setCursor(valueX, y);
        driver.println("--");
    }

    componentLayoutDrawn = true;
}

void DisplayManager::updateMeasurementValues(const SensorData& data) {
    Adafruit_ST7735 &tft = driver.getTFT();
    const int16_t screenHeight = tft.height();
    const int16_t contentTop = 24;
    const int16_t rowHeight = max(14, (screenHeight - contentTop - 8) / 4);
    const int16_t valueX = tft.width() / 2 + 2;
    const int16_t valueWidth = max(34, tft.width() - valueX - 3);
    const uint8_t size = 1;

    auto drawValueAt = [&](int16_t y, const char* text) {
        driver.getTFT().fillRect(valueX, y, valueWidth, rowHeight - 2, ST77XX_BLACK);
        driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        driver.setTextSize(size);
        driver.setCursor(valueX, y);
        driver.println(text);
    };

    const float vEps = 0.01f;
    const float iEps = 0.5f;
    char buf[24];

    if (fabs(data.voltage - lastSensorData.voltage) > vEps || !componentLayoutDrawn) {
        snprintf(buf, sizeof(buf), "%.2f V", (double)data.voltage);
        drawValueAt(contentTop, buf);
    }

    float in_mA = data.current * 1000.0f;
    float last_in_mA = lastSensorData.current * 1000.0f;
    if (fabs(in_mA - last_in_mA) > iEps || !componentLayoutDrawn) {
        snprintf(buf, sizeof(buf), "%d mA", (int)roundf(in_mA));
        drawValueAt(contentTop + rowHeight, buf);
    }

    if (fabs(data.voltage - lastSensorData.voltage) > vEps || !componentLayoutDrawn) {
        snprintf(buf, sizeof(buf), "%.2f V", (double)data.voltage);
        drawValueAt(contentTop + rowHeight * 2, buf);
    }

    float dut_mA = data.current * 1000.0f;
    float last_dut_mA = lastSensorData.current * 1000.0f;
    if (fabs(dut_mA - last_dut_mA) > iEps || !componentLayoutDrawn) {
        snprintf(buf, sizeof(buf), "%d mA", (int)roundf(dut_mA));
        drawValueAt(contentTop + rowHeight * 3, buf);
    }
}

/**
 * Clear the display and restore the default text style.
 */
void DisplayManager::clearAndReset() {
    driver.fillScreen(ST77XX_BLACK);
    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    driver.setTextSize(1);
}
