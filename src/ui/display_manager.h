#ifndef PLOTRONIX_DISPLAY_MANAGER_H
#define PLOTRONIX_DISPLAY_MANAGER_H

#include <Arduino.h>
#include "../hardware/display_driver.h"
#include "../data/component_info.h"
#include "../data/sensor_data.h"
#include "../data/config.h"

/* ============================================================================
 * DISPLAY MANAGER
 * Presentation layer - manages what is shown on the TFT display
 * ============================================================================ */

/**
 * @class DisplayManager
 * Controls UI rendering on TFT display
 * Abstracts the DisplayDriver and provides high-level display operations
 */
class DisplayManager {
public:
    /**
     * Initialize display and show startup screen
     */
    void init();

    /**
     * Display startup/splash screen
     * Shows project information and initialization message
     */
    void showStartupScreen();

    /**
     * Update main measurement display with current data
     * Shows component name, voltage, and current readings
     * Rate-limited by DISPLAY_REFRESH configuration
     * 
     * @param comp Component being tested
     * @param data Latest sensor readings
     */
    void updateMeasurementDisplay(const ComponentInfo& comp, const SensorData& data);

private:
    DisplayDriver driver;                    ///< Low-level display driver
    unsigned long lastDisplayUpdate = 0;     ///< Timestamp of last display update

    /**
     * Clear display and set default text styling
     */
    void clearAndReset();
};

#endif // PLOTRONIX_DISPLAY_MANAGER_H
