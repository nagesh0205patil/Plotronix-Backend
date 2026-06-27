#ifndef PLOTRONIX_COMPONENT_DETECTOR_H
#define PLOTRONIX_COMPONENT_DETECTOR_H

#include <Arduino.h>
#include "../data/component_info.h"
#include "../data/sensor_data.h"

/* ============================================================================
 * COMPONENT DETECTOR
 * Business logic for identifying component type based on selector voltage
 * ============================================================================ */

/**
 * @class ComponentDetector
 * Analyzes selector voltage to identify which component is selected
 */
class ComponentDetector {
public:
    /**
     * Detect component type from selector voltage
     * Uses voltage thresholds to map selector position to component type
     * 
     * @param selectorVoltage Voltage from selector potentiometer (0.0 - VREF)
     * @return ComponentInfo structure with type, tag, and name
     */
    ComponentInfo detect(float selectorVoltage);

private:
    /**
     * Create a ComponentInfo structure for a specific type
     * Helper method to encapsulate type metadata
     */
    ComponentInfo createInfo(ComponentType type, const char* tag, const char* name);
};

#endif // PLOTRONIX_COMPONENT_DETECTOR_H
