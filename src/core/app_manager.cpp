#include "app_manager.h"

/**
 * Retrieve the global singleton instance of the AppManager.
 * Ensures only one instance controls the application lifecycle.
 */
AppManager& AppManager::getInstance() {
    static AppManager instance;
    return instance;
}

/**
 * Initialize the application by setting up all subsystems.
 */
void AppManager::init() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("\n\n[APP] PlotTronix VI Lab - Initialization Starting");
    initializeSubsystems();
    Serial.println("[APP] Initialization Complete - Entering main loop");
}

/**
 * Main loop handler called from Arduino loop().
 * Processes commands, checks startup timing, and executes measurements.
 */
void AppManager::update() {
    // Always process incoming serial commands
    serialManager.processIncomingCommands();

    // Run the measurement and display cycle continuously so the UI state machine
    // can advance through the welcome, initialization, waiting, and component screens.
    executeMeasurementCycle();
}

/**
 * Initialize all subsystems in the correct order.
 * Serial first for debug output, then display and ADC.
 */
void AppManager::initializeSubsystems() {
    // Initialize serial first (for debug output if needed)
    serialManager.init();

    // Initialize display and show splash screen
    display.init();

    // Initialize ADC (minimal initialization needed)
    adc.init();

    // Initialize selector driver pins
    selector.init();

    // Record startup time for any future startup-related logic.
    startupTime = millis();
}

/**
 * Execute a full measurement cycle: sample sensors, detect component,
 * update UI, and send serial data if streaming.
 */
void AppManager::executeMeasurementCycle() {
    // Read all sensor inputs
    SensorData sensorData = adc.readAllSensors();

    // Read selector switches and encode a 4-bit component ID
    sensorData.selectorId = selector.readSelector();

    // Detect component type from the binary selector ID
    ComponentInfo component = componentDetector.detect(sensorData.selectorId);

    // Update display with latest readings
    display.updateMeasurementDisplay(component, sensorData);

    // Send data to host if streaming is active
    serialManager.sendDataFrame(component, sensorData);
}
