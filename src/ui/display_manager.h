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
     * Update the active display based on startup timing and detected component state.
     *
     * @param comp Component being tested
     * @param data Latest sensor readings
     */
    void updateMeasurementDisplay(const ComponentInfo& comp, const SensorData& data);

private:
    enum ScreenState {
        SCREEN_WELCOME,
        SCREEN_INITIALIZING,
        SCREEN_WAITING,
        SCREEN_COMPONENT
    };

    DisplayDriver driver;                    ///< Low-level display driver
    unsigned long lastDisplayUpdate = 0;     ///< Timestamp of last display update
    unsigned long screenStartTime = 0;       ///< Timestamp when current screen started
    ScreenState currentScreen = SCREEN_WELCOME;
    ComponentType lastDetectedType = UNKNOWN;
    SensorData lastSensorData{};            ///< Last values shown on screen
    bool componentLayoutDrawn = false;      ///< True when static labels for component screen are drawn

    /**
     * Clear display and set default text styling
     */
    void clearAndReset();

    /**
     * Render the welcome screen.
     */
    void showWelcomeScreen();

    /**
     * Render the initialization screen.
     */
    void showInitializingScreen();

    /**
     * Render the waiting screen.
     */
    void showWaitingScreen();

    /**
     * Render the component information screen.
     */
    void showComponentScreen(const ComponentInfo& comp, const SensorData& data);
    void drawComponentLayout(const ComponentInfo& comp);
    void updateMeasurementValues(const SensorData& data);

};

#endif // PLOTRONIX_DISPLAY_MANAGER_H
