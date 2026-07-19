#ifndef PLOTRONIX_SELECTOR_STATE_H
#define PLOTRONIX_SELECTOR_STATE_H

#include <stdint.h>

/**
 * Tracks the latest raw selector value and publishes it only after it remains
 * stable. A newer raw value increments the sequence and invalidates the older
 * pending value, so an earlier transition can never be applied later.
 */
class SelectorState {
public:
    explicit SelectorState(uint32_t stabilityTimeMs)
        : stabilityTimeMs(stabilityTimeMs) {}

    void reset(uint8_t value, uint32_t now) {
        stableValue = value & 0x0F;
        candidateValue = stableValue;
        candidateSince = now;
        sequence = 0;
        appliedSequence = 0;
        initialized = true;
    }

    bool update(uint8_t value, uint32_t now) {
        value &= 0x0F;

        if (!initialized) {
            reset(value, now);
            return false;
        }

        if (value != candidateValue) {
            candidateValue = value;
            candidateSince = now;
            ++sequence;
            return false;
        }

        if (candidateValue != stableValue &&
            static_cast<uint32_t>(now - candidateSince) >= stabilityTimeMs) {
            stableValue = candidateValue;
            appliedSequence = sequence;
            return true;
        }

        return false;
    }

    uint8_t getStableValue() const { return stableValue; }
    uint8_t getCandidateValue() const { return candidateValue; }
    uint32_t getSequence() const { return sequence; }
    uint32_t getAppliedSequence() const { return appliedSequence; }

private:
    const uint32_t stabilityTimeMs;
    uint8_t stableValue = 0;
    uint8_t candidateValue = 0;
    uint32_t candidateSince = 0;
    uint32_t sequence = 0;
    uint32_t appliedSequence = 0;
    bool initialized = false;
};

#endif // PLOTRONIX_SELECTOR_STATE_H
