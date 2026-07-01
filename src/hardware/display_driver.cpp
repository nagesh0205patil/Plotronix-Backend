#include "display_driver.h"

/**
 * Constructor initializes the TFT driver with defined SPI pins.
 */
DisplayDriver::DisplayDriver() : tft(TFT_CS, TFT_DC, TFT_RST) {
}

/**
 * Initialize the TFT display hardware and prepare a blank screen.
 */
void DisplayDriver::init() {
    Serial.println("[DEBUG] DisplayDriver::init() - Starting display initialization");
    Serial.print("[DEBUG] TFT_CS=");
    Serial.print(TFT_CS);
    Serial.print(" TFT_DC=");
    Serial.print(TFT_DC);
    Serial.print(" TFT_RST=");
    Serial.println(TFT_RST);

    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_RST, HIGH);

    digitalWrite(TFT_RST, LOW);
    delay(50);
    digitalWrite(TFT_RST, HIGH);
    delay(150);

    SPI.begin();

    const uint8_t initVariants[] = {INITR_BLACKTAB, INITR_REDTAB, INITR_GREENTAB};
    const uint8_t variantCount = sizeof(initVariants) / sizeof(initVariants[0]);
    bool initialized = false;
    uint8_t selectedVariant = TFT_INITR;

    for (uint8_t i = 0; i < variantCount; ++i) {
        const uint8_t variant = initVariants[i];
        unsigned long startTime = millis();
        tft.initR(variant);
        unsigned long elapsed = millis() - startTime;

        const int16_t width = tft.width();
        const int16_t height = tft.height();
        Serial.print("[DEBUG] initR variant ");
        Serial.print(variant);
        Serial.print(" completed in ");
        Serial.print(elapsed);
        Serial.print("ms (size=");
        Serial.print(width);
        Serial.print("x");
        Serial.print(height);
        Serial.println(")");

        if (width >= 120 && height >= 120) {
            initialized = true;
            selectedVariant = variant;
            break;
        }
    }

    if (!initialized) {
        Serial.println("[WARN] TFT panel geometry could not be verified; using the configured init variant");
    }

    Serial.print("[DEBUG] Selected TFT init variant: ");
    Serial.println(selectedVariant);

    tft.setRotation(TFT_ROTATION);
    tft.setTextWrap(false);
    tft.cp437(true);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.println("Display OK");
    tft.setCursor(0, 16);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    tft.println("Panel ready");

    Serial.println("[DEBUG] TFT screen cleared and ready for full-screen rendering");
}

/**
 * Fill the display with a solid RGB565 color.
 * @param color Color value to fill the screen.
 */
void DisplayDriver::fillScreen(uint16_t color) {
    tft.fillScreen(color);
}

/**
 * Set the color used for subsequent text output.
 * @param color RGB565 text color.
 */
void DisplayDriver::setTextColor(uint16_t color) {
    tft.setTextColor(color);
}

void DisplayDriver::setTextColor(uint16_t color, uint16_t bgColor) {
    tft.setTextColor(color, bgColor);
}

/**
 * Set the text scale for future text writes.
 * @param size Text size multiplier.
 */
void DisplayDriver::setTextSize(uint8_t size) {
    tft.setTextSize(size);
}

/**
 * Move the text cursor to a new location on the display.
 * @param x X coordinate in pixels.
 * @param y Y coordinate in pixels.
 */
void DisplayDriver::setCursor(int16_t x, int16_t y) {
    tft.setCursor(x, y);
}

/**
 * Print a null-terminated string at the current cursor position.
 * @param text Text to print.
 */
void DisplayDriver::print(const char* text) {
    tft.print(text);
}

/**
 * Print a null-terminated string and append a newline.
 * @param text Text to print.
 */
void DisplayDriver::println(const char* text) {
    tft.println(text);
}

/**
 * Print a floating point value with a fixed decimal precision.
 * @param value Numeric value to print.
 * @param decimals Decimal precision.
 */
void DisplayDriver::print(float value, int decimals) {
    tft.print(value, decimals);
}
