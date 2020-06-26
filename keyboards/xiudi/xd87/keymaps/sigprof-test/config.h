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
#define HOLD_ON_OTHER_KEY_PRESS

// Increase the number of RGB LEDs to test the behavior with a really long
// chain of WS2812 LEDs.  For 105 LEDs (imagine a fullsize ISO keyboard with
// per-key RGB) sending of the bitstream would take 3.15 ms.
#undef RGBLED_NUM
#define RGBLED_NUM 105
