#ifndef PLOTRONIX_APP_MANAGER_H
#define PLOTRONIX_APP_MANAGER_H

#include <Arduino.h>
#include "../hardware/adc_driver.h"
#include "../hardware/display_driver.h"
#include "../ui/display_manager.h"
#include "../analysis/component_detector.h"
#include "../communication/serial_manager.h"
#include "../data/sensor_data.h"
#include "../data/component_info.h"
#include "../data/config.h"

/* ============================================================================
 * APPLICATION MANAGER
 * Main controller orchestrating all subsystems (MVC Controller)
 * ============================================================================ */

/**
 * @class AppManager
 * Orchestrates the entire application lifecycle and system integration
 * 
 * Responsibilities:
 *   - Initialize all subsystems (hardware, UI, communication)
 *   - Manage main event loop
 *   - Coordinate data flow between modules
 *   - Handle timing and state management
 */
class AppManager {
public:
    /**
     * Singleton accessor - get the global app instance
     */
    static AppManager& getInstance();

    /**
     * Initialize all subsystems
     * Call once during Arduino setup()
     */
    void init();

    /**
     * Main application loop iteration
     * Call once per Arduino loop()
     */
    void update();

    /**
     * Check if currently in measurement phase (post-startup)
     */
    bool isReadyForMeasurement() const;

private:
    // Subsystem modules
    ADCDriver adc;                       ///< ADC input abstraction
    DisplayManager display;              ///< Display UI management
    ComponentDetector componentDetector;  ///< Component identification
    SerialManager serialManager;         ///< Host communication

    // Timing state
    unsigned long startupTime = 0;       ///< Timestamp when app initialized

    // Prevent instantiation (singleton pattern)
    AppManager() = default;
    AppManager(const AppManager&) = delete;
    AppManager& operator=(const AppManager&) = delete;

    /**
     * Initialize all subsystems
     */
    void initializeSubsystems();

    /**
     * Execute measurement and update cycle
     */
    void executeMeasurementCycle();
};

#endif // PLOTRONIX_APP_MANAGER_H
