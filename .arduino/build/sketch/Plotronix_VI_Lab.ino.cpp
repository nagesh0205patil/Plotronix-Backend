#include <Arduino.h>
#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\Plotronix_VI_Lab.ino"
#include "config.h"
#include "sensors.h"
#include "component.h"
#include "display.h"
#include "serial_comm.h"

static unsigned long startTime;

#line 9 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\Plotronix_VI_Lab.ino"
void setup();
#line 18 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\Plotronix_VI_Lab.ino"
void loop();
#line 9 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\Plotronix_VI_Lab.ino"
void setup() {
    Serial.begin(SERIAL_BAUD);

    initDisplay();
    showStartupScreen();

    startTime = millis();
}

void loop() {
    processSerialCommands();

    if (millis() - startTime < STARTUP_DELAY) {
        return;
    }

    SensorData    data = readSensors();
    ComponentInfo comp = detectComponent(data.selectorVoltage);

    updateDisplay(comp, data);
    sendSerialData(comp, data);
}

