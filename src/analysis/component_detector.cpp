#include "component_detector.h"

/**
 * Detect the selected component based on the digital selector switches.
 * Uses a direct 4-bit lookup so each switch combination maps to a known type.
 */
ComponentInfo ComponentDetector::detect(uint8_t selectorId) {
    switch (selectorId & 0x0F) {
        case 0x01: return { DIODE,  "DIODE",  "Diode" };
        case 0x02: return { ZENER,  "ZENER",  "Zener Diode" };
        case 0x03: return { SCR,    "SCR",    "SCR" };
        case 0x04: return { BJT_CB, "BJT_CB", "BJT-CB" };
        case 0x05: return { BJT_CE, "BJT_CE", "BJT-CE" };
        case 0x06: return { BJT_CC, "BJT_CC", "BJT-CC" };
        case 0x07: return { FET,    "FET",    "FET" };
        case 0x08: return { MOSFET, "MOSFET", "MOSFET" };
        case 0x09: return { IGBT,   "IGBT",   "IGBT" };
        case 0x0A: return { UJT,    "UJT",    "UJT" };
        case 0x0B: return { TRIAC,  "TRIAC",  "TRIAC" };
        case 0x0C: return { DIAC,   "DIAC",   "DIAC" };
        default:   return { UNKNOWN, "UNKNOWN", "?" };
    }
}
