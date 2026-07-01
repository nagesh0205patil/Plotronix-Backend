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
};

#endif // PLOTRONIX_COMPONENT_DETECTOR_H
