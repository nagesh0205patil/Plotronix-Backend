#ifndef PLOTRONIX_ADC_DRIVER_H
#define PLOTRONIX_ADC_DRIVER_H

#include <Arduino.h>
#include "../data/sensor_data.h"
#include "../data/config.h"

/* ============================================================================
 * ADC DRIVER
 * Hardware abstraction layer for analog-to-digital conversion
 * ============================================================================ */

/**
 * @class ADCDriver
 * Provides abstraction for reading ADC inputs with averaging and conversion
 */
class ADCDriver {
public:
    /**
     * Initialize the ADC driver
     * Must be called once during setup
     */
    void init();

    /**
     * Read all sensor channels and return calibrated data
     * @return SensorData containing voltage, current, and selector readings
     */
    SensorData readAllSensors();

    /**
     * Read raw ADC value from a specific pin with averaging
     * @param pin Arduino analog pin
     * @return Averaged ADC value (0.0 - VREF)
     */
    float readRawVoltage(int pin);

private:
    /**
     * Read and average a single ADC pin over NUM_SAMPLES
     * @param pin Arduino analog pin
     * @return Averaged 10-bit ADC value
     */
    float readAveraged(int pin);

    /**
     * Convert raw ADC value to voltage
     * @param rawValue 10-bit ADC value (0-1023)
     * @return Voltage (0.0 - VREF)
     */
    float adcToVoltage(int rawValue);
};

#endif // PLOTRONIX_ADC_DRIVER_H
