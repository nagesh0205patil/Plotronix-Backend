#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\component.h"
#ifndef COMPONENT_H
#define COMPONENT_H

#include <Arduino.h>

enum ComponentType {
    DIODE,
    ZENER,
    SCR,
    BJT_CB,
    BJT_CE,
    BJT_CC,
    FET,
    MOSFET,
    IGBT,
    UJT,
    TRIAC,
    DIAC,
    UNKNOWN
};

struct ComponentInfo {
    ComponentType type;
    const char* tag;    // protocol tag matched by GUI router (e.g. "DIODE", "BJT_CB")
    const char* name;   // human-readable label for display
};

ComponentInfo detectComponent(float voltage);

#endif
