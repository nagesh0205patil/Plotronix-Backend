#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\sensors.cpp"
#include "sensors.h"
#include "config.h"

static float readAveraged(int pin) {
    long sum = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += analogRead(pin);
    }

    float avg = sum / (float)NUM_SAMPLES;
    return (avg / ADC_RESOLUTION) * VREF;
}

SensorData readSensors() {
    SensorData data;

    data.voltage         = readAveraged(PIN_VOLTAGE);
    data.current         = readAveraged(PIN_CURRENT) / SHUNT_RESISTOR;
    data.selectorVoltage = readAveraged(PIN_SELECTOR);

    return data;
}
