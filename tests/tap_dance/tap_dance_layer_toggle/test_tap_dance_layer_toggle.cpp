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

class TapDanceLayerToggle : public TestFixture {};

TEST_F(TapDanceLayerToggle, SingleTap) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_l_togg.press();
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);

    // We get the key press and the release for the tap dance on timeout.
    idle_for(TAPPING_TERM);
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);

    // The normal key still behaves like it should on layer 0.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, SingleTapCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_l_togg.press();
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);

    // We get the key press and the release for the tap dance on timeout.
    idle_for(TAPPING_TERM);
    EXPECT_REPORT(driver, (KC_ESC));
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

TEST_F(TapDanceLayerToggle, SingleTapFast) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_l_togg.press();
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, SingleTapFastCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first tap.
    key_l_togg.press();
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);
    EXPECT_REPORT(driver, (KC_X));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, SingleTapRolling) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first press.
    key_l_togg.press();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_REPORT(driver, (KC_ESC, KC_A));
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_REPORT(driver, (KC_A));
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, SingleTapRollingCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // The tap dance key does nothing immediately on the first press.
    key_l_togg.press();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();

    // A quick press of the next key causes a tap for the tap dance to be sent
    // before the press event for the next key.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_REPORT(driver, (KC_ESC, KC_X));
    EXPECT_REPORT(driver, (KC_ESC));
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);
    key_normal.release();
    EXPECT_REPORT(driver, (KC_Y));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTap) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Double tap does not send anything.
    tap_key(key_l_togg);
    tap_key(key_l_togg);
    EXPECT_NO_REPORT(driver);

    // Wait for the tapping term to expire.
    idle_for(TAPPING_TERM);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_B));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTapCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Double tap does not send anything.
    tap_key(key_l_togg);
    tap_key(key_l_togg);
    EXPECT_NO_REPORT(driver);

    // Wait for the tapping term to expire.
    idle_for(TAPPING_TERM);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_Z));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_W));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTapFast) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Double tap does not send anything.
    tap_key(key_l_togg);
    tap_key(key_l_togg);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1 even if pressed
    // quickly.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_B));
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTapFastCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Double tap does not send anything.
    tap_key(key_l_togg);
    tap_key(key_l_togg);
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1 even if pressed
    // quickly.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_Z));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_W));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTapRolling) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, KC_A};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, KC_B};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Tap-then-hold does not send anything.
    tap_key(key_l_togg);
    key_l_togg.press();
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1, even when the tap
    // dance key is released in the middle.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_B));
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}

TEST_F(TapDanceLayerToggle, DoubleTapRollingCustom) {
    TestDriver driver;
    InSequence s;
    auto       key_l_togg    = KeymapKey{0, 1, 0, TD(TD_L_TOGG)};
    auto       key_normal    = KeymapKey{0, 2, 0, CUST_XY};
    auto       key_l_togg_l1 = KeymapKey{1, 1, 0, KC_TRNS};
    auto       key_normal_l1 = KeymapKey{1, 2, 0, CUST_ZW};

    set_keymap({key_l_togg, key_normal, key_l_togg_l1, key_normal_l1});

    // Tap-then-hold does not send anything.
    tap_key(key_l_togg);
    key_l_togg.press();
    run_one_scan_loop();
    EXPECT_NO_REPORT(driver);

    // The normal key behaves like it should on layer 1, even when the tap
    // dance key is released in the middle.
    key_normal.press();
    EXPECT_REPORT(driver, (KC_Z));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
    key_l_togg.release();
    EXPECT_NO_REPORT(driver);
    run_one_scan_loop();
    key_normal.release();
    EXPECT_REPORT(driver, (KC_W));
    EXPECT_EMPTY_REPORT(driver);
    run_one_scan_loop();
}
