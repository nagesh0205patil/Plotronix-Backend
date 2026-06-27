#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\sensors.h"
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

struct SensorData {
    float voltage;
    float current;
    float selectorVoltage;
};

SensorData readSensors();

#endif
