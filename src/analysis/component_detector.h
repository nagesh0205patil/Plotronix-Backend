#ifndef PLOTRONIX_COMPONENT_DETECTOR_H
#define PLOTRONIX_COMPONENT_DETECTOR_H

#include <Arduino.h>
#include "../data/component_info.h"
#include "../data/sensor_data.h"

/* ============================================================================
 * COMPONENT DETECTOR
 * Business logic for identifying component type based on selector switches
 * ============================================================================ */

/**
 * @class ComponentDetector
 * Analyzes digital selector switches to identify which component is selected
 */
class ComponentDetector {
public:
    /**
* Detect component type from a 4-bit selector ID.
 * Selector is read from digital switches to form a binary component code.
 * 
 * @param selectorId 4-bit component selector ID (0-15)
 * @return ComponentInfo structure with type, tag, and name
 */
 ComponentInfo detect(uint8_t selectorId);

private:
    /**
     * Create a ComponentInfo structure for a specific type
     * Helper method to encapsulate type metadata
     */
    ComponentInfo createInfo(ComponentType type, const char* tag, const char* name);
};

#endif // PLOTRONIX_COMPONENT_DETECTOR_H
