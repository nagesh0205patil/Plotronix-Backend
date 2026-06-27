#include "adc_driver.h"

/**
 * Initialize ADC driver.
 * Arduino analogRead() does not require explicit pinMode configuration.
 */
void ADCDriver::init() {
    // No explicit initialization needed for Arduino ADC
    // pinMode calls are not necessary for analogRead()
}

/**
 * Read all configured sensors and return calibrated measurements.
 * Combines voltage, current, and selector voltage into a SensorData packet.
 */
SensorData ADCDriver::readAllSensors() {
    SensorData data;

    data.voltage         = readRawVoltage(PIN_VOLTAGE);
    data.selectorVoltage = readRawVoltage(PIN_SELECTOR);

    // Current is derived from voltage across shunt resistor: I = V / R
    float shuntVoltage   = readRawVoltage(PIN_CURRENT);
    data.current         = shuntVoltage / SHUNT_RESISTOR;

    return data;
}

/**
 * Read a raw analog pin and convert the averaged ADC result to voltage.
 * @param pin Analog pin to sample.
 * @return Calculated voltage in volts.
 */
float ADCDriver::readRawVoltage(int pin) {
    float avgValue = readAveraged(pin);
    return adcToVoltage(avgValue);
}

/**
 * Read and average the specified ADC pin over NUM_SAMPLES samples.
 * @param pin Analog pin to sample.
 * @return Averaged raw ADC value.
 */
float ADCDriver::readAveraged(int pin) {
    long sum = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += analogRead(pin);
    }

    return sum / (float)NUM_SAMPLES;
}

/**
 * Translate a raw ADC reading into a voltage using the reference voltage.
 * @param rawValue Raw 10-bit ADC reading.
 * @return Converted voltage value.
 */
float ADCDriver::adcToVoltage(int rawValue) {
    return (rawValue / ADC_RESOLUTION) * VREF;
}
