#include QMK_KEYBOARD_H

#include <avr/boot.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {LAYOUT_ortho_1x1(KC_NO)};

static uint8_t get_fuse(uint8_t addr) {
    uint8_t result;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { result = boot_lock_fuse_bits_get(addr); }
    return result;
}

static uint8_t get_sig(uint8_t addr) {
    uint8_t result;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { result = boot_signature_byte_get(addr); }
    return result;
}

static void show_fuses(void) {
    uint8_t fuse_l = get_fuse(GET_LOW_FUSE_BITS);
    uint8_t fuse_h = get_fuse(GET_HIGH_FUSE_BITS);
    uint8_t fuse_e = get_fuse(GET_EXTENDED_FUSE_BITS);
    uint8_t lock   = get_fuse(GET_LOCK_BITS);
    uint8_t sig_1  = get_sig(0);
    uint8_t sig_2  = get_sig(2);
    uint8_t sig_3  = get_sig(4);

    uprintf("Fuse: E:%02X H:%02X L:%02X Lock: %02X\n", fuse_e, fuse_h, fuse_l, lock);
    uprintf("Sig: %02X %02X %02X\n", sig_1, sig_2, sig_3);
}

bool init_done = false;

void matrix_scan_user(void) {
    if (!init_done) {
        if (timer_elapsed(0) >= 3000) {
            init_done = true;
            show_fuses();
        }
    }
}
