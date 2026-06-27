#include "component_detector.h"

/**
 * Detect the selected component based on the selector potentiometer voltage.
 * Each voltage range corresponds to a specific component position.
 */
ComponentInfo ComponentDetector::detect(float selectorVoltage) {
    // Map selector voltage ranges to component types
    // Each voltage band corresponds to a position on the rotary selector

    if      (selectorVoltage < 0.5)  return createInfo(DIODE,   "DIODE",   "Diode");
    else if (selectorVoltage < 1.0)  return createInfo(ZENER,   "ZENER",   "Zener Diode");
    else if (selectorVoltage < 1.5)  return createInfo(SCR,     "SCR",     "SCR");
    else if (selectorVoltage < 2.0)  return createInfo(BJT_CB,  "BJT_CB",  "BJT-CB");
    else if (selectorVoltage < 2.5)  return createInfo(BJT_CE,  "BJT_CE",  "BJT-CE");
    else if (selectorVoltage < 3.0)  return createInfo(BJT_CC,  "BJT_CC",  "BJT-CC");
    else if (selectorVoltage < 3.3)  return createInfo(FET,     "FET",     "FET");
    else if (selectorVoltage < 3.6)  return createInfo(MOSFET,  "MOSFET",  "MOSFET");
    else if (selectorVoltage < 4.0)  return createInfo(IGBT,    "IGBT",    "IGBT");
    else if (selectorVoltage < 4.3)  return createInfo(UJT,     "UJT",     "UJT");
    else if (selectorVoltage < 4.6)  return createInfo(TRIAC,   "TRIAC",   "TRIAC");
    else if (selectorVoltage <= 5.1) return createInfo(DIAC,    "DIAC",    "DIAC");
    else                             return createInfo(UNKNOWN,  "UNKNOWN", "?");
}

/**
 * Build a ComponentInfo object with the supplied metadata.
 * @param type Component type enum.
 * @param tag Protocol tag for host communication.
 * @param name Human-readable component label.
 */
ComponentInfo ComponentDetector::createInfo(ComponentType type, const char* tag, const char* name) {
    ComponentInfo info;
    info.type = type;
    info.tag  = tag;
    info.name = name;
    return info;
}
