#include <unity.h>
#include "../../src/hardware/selector_state.h"

static const uint32_t STABILITY_MS = 25;

void test_first_toggle_is_applied_after_stability_period() {
    SelectorState state(STABILITY_MS);
    state.reset(0x00, 0);
    TEST_ASSERT_FALSE(state.update(0x01, 1));
    TEST_ASSERT_EQUAL_UINT8(0x00, state.getStableValue());
    TEST_ASSERT_TRUE(state.update(0x01, 26));
    TEST_ASSERT_EQUAL_UINT8(0x01, state.getStableValue());
}

void test_repeated_on_off_toggles_are_each_applied() {
    SelectorState state(STABILITY_MS);
    state.reset(0x00, 0);
    TEST_ASSERT_FALSE(state.update(0x01, 1));
    TEST_ASSERT_TRUE(state.update(0x01, 26));
    TEST_ASSERT_FALSE(state.update(0x00, 30));
    TEST_ASSERT_TRUE(state.update(0x00, 55));
    TEST_ASSERT_FALSE(state.update(0x01, 60));
    TEST_ASSERT_TRUE(state.update(0x01, 85));
    TEST_ASSERT_EQUAL_UINT8(0x01, state.getStableValue());
}

void test_rapid_toggles_keep_latest_input_as_source_of_truth() {
    SelectorState state(STABILITY_MS);
    state.reset(0x01, 0);
    TEST_ASSERT_FALSE(state.update(0x00, 1));
    TEST_ASSERT_FALSE(state.update(0x01, 5));
    TEST_ASSERT_FALSE(state.update(0x02, 10));
    TEST_ASSERT_FALSE(state.update(0x02, 34));
    TEST_ASSERT_TRUE(state.update(0x02, 35));
    TEST_ASSERT_EQUAL_UINT8(0x02, state.getStableValue());
}

void test_multiple_switches_are_published_as_one_consistent_value() {
    SelectorState state(STABILITY_MS);
    state.reset(0x01, 0);
    TEST_ASSERT_FALSE(state.update(0x03, 2));
    TEST_ASSERT_FALSE(state.update(0x07, 4));
    TEST_ASSERT_TRUE(state.update(0x07, 29));
    TEST_ASSERT_EQUAL_UINT8(0x07, state.getStableValue());
}

void test_slow_processing_still_applies_current_candidate() {
    SelectorState state(STABILITY_MS);
    state.reset(0x00, 0);
    TEST_ASSERT_FALSE(state.update(0x08, 5));
    TEST_ASSERT_TRUE(state.update(0x08, 500));
    TEST_ASSERT_EQUAL_UINT8(0x08, state.getStableValue());
}

void test_older_candidate_cannot_apply_after_newer_input() {
    SelectorState state(STABILITY_MS);
    state.reset(0x01, 0);
    TEST_ASSERT_FALSE(state.update(0x00, 1));
    const uint32_t oldSequence = state.getSequence();
    TEST_ASSERT_FALSE(state.update(0x02, 20));
    TEST_ASSERT_GREATER_THAN_UINT32(oldSequence, state.getSequence());
    TEST_ASSERT_FALSE(state.update(0x02, 44));
    TEST_ASSERT_TRUE(state.update(0x02, 45));
    TEST_ASSERT_EQUAL_UINT8(0x02, state.getStableValue());
}

void test_valid_selection_does_not_revert_for_transient_empty_value() {
    SelectorState state(STABILITY_MS);
    state.reset(0x05, 0);
    TEST_ASSERT_FALSE(state.update(0x00, 1));
    TEST_ASSERT_EQUAL_UINT8(0x05, state.getStableValue());
    TEST_ASSERT_FALSE(state.update(0x05, 10));
    TEST_ASSERT_EQUAL_UINT8(0x05, state.getStableValue());
}

void test_stable_empty_value_explicitly_clears_selection() {
    SelectorState state(STABILITY_MS);
    state.reset(0x05, 0);
    TEST_ASSERT_FALSE(state.update(0x00, 1));
    TEST_ASSERT_TRUE(state.update(0x00, 26));
    TEST_ASSERT_EQUAL_UINT8(0x00, state.getStableValue());
}

void test_invalid_transition_can_recover_on_retry() {
    SelectorState state(STABILITY_MS);
    state.reset(0x03, 0);
    TEST_ASSERT_FALSE(state.update(0x0F, 1));
    TEST_ASSERT_TRUE(state.update(0x0F, 26));
    TEST_ASSERT_FALSE(state.update(0x03, 30));
    TEST_ASSERT_TRUE(state.update(0x03, 55));
    TEST_ASSERT_EQUAL_UINT8(0x03, state.getStableValue());
}

int main(int, char**) {
    UNITY_BEGIN();
    RUN_TEST(test_first_toggle_is_applied_after_stability_period);
    RUN_TEST(test_repeated_on_off_toggles_are_each_applied);
    RUN_TEST(test_rapid_toggles_keep_latest_input_as_source_of_truth);
    RUN_TEST(test_multiple_switches_are_published_as_one_consistent_value);
    RUN_TEST(test_slow_processing_still_applies_current_candidate);
    RUN_TEST(test_older_candidate_cannot_apply_after_newer_input);
    RUN_TEST(test_valid_selection_does_not_revert_for_transient_empty_value);
    RUN_TEST(test_stable_empty_value_explicitly_clears_selection);
    RUN_TEST(test_invalid_transition_can_recover_on_retry);
    return UNITY_END();
}
