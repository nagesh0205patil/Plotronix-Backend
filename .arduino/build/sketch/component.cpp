#line 1 "F:\\CloudTronix\\PlotTronix\\Plotronix_VI_Lab\\component.cpp"
#include "component.h"

ComponentInfo detectComponent(float selectorVoltage) {

    if      (selectorVoltage < 0.5)  return { DIODE,   "DIODE",   "Diode"       };
    else if (selectorVoltage < 1.0)  return { ZENER,   "ZENER",   "Zener Diode" };
    else if (selectorVoltage < 1.5)  return { SCR,     "SCR",     "SCR"         };
    else if (selectorVoltage < 2.0)  return { BJT_CB,  "BJT_CB",  "BJT-CB"      };
    else if (selectorVoltage < 2.5)  return { BJT_CE,  "BJT_CE",  "BJT-CE"      };
    else if (selectorVoltage < 3.0)  return { BJT_CC,  "BJT_CC",  "BJT-CC"      };
    else if (selectorVoltage < 3.3)  return { FET,     "FET",     "FET"         };
    else if (selectorVoltage < 3.6)  return { MOSFET,  "MOSFET",  "MOSFET"      };
    else if (selectorVoltage < 4.0)  return { IGBT,    "IGBT",    "IGBT"        };
    else if (selectorVoltage < 4.3)  return { UJT,     "UJT",     "UJT"         };
    else if (selectorVoltage < 4.6)  return { TRIAC,   "TRIAC",   "TRIAC"       };
    else if (selectorVoltage <= 5.1) return { DIAC,    "DIAC",    "DIAC"        };
    else                             return { UNKNOWN,  "UNKNOWN", "?"           };
}
