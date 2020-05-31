// Change the product ID to avoid the Linux-specific VIA bug (if a keyboard
// with the supported VID:PID but a non-VIA firmware is connected, the app
// becomes very slow to respond).
#undef PRODUCT_ID
#define PRODUCT_ID      0x8080

// Measure the matrix scan rate and report it in the debug console.
#define DEBUG_MATRIX_SCAN_RATE

// Use the minimum possible I/O delay for the matrix to speed up scanning.
#define MATRIX_IO_DELAY 1

// Select the hold action for a dual-use key immediately when another key is
// pressed (this is different from PERMISSIVE_HOLD, which selects the hold
// action when another key is pressed and then released).
#define HOLD_ON_OTHER_KEY_PRESS
