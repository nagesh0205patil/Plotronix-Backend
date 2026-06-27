#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\serial_comm.cpp"
#include "serial_comm.h"
#include "config.h"

static bool           streaming = false;
static unsigned long  lastTx    = 0;

void processSerialCommands() {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "START") {
        streaming = true;
    } else if (cmd == "STOP") {
        streaming = false;
    }
}

bool isStreaming() {
    return streaming;
}

void sendSerialData(const ComponentInfo& comp, const SensorData& data) {
    if (!streaming)              return;
    if (comp.type == UNKNOWN)    return;

    unsigned long now = millis();
    if (now - lastTx < SERIAL_RATE) return;
    lastTx = now;

    Serial.print("DATA,");
    Serial.print(comp.tag);
    Serial.print(",");
    Serial.print(data.voltage, 3);
    Serial.print(",");
    Serial.println(data.current, 5);
}
