#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\config.h"
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- ADC Input Pins ---
#define PIN_VOLTAGE     A2      // Voltage across DUT
#define PIN_CURRENT     A3      // Voltage across shunt resistor (for current sense)
#define PIN_SELECTOR    A4      // Rotary selector wheel position

// --- TFT Display Pins ---
#define TFT_CS          6
#define TFT_DC          7
#define TFT_RST         5

// --- ADC / Voltage Reference ---
#define ADC_RESOLUTION  1023.0
#define VREF            5.0

// --- Current Sensing ---
#define SHUNT_RESISTOR  100.0   // Shunt resistor value in ohms

// --- Sampling ---
#define NUM_SAMPLES     10      // ADC averages per reading

// --- Timing (ms) ---
#define STARTUP_DELAY   15000   // Startup screen hold time
#define DISPLAY_REFRESH 200     // Minimum ms between display updates
#define SERIAL_RATE     50      // Minimum ms between serial TX frames

// --- Serial ---
#define SERIAL_BAUD     9600

#endif
