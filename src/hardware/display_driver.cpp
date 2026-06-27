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
    // Configure TFT control pins as outputs and drive known states
    pinMode(TFT_CS, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    digitalWrite(TFT_DC, HIGH);
    digitalWrite(TFT_RST, HIGH);

    Serial.print("[DEBUG] Pin states (digitalRead): CS=");
    Serial.print(digitalRead(TFT_CS));
    Serial.print(" DC=");
    Serial.print(digitalRead(TFT_DC));
    Serial.print(" RST=");
    Serial.println(digitalRead(TFT_RST));

    // Pulse RST low to ensure display resets
    Serial.println("[DEBUG] Resetting display (pulse RST low) ...");
    digitalWrite(TFT_RST, LOW);
    delay(50);
    digitalWrite(TFT_RST, HIGH);
    delay(150);

    Serial.println("[DEBUG] SPI.begin() ...");
    SPI.begin();
    Serial.println("[DEBUG] SPI.begin() complete");
    
    Serial.print("[DEBUG] Attempting tft.initR() with timeout (2s max)...");
    Serial.print("[DEBUG] Using INITR variant: ");
#if TFT_INITR == INITR_BLACKTAB
    Serial.println("INITR_BLACKTAB");
#elif TFT_INITR == INITR_GREENTAB
    Serial.println("INITR_GREENTAB");
#elif TFT_INITR == INITR_REDTAB
    Serial.println("INITR_REDTAB");
#else
    Serial.println("UNKNOWN");
#endif

    unsigned long startTime = millis();
    tft.initR(TFT_INITR);
    unsigned long elapsed = millis() - startTime;
    
    Serial.println("[DEBUG] tft.initR() returned after ");
    Serial.print(elapsed);
    Serial.println("ms");

    if (elapsed > 2000) {
        Serial.println("[WARN] tft.initR() took > 2 seconds (possible SPI comms issue)");
        Serial.println("[WARN] Check TFT wiring: CS=D10, DC=D9, RST=D8");
        Serial.println("[WARN] Check SPI wiring: MOSI=D11, SCK=D13, CS/DC/RST as above");
        Serial.println("[WARN] Check power to display (5V or 3.3V depending on module)");
    }

    // Attempt to read display ID to verify SPI communication
    Serial.println("[DEBUG] Attempting to read display ID register via SPI...");
    digitalWrite(TFT_CS, LOW);
    SPI.transfer(0x04);  // Read ID command
    uint8_t id1 = SPI.transfer(0x00);
    uint8_t id2 = SPI.transfer(0x00);
    uint8_t id3 = SPI.transfer(0x00);
    digitalWrite(TFT_CS, HIGH);
    Serial.print("[DEBUG] Display ID bytes: 0x");
    Serial.print(id1, HEX);
    Serial.print(" 0x");
    Serial.print(id2, HEX);
    Serial.print(" 0x");
    Serial.println(id3, HEX);

#if TFT_INITR == INITR_BLACKTAB
    Serial.println("[DEBUG] Display initialized with INITR_BLACKTAB");
#elif TFT_INITR == INITR_GREENTAB
    Serial.println("[DEBUG] Display initialized with INITR_GREENTAB");
#elif TFT_INITR == INITR_REDTAB
    Serial.println("[DEBUG] Display initialized with INITR_REDTAB");
#else
    Serial.println("[DEBUG] Display initialized with UNKNOWN INITR");
#endif

    tft.setRotation(1);
    Serial.println("[DEBUG] Rotation set to 1");

    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.println("Display OK");
    tft.setCursor(0, 16);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    tft.println("If blank, check");
    tft.setCursor(0, 32);
    tft.println("TFT wiring and");
    tft.setCursor(0, 48);
    tft.println("INITR variant.");

    Serial.println("[DEBUG] Screen filled with black and visible test text drawn");
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
