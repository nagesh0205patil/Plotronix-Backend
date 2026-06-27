#include "serial_manager.h"
#include "serial_protocol.h"

/**
 * Initialize the serial port for host communication.
 */
void SerialManager::init() {
    Serial.begin(SERIAL_BAUD);
}

/**
 * Poll the serial buffer for incoming commands and execute them.
 * Supports START and STOP commands.
 */
void SerialManager::processIncomingCommands() {
    if (!Serial.available()) return;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    executeCommand(cmd);
}

/**
 * Send a comma-separated data frame to the host if streaming is active.
 * Data format: DATA,<TAG>,<VOLTAGE>,<CURRENT>.
 */
void SerialManager::sendDataFrame(const ComponentInfo& comp, const SensorData& data) {
    // Don't send if not streaming
    if (!streaming) return;

    // Don't send if component is unknown
    if (comp.type == UNKNOWN) return;

    // Rate limit transmission
    unsigned long now = millis();
    if (now - lastFrameTime < SERIAL_RATE) return;
    lastFrameTime = now;

    // Format: DATA,<TAG>,<VOLTAGE>,<CURRENT>
    Serial.print(FRAME_PREFIX);
    Serial.print(",");
    Serial.print(comp.tag);
    Serial.print(",");
    Serial.print(data.voltage, 3);
    Serial.print(",");
    Serial.println(data.current, 5);
}

/**
 * Check whether the serial manager is currently in streaming mode.
 * @return true if streaming output is enabled.
 */
bool SerialManager::isStreaming() const {
    return streaming;
}

/**
 * Execute a parsed serial command string.
 * Recognizes START and STOP commands.
 */
void SerialManager::executeCommand(const String& cmd) {
    if (cmd == CMD_START) {
        streaming = true;
    } else if (cmd == CMD_STOP) {
        streaming = false;
    }
    // Silently ignore unknown commands
}
