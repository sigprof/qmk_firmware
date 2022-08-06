// Copyright 2022 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keyboard_report_util.hpp"
#include "keycode.h"
#include "test_common.hpp"
#include "action_tapping.h"
#include "test_keymap_key.hpp"
#include "tap_dance_defs.h"

using testing::_;
using testing::InSequence;

class TapDanceCustomLT : public TestFixture {};

TEST_F(TapDanceCustomLT, SingleTap) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_cust_lt.press();
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);

    // We get the key press and the release for the tap dance on timeout.
    idle_for(TAPPING_TERM);
    EXPECT_REPORT(driver, (KC_APP));
    EXPECT_EMPTY_REPORT(driver);

    // The normal key still behaves like it should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleTapCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_cust_lt.press();
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);

    // We get the key press and the release for the tap dance on timeout.
    idle_for(TAPPING_TERM);
    EXPECT_REPORT(driver, (KC_APP));
    EXPECT_EMPTY_REPORT(driver);

    // The normal key still behaves like it should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_X));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleTapFast) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_cust_lt.press();
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_APP));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleTapFastCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_cust_lt.press();
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_APP));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_X));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleHold) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first press.
    key_cust_lt.press();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // On timeout the layer switch happens, but no report is sent.
    idle_for(TAPPING_TERM);
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    EXPECT_TRUE(layer_state_is(1));

    // The normal key behaves as it should on layer 1.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_B));
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleHoldRolling) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first press.
    key_cust_lt.press();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes the layer switch to happen.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_B));
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, SingleHoldRollingCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first press.
    key_cust_lt.press();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes the layer switch to happen.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_Z));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_W));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleTap) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Double tap does not send anything immediately.
    tap_key(key_cust_lt);
    tap_key(key_cust_lt);
    EXPECT_NO_REPORT(driver);

    // Wait for the tapping term to expire.
    idle_for(TAPPING_TERM - 1);
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();

    // The normal key behaves like it should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleTapCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Double tap does not send anything immediately.
    tap_key(key_cust_lt);
    tap_key(key_cust_lt);
    EXPECT_NO_REPORT(driver);

    // Wait for the tapping term to expire.
    idle_for(TAPPING_TERM - 1);
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();

    // The normal key behaves like it should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_X));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleTapFast) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Double tap does not send anything immediately.
    tap_key(key_cust_lt);
    tap_key(key_cust_lt);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 0 even if pressed
    // quickly.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleTapFastCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Double tap does not send anything immediately.
    tap_key(key_cust_lt);
    tap_key(key_cust_lt);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 0 even if pressed
    // quickly.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_X));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleHoldRolling) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, KC_A};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Tap-then-hold does not send anything immediately.
    tap_key(key_cust_lt);
    key_cust_lt.press();
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);

    // The tap dance sends a press of KC_RCTL; the normal key behaves like it
    // should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_REPORT(driver, (KC_RCTL, KC_A));
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceCustomLT, DoubleHoldRollingCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_cust_lt    = KeymapKey{0, 1, 0, TD(TD_CUST_LT)};
    auto       key_normal     = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_cust_lt_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1  = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_cust_lt, key_normal, key_cust_lt_l1, key_normal_l1});

    // Tap-then-hold does not send anything immediately.
    tap_key(key_cust_lt);
    key_cust_lt.press();
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);

    // The tap dance sends a press of KC_RCTL; the normal key behaves like it
    // should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_RCTL));
    EXPECT_REPORT(driver, (KC_RCTL, KC_X));
    EXPECT_REPORT(driver, (KC_RCTL));
    run_one_scan_loop();
    key_cust_lt.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}
