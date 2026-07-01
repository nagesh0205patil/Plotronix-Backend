#include "component_detector.h"

/**
 * Detect the selected component based on the digital selector switches.
 * Uses a direct 4-bit lookup so each switch combination maps to a known type.
 */
ComponentInfo ComponentDetector::detect(uint8_t selectorId) {
    switch (selectorId & 0x0F) {
        case 0x01: return createInfo(DIODE, "DIODE", "Diode");
        case 0x02: return createInfo(ZENER, "ZENER", "Zener Diode");
        case 0x03: return createInfo(SCR, "SCR", "SCR");
        case 0x04: return createInfo(BJT_CB, "BJT_CB", "BJT-CB");
        case 0x05: return createInfo(BJT_CE, "BJT_CE", "BJT-CE");
        case 0x06: return createInfo(BJT_CC, "BJT_CC", "BJT-CC");
        case 0x07: return createInfo(FET, "FET", "FET");
        case 0x08: return createInfo(MOSFET, "MOSFET", "MOSFET");
        case 0x09: return createInfo(IGBT, "IGBT", "IGBT");
        case 0x0A: return createInfo(UJT, "UJT", "UJT");
        case 0x0B: return createInfo(TRIAC, "TRIAC", "TRIAC");
        case 0x0C: return createInfo(DIAC, "DIAC", "DIAC");
        default:   return createInfo(UNKNOWN, "UNKNOWN", "?");
    }
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
