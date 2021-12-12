// Measure the matrix scan rate and report it in the debug console.
#define DEBUG_MATRIX_SCAN_RATE

// Use the minimum possible I/O delay for the matrix to speed up scanning.
#define MATRIX_IO_DELAY 1

// Set the USB polling rate to 1000 Hz to reduce the latency to the minimum.
#define USB_POLLING_INTERVAL_MS 1

// Make the MT() behavior identical to LT() and other dual-use keys (this does
// not really change the behavior when HOLD_ON_OTHER_KEY_PRESS is enabled, but
// removes some extra code).
#define IGNORE_MOD_TAP_INTERRUPT

// Select the hold action for a dual-use key immediately when another key is
// pressed (this is different from PERMISSIVE_HOLD, which selects the hold
// action when another key is pressed and then released).
//
// Use the “per key” variant of this setting, because it is appropriate only
// for keys which are not used for their tap action in the middle of text
// (otherwise it would be too easy to trigger the hold action by mistake,
// because during fast typing the next key is often pressed before the previous
// one is released).
//
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Select the hold action for a dual-role key when another key is pressed and
// then released while still holding the dual-role key pressed.
//
// Use the “per key” variant of this setting, because this behavior is needed
// only for some keys.
//
#define PERMISSIVE_HOLD_PER_KEY

// Increase the backlight PWM frequency by decreasing the resolution.  The
// default value (0xFFFFU) together with the 16 MHz clock results in ≈244 Hz
// PWM frequency, which is very noticeable.  However, another concern with XD87
// is that PWM for the backlight is implemented by toggling the pin in software
// (due to bad hardware design not using a PWM-capable pin for that purpose),
// therefore the PWM frequency should not be set too high.  Use 2000 Hz as a
// compromise.
#define BACKLIGHT_CUSTOM_RESOLUTION (F_CPU / 2000)

// Increase the number of RGB LEDs to test the behavior with a really long
// chain of WS2812 LEDs.  For 105 LEDs (imagine a fullsize ISO keyboard with
// per-key RGB) sending of the bitstream would take 3.15 ms.
#undef RGBLED_NUM
#define RGBLED_NUM 105
