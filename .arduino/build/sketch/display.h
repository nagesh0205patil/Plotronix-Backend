#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\display.h"
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "component.h"
#include "sensors.h"

void initDisplay();
void showStartupScreen();
void updateDisplay(ComponentInfo comp, SensorData data);

#endif
