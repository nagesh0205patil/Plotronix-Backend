#include "selector_driver.h"

void SelectorDriver::init() {
    pinMode(PIN_SELECTOR_SWITCH_0, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_1, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_2, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_3, INPUT);
}

uint8_t SelectorDriver::readSelector() const {
    bool b0 = !digitalRead(PIN_SELECTOR_SWITCH_0);
    bool b1 = !digitalRead(PIN_SELECTOR_SWITCH_1);
    bool b2 = !digitalRead(PIN_SELECTOR_SWITCH_2);
    bool b3 = !digitalRead(PIN_SELECTOR_SWITCH_3);

    return (uint8_t)(b0 ? 1u : 0u)
         | (uint8_t)(b1 ? 2u : 0u)
         | (uint8_t)(b2 ? 4u : 0u)
         | (uint8_t)(b3 ? 8u : 0u);
}
