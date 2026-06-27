#ifndef PLOTRONIX_SERIAL_MANAGER_H
#define PLOTRONIX_SERIAL_MANAGER_H

#include <Arduino.h>
#include "../data/component_info.h"
#include "../data/sensor_data.h"
#include "../data/config.h"

/* ============================================================================
 * SERIAL MANAGER
 * Manages serial communication with host GUI application
 * ============================================================================ */

/**
 * @class SerialManager
 * Handles bidirectional serial communication with the GUI host
 * Manages streaming state and enforces protocol contract
 */
class SerialManager {
public:
    /**
     * Initialize serial communication
     * Sets up UART at SERIAL_BAUD
     */
    void init();

    /**
     * Process incoming commands from host (START/STOP)
     * Must be called regularly in main loop
     */
    void processIncomingCommands();

    /**
     * Send measurement data frame to host if streaming is active
     * Rate-limited by SERIAL_RATE configuration
     * 
     * @param comp Component information
     * @param data Current sensor measurements
     */
    void sendDataFrame(const ComponentInfo& comp, const SensorData& data);

    /**
     * Check if currently streaming data
     * @return true if START received and STOP not received
     */
    bool isStreaming() const;

private:
    bool streaming = false;              ///< Current streaming state
    unsigned long lastFrameTime = 0;     ///< Timestamp of last sent frame

    /**
     * Parse and execute incoming serial command
     * @param cmd Command string (trimmed)
     */
    void executeCommand(const String& cmd);
};

#endif // PLOTRONIX_SERIAL_MANAGER_H
