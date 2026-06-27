#ifndef PLOTRONIX_COMPONENT_INFO_H
#define PLOTRONIX_COMPONENT_INFO_H

#include <Arduino.h>

/* ============================================================================
 * COMPONENT INFORMATION STRUCTURES
 * Data models for component type identification and metadata
 * ============================================================================ */

/**
 * @enum ComponentType
 * Enumeration of supported component types in the VI Lab
 */
enum ComponentType {
    DIODE,      ///< Standard diode
    ZENER,      ///< Zener diode
    SCR,        ///< Silicon Controlled Rectifier
    BJT_CB,     ///< BJT Common Base configuration
    BJT_CE,     ///< BJT Common Emitter configuration
    BJT_CC,     ///< BJT Common Collector configuration
    FET,        ///< Field Effect Transistor
    MOSFET,     ///< Metal Oxide Semiconductor FET
    IGBT,       ///< Insulated Gate Bipolar Transistor
    UJT,        ///< Unijunction Transistor
    TRIAC,      ///< Triode for AC
    DIAC,       ///< Diode for AC
    UNKNOWN     ///< Component type not detected
};

/**
 * @struct ComponentInfo
 * Contains complete information about a detected component
 */
struct ComponentInfo {
    ComponentType type;     ///< Component type enum
    const char* tag;        ///< Protocol tag for GUI router (e.g., "DIODE", "BJT_CB")
    const char* name;       ///< Human-readable display name
};

#endif // PLOTRONIX_COMPONENT_INFO_H
