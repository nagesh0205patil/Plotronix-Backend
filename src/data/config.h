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

// --- Digital Selector Switch Pins ---
// Four SPST toggle switches wired with external 4.7K pull-ups to +5V.
// Switch OFF reads HIGH; switch ON reads LOW. Inverted in software.
#define PIN_SELECTOR_SWITCH_0  2   // D2 - Bit 0 (LSB)
#define PIN_SELECTOR_SWITCH_1  3   // D3 - Bit 1
#define PIN_SELECTOR_SWITCH_2  4   // D4 - Bit 2
#define PIN_SELECTOR_SWITCH_3  5   // D5 - Bit 3 (MSB)

// --- TFT Display Pins ---
#define TFT_CS          10      // Chip Select (D10)
#define TFT_DC          8       // Data/Command (D9)
#define TFT_RST         9       // Reset (D8)
#define TFT_INITR       INITR_REDTAB // Default variant; the driver will fall back if needed
#define TFT_ROTATION    1       // 1 = landscape, 0/2 = portrait
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
#define WELCOME_SCREEN_DURATION      5000    // Welcome screen display duration
#define INITIALIZATION_SCREEN_DURATION 5000   // Initialization screen display duration
#define DISPLAY_REFRESH              200     // Minimum time between display updates
#define SERIAL_RATE                  50      // Minimum time between serial transmissions
#define SELECTOR_STABILITY_TIME      25      // Time a selector value must remain stable before use

// Set to 1 while diagnosing selector input. Logs are disabled by default because
// verbose output at 9600 baud can itself delay the main loop.
#define SELECTOR_DEBUG_LOGS          0

// --- Serial Communication ---
#define SERIAL_BAUD     9600    // Serial baud rate

#endif // PLOTRONIX_CONFIG_H
