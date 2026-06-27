#ifndef PLOTRONIX_CONFIG_H
#define PLOTRONIX_CONFIG_H

#include <Arduino.h>

/* ============================================================================
 * PLOTRONIX VI LAB CONFIGURATION
 * Central configuration file for all hardware pins, timing, and constants
 * ============================================================================ */

// --- ADC Input Pins ---
#define PIN_VOLTAGE     A2      // Voltage across DUT
#define PIN_CURRENT     A3      // Voltage across shunt resistor (for current sense)
#define PIN_SELECTOR    A4      // Rotary selector wheel position

// --- TFT Display Pins ---
#define TFT_CS          10      // Chip Select (D10)
#define TFT_DC          9       // Data/Command (D9)
#define TFT_RST         8       // Reset (D8)
#define TFT_INITR       INITR_REDTAB // Display controller initialization type
//#define TFT_INITR    INITR_GREENTAB // Uncomment if redtab does not work
//#define TFT_INITR    INITR_BLACKTAB   // Uncomment if redtab does not work

// --- ADC Configuration ---
#define ADC_RESOLUTION  1023.0  // 10-bit ADC max value
#define VREF            5.0     // Reference voltage (Vcc)

// --- Current Sensing ---
#define SHUNT_RESISTOR  10000.0   // Shunt resistor value in ohms

// --- Sampling ---
#define NUM_SAMPLES     10      // Number of ADC samples to average

// --- Timing (milliseconds) ---
#define STARTUP_DELAY   15000   // Startup screen display duration
#define DISPLAY_REFRESH 200     // Minimum time between display updates
#define SERIAL_RATE     50      // Minimum time between serial transmissions

// --- Serial Communication ---
#define SERIAL_BAUD     9600    // Serial baud rate

#endif // PLOTRONIX_CONFIG_H
