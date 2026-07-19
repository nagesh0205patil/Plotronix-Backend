#ifndef PLOTRONIX_SELECTOR_DRIVER_H
#define PLOTRONIX_SELECTOR_DRIVER_H

#include <Arduino.h>
#include "../data/config.h"
#include "selector_state.h"

/**
 * @class SelectorDriver
 * Reads four digital selector switches and encodes them into a 4-bit ID.
 */
class SelectorDriver {
public:
    void init();
    bool update();
    uint8_t readSelector() const;
    uint32_t getSequence() const;

private:
    SelectorState state{SELECTOR_STABILITY_TIME};

    uint8_t readRawSelector() const;
    void logRawChanges(uint8_t previous, uint8_t current, uint32_t sequence) const;
};

#endif // PLOTRONIX_SELECTOR_DRIVER_H
