#include QMK_KEYBOARD_H

enum layer_names {
    _QWERTY,
    _NUMPAD,
    _FN,
    _ADJUST,
};

// RGB keycode synonyms that fit in 7 characters (ANimation type Inc/Dec).
#define RGB_ANI RGB_MOD
#define RGB_AND RGB_RMOD

// Custom keycodes with names that fit in 7 characters.
#define U_FAPP  LT(_FN, KC_APP)
#define U_MOADJ MO(_ADJUST)
#define U_TGNUM TG(_NUMPAD)
#define U_FCAPS LT(_FN, KC_CAPS)
#define U_NBSLS LT(_NUMPAD, KC_BSLS)
#define U_OSRCT OSM(MOD_RCTL)
#define U_OSRGU OSM(MOD_RGUI)
#define U_CPGUP RCTL(KC_PGUP)
#define U_CPGDN RCTL(KC_PGDN)

enum custom_keycode {
    U_XUNDO = SAFE_RANGE,
    U_XCUT,
    U_XCOPY,
    U_XPSTE,
    U_XREDO,
    U_XSCRU,
    U_XSCRD,
    U_XPANL,
    U_XPANR,
    U_XNKBL,
    U_XDSAW,
    U_XDSAA,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Plain ANSI QWERTY keyboard, except Caps Lock and App (Menu) keys are
     * overloaded (using the Layer Tap functionality) to also work as a
     * momentary switch to the function layer (currently the same function
     * layer is used for both of these keys).
     *
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
     * │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Scr│Pse│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
     * │ ` │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp│ │Ins│Hom│PgU│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │  \  │ │Del│End│PgD│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
     * │CapsFn│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │  Enter │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
     * │ Shift  │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │    Shift │     │ ↑ │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
     * │Ctrl│GUI │Alt │                        │ Alt│ GUI│ApFn│Ctrl│ │ ← │ ↓ │ → │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘
     */
    [_QWERTY] = LAYOUT_tkl_ansi(
    /* ┌───────┐       ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐ ┌───────┬───────┬───────┐ */
        KC_ESC         ,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4      ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8      ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12   ,KC_PSCR,KC_SLCK,KC_PAUS,
    /* └───────┘       └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘ └───────┴───────┴───────┘ */

    /* ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────────────┐ ┌───────┬───────┬───────┐ */
        KC_GRV ,KC_1   ,KC_2   ,KC_3   ,KC_4   ,KC_5   ,KC_6   ,KC_7   ,KC_8   ,KC_9   ,KC_0   ,KC_MINS,KC_EQL ,    KC_BSPC      ,KC_INS ,KC_HOME,KC_PGUP,
    /* ├───────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───────────┤ ├───────┼───────┼───────┤ */
          KC_TAB   ,KC_Q   ,KC_W   ,KC_E   ,KC_R   ,KC_T   ,KC_Y   ,KC_U   ,KC_I   ,KC_O   ,KC_P   ,KC_LBRC,KC_RBRC,  U_NBSLS    ,KC_DEL ,KC_END ,KC_PGDN,
    /* ├───────────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴───────────┤ └───────┴───────┴───────┘ */
           U_FCAPS   ,KC_A   ,KC_S   ,KC_D   ,KC_F   ,KC_G   ,KC_H   ,KC_J   ,KC_K   ,KC_L   ,KC_SCLN,KC_QUOT,     KC_ENT                                ,
    /* ├─────────────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴─────────────────┤         ┌───────┐         */
             KC_LSFT     ,KC_Z   ,KC_X   ,KC_C   ,KC_V   ,KC_B   ,KC_N   ,KC_M   ,KC_COMM,KC_DOT ,KC_SLSH,       KC_RSFT                 ,KC_UP          ,
    /* ├─────────┬───────┴─┬─────┴───┬───┴───────┴───────┴───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┬─────────┤ ┌───────┼───────┼───────┐ */
         KC_LCTL , KC_LGUI , KC_LALT ,                     KC_SPC                      , KC_RALT , KC_RGUI , U_FAPP  , KC_RCTL   ,KC_LEFT,KC_DOWN,KC_RGHT
    /* └─────────┴─────────┴─────────┴─────────────────────────────────────────────────┴─────────┴─────────┴─────────┴─────────┘ └───────┴───────┴───────┘ */
    ),

    /*
     * Numeric keypad emulation layer.
     * Unused alphanumeric keys are turned off.
     *
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
     * │Esc│   │F1 │F2 │F3 │F4 │ │F5 │F6 │F7 │F8 │ │F9 │F10│F11│F12│ │PSc│Scr│Pse│
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
     * │   │P1 │P2 │P3 │P4 │P5 │P6 │P7 │P8 │P9 │P0 │P- │P+ │ Backsp│ │Ins│Hom│PgU│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
     * │ Tab │   │   │   │   │   │   │P4 │P5 │P6 │P* │PEn│NLk│ NLk │ │Del│End│PgD│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
     * │CapsFn│   │   │   │   │   │   │P1 │P2 │P3 │P+ │P* │ PEnter │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
     * │ Shift  │   │   │   │   │   │   │P0 │P, │P. │P/ │    Shift │     │ ↑ │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
     * │Ctrl│GUI │Alt │                        │ Alt│ GUI│ApFn│Ctrl│ │ ← │ ↓ │ → │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘
     */
    [_NUMPAD] = LAYOUT_tkl_ansi(
    /* ┌───────┐       ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐ ┌───────┬───────┬───────┐ */
        _______        ,_______,_______,_______,_______    ,_______,_______,_______,_______    ,_______,_______,_______,_______  ,_______,_______,_______,
    /* └───────┘       └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘ └───────┴───────┴───────┘ */

    /* ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────────────┐ ┌───────┬───────┬───────┐ */
        U_XNKBL,KC_P1  ,KC_P2  ,KC_P3  ,KC_P4  ,KC_P5  ,KC_P6  ,KC_P7  ,KC_P8  ,KC_P9  ,KC_P0  ,KC_PMNS,KC_PPLS,    _______      ,_______,_______,_______,
    /* ├───────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───────────┤ ├───────┼───────┼───────┤ */
          _______  ,U_XDSAW,U_XDSAA,XXXXXXX,U_XSCRU,XXXXXXX,U_XREDO,KC_P4  ,KC_P5  ,KC_P6  ,KC_PAST,KC_PENT,KC_NLCK,  KC_NLCK    ,_______,_______,_______,
    /* ├───────────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴───────────┤ └───────┴───────┴───────┘ */
           _______   ,XXXXXXX,XXXXXXX,U_XPANL,U_XSCRD,U_XPANR,XXXXXXX,KC_P1  ,KC_P2  ,KC_P3  ,KC_PPLS,KC_PAST,     KC_PENT                               ,
    /* ├─────────────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴─────────────────┤         ┌───────┐         */
             _______     ,U_XUNDO,U_XCUT ,U_XCOPY,U_XPSTE,XXXXXXX,XXXXXXX,KC_P0  ,KC_PCMM,KC_PDOT,KC_PSLS,       _______                 ,_______        ,
    /* ├─────────┬───────┴─┬─────┴───┬───┴───────┴───────┴───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┬─────────┤ ┌───────┼───────┼───────┐ */
         _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______   ,_______,_______,_______
    /* └─────────┴─────────┴─────────┴─────────────────────────────────────────────────┴─────────┴─────────┴─────────┴─────────┘ └───────┴───────┴───────┘ */
    ),

    /*
     * Function layer:
     * - mouse emulation
     * - cursor and nav cluster keys (for compatibility with 60%)
     * - one-shot RCtrl and RGUI (for compatibility with 60%/75%)
     * - Num Lock key
     * - toggle number pad emulation layer
     * - media keys
     */
    [_FN] = LAYOUT_tkl_ansi(
    /* ┌───────┐       ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐ ┌───────┬───────┬───────┐ */
        XXXXXXX        ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX    ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX    ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX  ,XXXXXXX,KC_NLCK,XXXXXXX,
    /* └───────┘       └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘ └───────┴───────┴───────┘ */

    /* ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────────────┐ ┌───────┬───────┬───────┐ */
        KC_GRV ,KC_F1  ,KC_F2  ,KC_F3  ,KC_F4  ,KC_F5  ,KC_F6  ,KC_F7  ,KC_F8  ,KC_F9  ,KC_F10 ,KC_F11 ,KC_F12 ,    KC_DEL       ,XXXXXXX,XXXXXXX,U_CPGUP,
    /* ├───────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───────────┤ ├───────┼───────┼───────┤ */
          XXXXXXX  ,KC_BTN1,KC_MS_U,KC_BTN2,KC_WH_U,XXXXXXX,KC_INS ,KC_HOME,KC_UP  ,KC_END ,KC_PGUP,KC_VOLU,KC_MUTE,  U_TGNUM    ,XXXXXXX,XXXXXXX,U_CPGDN,
    /* ├───────────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴───────────┤ └───────┴───────┴───────┘ */
           _______   ,KC_MS_L,KC_MS_D,KC_MS_R,KC_WH_D,XXXXXXX,KC_DEL ,KC_LEFT,KC_DOWN,KC_RGHT,KC_PGDN,KC_VOLD,     KC_PENT                               ,
    /* ├─────────────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴─────────────────┤         ┌───────┐         */
             _______     ,KC_BTN3,KC_BTN4,KC_BTN5,KC_WH_L,KC_WH_R,KC_PSCR,KC_SLCK,KC_PAUS,U_OSRGU,U_OSRCT,       _______                 ,KC_PGUP        ,
    /* ├─────────┬───────┴─┬─────┴───┬───┴───────┴───────┴───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┬─────────┤ ┌───────┼───────┼───────┐ */
         _______ , _______ , _______ ,                     U_MOADJ                     , _______ , _______ , _______ , _______   ,KC_HOME,KC_PGDN,KC_END
    /* └─────────┴─────────┴─────────┴─────────────────────────────────────────────────┴─────────┴─────────┴─────────┴─────────┘ └───────┴───────┴───────┘ */
    ),

    /*
     * Adjust layer:
     * - backlight control
     * - RGB underglow control
     * - reset to bootloader
     * - EEPROM reset
     * - NKRO config
     */
    /*
     * ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
     * │Rst│   │   │   │   │   │ │   │   │   │   │ │   │NK-│NK+│Dbg│ │   │   │   │
     * └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐
     * │   │   │   │   │   │   │   │   │   │   │   │   │NK±│EEReset│ │   │   │   │
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤
     * │     │BTg│BL-│BL+│BBr│   │   │   │   │   │   │   │   │     │ │   │   │   │
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘
     * │CapsFn│UTg│UM+│UH+│US+│UV+│Sp+│   │   │   │   │   │        │
     * ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐
     * │ Shift  │UMP│UM-│UH-│US-│UV-│Sp-│   │   │   │   │    Shift │     │   │
     * ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐
     * │Ctrl│GUI │Alt │         Adjust         │ Alt│ GUI│ApFn│Ctrl│ │   │   │   │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘
     */
    [_ADJUST] = LAYOUT_tkl_ansi(
    /* ┌───────┐       ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐   ┌───────┬───────┬───────┬───────┐ ┌───────┬───────┬───────┐ */
        RESET          ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX    ,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX    ,XXXXXXX,NK_OFF ,NK_ON  ,DEBUG    ,XXXXXXX,XXXXXXX,XXXXXXX,
    /* └───────┘       └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘   └───────┴───────┴───────┴───────┘ └───────┴───────┴───────┘ */

    /* ┌───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────┬───────────────┐ ┌───────┬───────┬───────┐ */
        XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,NK_TOGG,    EEP_RST      ,XXXXXXX,XXXXXXX,XXXXXXX,
    /* ├───────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───────────┤ ├───────┼───────┼───────┤ */
          XXXXXXX  ,BL_TOGG,BL_DEC ,BL_INC ,BL_BRTG,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,  XXXXXXX    ,XXXXXXX,XXXXXXX,XXXXXXX,
    /* ├───────────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴─┬─────┴───────────┤ └───────┴───────┴───────┘ */
           _______   ,RGB_TOG,RGB_ANI,RGB_HUI,RGB_SAI,RGB_VAI,RGB_SPI,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,     XXXXXXX                               ,
    /* ├─────────────┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴───┬───┴─────────────────┤         ┌───────┐         */
             _______     ,RGB_M_P,RGB_AND,RGB_HUD,RGB_SAD,RGB_VAD,RGB_SPD,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,       _______                 ,XXXXXXX        ,
    /* ├─────────┬───────┴─┬─────┴───┬───┴───────┴───────┴───────┴───────┴───────┴─────┬─┴───────┼───────┴─┬─────────┬─────────┤ ┌───────┼───────┼───────┐ */
         _______ , _______ , _______ ,                     _______                     , _______ , _______ , _______ , _______   ,XXXXXXX,XXXXXXX,XXXXXXX
    /* └─────────┴─────────┴─────────┴─────────────────────────────────────────────────┴─────────┴─────────┴─────────┴─────────┘ └───────┴───────┴───────┘ */
    ),
};

// {{{1 Test QMK timers: release Esc after it was held for 1000 ms
//
// This test can reveal timer skew when some code disables interrupts for a
// longer time than the timer interrupt period; when some timer interrupts are
// lost because of that, the actual delay before Esc is released by this code
// becomes more than the intended 1000 ms.

#define TEST_ESC_RELEASE_BY_TIMER

#ifdef TEST_ESC_RELEASE_BY_TIMER
static bool esc_pressed = false;
static uint16_t esc_timer;
#endif

static bool process_record_esc_release_by_timer(uint16_t keycode, keyrecord_t *record) {
#ifdef TEST_ESC_RELEASE_BY_TIMER
    switch (keycode) {
        case KC_ESC:
            if (record->event.pressed) {
                esc_pressed = true;
                esc_timer = timer_read();
            } else {
                esc_pressed = false;
            }
            break;
    }
#endif
    return true;
}

static void matrix_scan_esc_release_by_timer(void) {
#ifdef TEST_ESC_RELEASE_BY_TIMER
    if (esc_pressed) {
        if (timer_elapsed(esc_timer) >= 1000) {
            esc_pressed = false;
            unregister_code(KC_ESC);
        }
    }
#endif
}

// }}}1

static struct {
    bool group_led : 1;
} indicators;

static void update_indicators(void) {
    if (indicators.group_led) {
        backlight_set(get_backlight_level());
    } else {
        backlight_set(0);
    }
}

void keyboard_post_init_user(void) {
    update_indicators();
}

bool led_update_user(led_t led_state) {
    // Assume that "grp_led:scroll" is used to make the Scroll Lock LED
    // indicate the input language (XKB group).
    if (indicators.group_led != led_state.scroll_lock) {
        indicators.group_led = led_state.scroll_lock;
        update_indicators();
    }

    // Keep the default Caps Lock LED function
    return true;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case U_FCAPS:
        case U_FAPP:
            return true;
    }
    return false;
}

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case U_NBSLS:
            return true;
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_esc_release_by_timer(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case U_XUNDO:
            host_consumer_send(record->event.pressed ? 0x21a : 0);
            return true;
        case U_XCUT:
            host_consumer_send(record->event.pressed ? 0x21c : 0);
            return true;
        case U_XCOPY:
            host_consumer_send(record->event.pressed ? 0x21b : 0);
            return true;
        case U_XPSTE:
            host_consumer_send(record->event.pressed ? 0x21d : 0);
            return true;
        case U_XREDO:
            host_consumer_send(record->event.pressed ? 0x279 : 0);
            return true;
        case U_XSCRU:
            host_consumer_send(record->event.pressed ? 0x233 : 0);
            return true;
        case U_XSCRD:
            host_consumer_send(record->event.pressed ? 0x234 : 0);
            return true;
        case U_XPANL:
            host_consumer_send(record->event.pressed ? 0x236 : 0);
            return true;
        case U_XPANR:
            host_consumer_send(record->event.pressed ? 0x237 : 0);
            return true;
        case U_XNKBL:
            host_consumer_send(record->event.pressed ? 0x29d : 0);
            return true;
        case U_XDSAW:
            host_consumer_send(record->event.pressed ? 0x29f : 0);
            return true;
        case U_XDSAA:
            host_consumer_send(record->event.pressed ? 0x2a2 : 0);
            return true;
    }

    return true;
}

void matrix_scan_user(void) {
    matrix_scan_esc_release_by_timer();
}
