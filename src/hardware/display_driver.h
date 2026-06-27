#ifndef PLOTRONIX_DISPLAY_DRIVER_H
#define PLOTRONIX_DISPLAY_DRIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "../data/config.h"

/* ============================================================================
 * DISPLAY DRIVER
 * Hardware abstraction layer for TFT display control
 * ============================================================================ */

/**
 * @class DisplayDriver
 * Provides low-level hardware interface to ST7735 TFT display
 */
class DisplayDriver {
public:
    /**
     * Constructor - initialize member TFT driver
     */
    DisplayDriver();

    /**
     * Initialize the display hardware
     * Sets up SPI, pins, and initializes the ST7735 controller
     */
    void init();

    /**
     * Fill entire screen with a color
     * @param color 16-bit RGB565 color value
     */
    void fillScreen(uint16_t color);

    /**
     * Set text drawing color
     * @param color 16-bit RGB565 color value
     */
    void setTextColor(uint16_t color);
    void setTextColor(uint16_t color, uint16_t bgColor);

    /**
     * Set text size/scale
     * @param size Multiplier for character size (1=default)
     */
    void setTextSize(uint8_t size);

    /**
     * Set cursor position for text drawing
     * @param x X coordinate in pixels
     * @param y Y coordinate in pixels
     */
    void setCursor(int16_t x, int16_t y);

    /**
     * Print text at current cursor position
     * @param text Text string to print
     */
    void print(const char* text);

    /**
     * Print text with newline at current cursor position
     * @param text Text string to print
     */
    void println(const char* text);

    /**
     * Print numeric value
     * @param value Float value to print
     * @param decimals Number of decimal places
     */
    void print(float value, int decimals);

    /**
     * Get reference to underlying Adafruit_ST7735 object (for advanced use)
     */
    Adafruit_ST7735& getTFT() { return tft; }

private:
    Adafruit_ST7735 tft;  ///< Underlying TFT driver object
};

#endif // PLOTRONIX_DISPLAY_DRIVER_H
