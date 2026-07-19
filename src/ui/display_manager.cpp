#include "display_manager.h"

namespace {
constexpr int16_t TABLE_X = 6;
constexpr int16_t TABLE_Y = 24;
constexpr int16_t TABLE_WIDTH = 148;
constexpr int16_t TABLE_COLUMN_X = 68;
constexpr int16_t TABLE_ROW_HEIGHT = 14;
constexpr uint8_t TABLE_DATA_ROWS = 6;
constexpr int16_t TABLE_HEIGHT = TABLE_ROW_HEIGHT * (TABLE_DATA_ROWS + 1);
}

/**
 * Initialize display subsystem and show the startup sequence.
 */
void DisplayManager::init() {
    driver.init();
    currentScreen = SCREEN_WELCOME;
    lastDetectedType = UNKNOWN;
    screenStartTime = millis();
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
            lastDetectedType = UNKNOWN;
            componentLayoutDrawn = false;
            showWaitingScreen();
        }
        return;
    }

    // If a component is detected, draw static layout once and update values incrementally
    if (componentDetected) {
        if (currentScreen != SCREEN_COMPONENT ||
            comp.type != lastDetectedType ||
            !componentLayoutDrawn) {
            // New component selected: draw full layout
            currentScreen = SCREEN_COMPONENT;
            lastDetectedType = comp.type;
            componentLayoutDrawn = false;
            drawComponentLayout(comp);
            updateMeasurementValues(data);
            componentLayoutDrawn = true;
            lastSensorData = data;
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

void DisplayManager::drawComponentLayout(const ComponentInfo& comp) {
    clearAndReset();
    Adafruit_ST7735 &tft = driver.getTFT();

    const int16_t screenWidth = tft.width();
    const uint16_t tableColor = ST77XX_CYAN;

    driver.setTextColor(0x07E0, ST77XX_BLACK);
    driver.setTextSize(2);
    int16_t nx = (screenWidth - (int16_t)strlen(comp.name) * 6 * 2) / 2;
    if (nx < 0) nx = 0;
    driver.setCursor(nx, 3);
    driver.println(comp.name);

    // Framed two-column table: one header row followed by six data rows.
    tft.drawRect(TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT, tableColor);
    tft.drawFastVLine(TABLE_COLUMN_X, TABLE_Y, TABLE_HEIGHT, tableColor);
    for (uint8_t row = 1; row <= TABLE_DATA_ROWS; ++row) {
        tft.drawFastHLine(
            TABLE_X,
            TABLE_Y + row * TABLE_ROW_HEIGHT,
            TABLE_WIDTH,
            tableColor);
    }

    driver.setTextSize(1);
    driver.setTextColor(tableColor, ST77XX_BLACK);
    driver.setCursor(TABLE_X + 4, TABLE_Y + 3);
    driver.print("PARAMETER");
    driver.setCursor(TABLE_COLUMN_X + 27, TABLE_Y + 3);
    driver.print("VALUE");

    const char* labels[] = {
        "VP1",
        "IP1",
        "Vdut",
        "Idut",
        "VP2",
        "IP2"
    };

    driver.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    for (uint8_t i = 0; i < TABLE_DATA_ROWS; ++i) {
        const int16_t labelWidth = (int16_t)strlen(labels[i]) * 6;
        const int16_t labelX = TABLE_X + (TABLE_COLUMN_X - TABLE_X - labelWidth) / 2;
        const int16_t y = TABLE_Y + (i + 1) * TABLE_ROW_HEIGHT + 3;
        driver.setCursor(labelX, y);
        driver.print(labels[i]);
    }
}

void DisplayManager::updateMeasurementValues(const SensorData& data) {
    Adafruit_ST7735 &tft = driver.getTFT();
    const int16_t valueCellX = TABLE_COLUMN_X + 1;
    const int16_t valueCellWidth = TABLE_X + TABLE_WIDTH - valueCellX - 1;

    auto drawValueAt = [&](uint8_t row, const char* text) {
        const int16_t cellY = TABLE_Y + (row + 1) * TABLE_ROW_HEIGHT + 1;
        const int16_t textWidth = (int16_t)strlen(text) * 6;
        const int16_t textX = valueCellX + (valueCellWidth - textWidth) / 2;
        tft.fillRect(valueCellX, cellY, valueCellWidth, TABLE_ROW_HEIGHT - 2, ST77XX_BLACK);
        driver.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
        driver.setTextSize(1);
        driver.setCursor(textX, cellY + 2);
        driver.println(text);
    };

    const float vEps = 0.01f;
    const float iEps = 0.5f;
    const bool voltageChanged =
        fabs(data.voltage - lastSensorData.voltage) > vEps || !componentLayoutDrawn;
    const float currentMilliAmps = data.current * 1000.0f;
    const float lastCurrentMilliAmps = lastSensorData.current * 1000.0f;
    const bool currentChanged =
        fabs(currentMilliAmps - lastCurrentMilliAmps) > iEps || !componentLayoutDrawn;
    char voltageText[24];
    char currentText[24];

    if (voltageChanged) {
        snprintf(voltageText, sizeof(voltageText), "%.2f V", (double)data.voltage);
        drawValueAt(0, voltageText);
        drawValueAt(2, voltageText);
        drawValueAt(4, voltageText);
    }

    if (currentChanged) {
        snprintf(currentText, sizeof(currentText), "%d mA", (int)roundf(currentMilliAmps));
        drawValueAt(1, currentText);
        drawValueAt(3, currentText);
        drawValueAt(5, currentText);
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
