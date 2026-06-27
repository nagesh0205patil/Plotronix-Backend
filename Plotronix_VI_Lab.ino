/* ============================================================================
 * PLOTRONIX VI CHARACTERISTICS LABORATORY
 * Modular Application Refactored with MVC Architecture
 * 
 * Project Structure:
 *   /src/core             - Application manager and orchestration
 *   /src/hardware         - Hardware drivers (ADC, Display)
 *   /src/analysis         - Business logic (Component detection)
 *   /src/communication    - Serial protocol and management
 *   /src/ui               - Display/presentation layer
 *   /src/data             - Data models and configuration
 * 
 * Architecture: Model-View-Controller
 *   Model:       SensorData, ComponentInfo
 *   View:        DisplayManager
 *   Controller:  AppManager
 * ============================================================================ */

#include "src/core/app_manager.h"

/**
 * Arduino setup() - Initialization phase
 * Initializes all application subsystems
 */
void setup() {
  AppManager::getInstance().init();
}

/**
 * Arduino loop() - Main event loop
 * Executes one cycle of: read sensors → detect component → update display → send data
 */
void loop() {
  AppManager::getInstance().update();
}
