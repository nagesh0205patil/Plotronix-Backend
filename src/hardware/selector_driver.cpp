#include "selector_driver.h"

void SelectorDriver::init() {
    pinMode(PIN_SELECTOR_SWITCH_0, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_1, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_2, INPUT);
    pinMode(PIN_SELECTOR_SWITCH_3, INPUT);

    state.reset(readRawSelector(), millis());
}

bool SelectorDriver::update() {
    const uint8_t previousCandidate = state.getCandidateValue();
#if SELECTOR_DEBUG_LOGS
    const bool hadPendingCandidate = previousCandidate != state.getStableValue();
    const uint32_t previousSequence = state.getSequence();
#endif
    const uint8_t rawValue = readRawSelector();
    const bool changed = state.update(rawValue, millis());

    if (rawValue != previousCandidate) {
#if SELECTOR_DEBUG_LOGS
        if (hadPendingCandidate) {
            Serial.print(F("[SELECTOR] ignored stale seq="));
            Serial.println(previousSequence);
        }
#endif
        logRawChanges(previousCandidate, rawValue, state.getSequence());
    }

#if SELECTOR_DEBUG_LOGS
    if (changed) {
        Serial.print(F("[SELECTOR] applied seq="));
        Serial.print(state.getAppliedSequence());
        Serial.print(F(" value=0x"));
        Serial.println(state.getStableValue(), HEX);
    }
#endif

    return changed;
}

uint8_t SelectorDriver::readSelector() const {
    return state.getStableValue();
}

uint32_t SelectorDriver::getSequence() const {
    return state.getAppliedSequence();
}

uint8_t SelectorDriver::readRawSelector() const {
    bool b0 = !digitalRead(PIN_SELECTOR_SWITCH_0);
    bool b1 = !digitalRead(PIN_SELECTOR_SWITCH_1);
    bool b2 = !digitalRead(PIN_SELECTOR_SWITCH_2);
    bool b3 = !digitalRead(PIN_SELECTOR_SWITCH_3);

    return (uint8_t)(b0 ? 1u : 0u)
         | (uint8_t)(b1 ? 2u : 0u)
         | (uint8_t)(b2 ? 4u : 0u)
         | (uint8_t)(b3 ? 8u : 0u);
}

void SelectorDriver::logRawChanges(uint8_t previous, uint8_t current, uint32_t sequence) const {
#if SELECTOR_DEBUG_LOGS
    const uint8_t changedBits = previous ^ current;
    for (uint8_t switchId = 0; switchId < 4; ++switchId) {
        const uint8_t mask = static_cast<uint8_t>(1u << switchId);
        if ((changedBits & mask) == 0) continue;

        Serial.print(F("[SELECTOR] switch="));
        Serial.print(switchId);
        Serial.print(F(" value="));
        Serial.print((current & mask) != 0 ? 1 : 0);
        Serial.print(F(" seq="));
        Serial.println(sequence);
    }
#else
    (void)previous;
    (void)current;
    (void)sequence;
#endif
}
