// Use the minimum possible I/O delay for the matrix to speed up scanning.
#define MATRIX_IO_DELAY 1

// Set the USB polling rate to 1000 Hz to reduce the latency to the minimum.
#define USB_POLLING_INTERVAL_MS 1

// Enable NKRO during keyboard startup, so that adding the “Toggle NKRO” key to
// the keymap won't be needed.
#define FORCE_NKRO

// Select the hold action for a dual-role key immediately when another key is
// pressed and then released while the dual-role key is held, even if that
// happens before the tapping term expires.  This mode works better for fast
// Layer Tap usage, while still keeping any “rolling” presses working normally.
#define PERMISSIVE_HOLD
