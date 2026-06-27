#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\serial_comm.h"
#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <Arduino.h>
#include "component.h"
#include "sensors.h"

// Call each loop iteration to handle incoming START / STOP commands from GUI
void processSerialCommands();

// Returns true while a START command has been received and STOP has not
bool isStreaming();

// Sends one DATA frame if streaming is active; rate-limited by SERIAL_RATE
void sendSerialData(const ComponentInfo& comp, const SensorData& data);

#endif
