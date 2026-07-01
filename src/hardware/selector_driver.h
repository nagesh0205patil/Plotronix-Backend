#ifndef PLOTRONIX_SELECTOR_DRIVER_H
#define PLOTRONIX_SELECTOR_DRIVER_H

#include <Arduino.h>
#include "../data/config.h"

/**
 * @class SelectorDriver
 * Reads four digital selector switches and encodes them into a 4-bit ID.
 */
class SelectorDriver {
public:
    void init();
    uint8_t readSelector() const;
};

#endif // PLOTRONIX_SELECTOR_DRIVER_H
