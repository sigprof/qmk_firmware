#include QMK_KEYBOARD_H

enum Layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
};

#ifdef OLED_ENABLE
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}

static void render_status(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer\n"), false);
  oled_write_P(PSTR("-----\n"), false);
  switch (get_highest_layer(layer_state)) {
  case _BASE:
    oled_write_P(PSTR("Base \n"), false);
    break;
  case _LOWER:
    oled_write_P(PSTR("Lower\n"), false);
    break;
  case _RAISE:
    oled_write_P(PSTR("Raise\n"), false);
    break;
  case _ADJUST:
    oled_write_P(PSTR("Adjust\n"), false);
    break;
  default:
    oled_write_P(PSTR("Undef\n"), false);
  }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;
}

bool oled_task_kb(void) {
	if (!oled_task_user()) {
		return false;
	}

  if (is_keyboard_master()) {
      render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
  } else {
      render_logo();
  }

  return true;
}
#endif