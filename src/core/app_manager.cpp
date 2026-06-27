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

    // Check if we've passed the startup phase
    if (!isReadyForMeasurement()) {
        return;
    }

    // Execute measurement and display cycle
    executeMeasurementCycle();
}

/**
 * Determine whether the startup delay has passed and the system is ready.
 * @return true if the startup splash period is complete.
 */
bool AppManager::isReadyForMeasurement() const {
    return (millis() - startupTime) >= STARTUP_DELAY;
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

    // Record startup time for splash screen duration
    startupTime = millis();
}

/**
 * Execute a full measurement cycle: sample sensors, detect component,
 * update UI, and send serial data if streaming.
 */
void AppManager::executeMeasurementCycle() {
    // Read all sensor inputs
    SensorData sensorData = adc.readAllSensors();

    // Detect component type
    ComponentInfo component = componentDetector.detect(sensorData.selectorVoltage);

    // Update display with latest readings
    display.updateMeasurementDisplay(component, sensorData);

    // Send data to host if streaming is active
    serialManager.sendDataFrame(component, sensorData);
}
