#include "layouts/community/tkl_ansi/sigprof-ansi/keymap.c"

bool led_update_user(led_t led_state)
{
    writePin(E2, !led_state.kana);
    return false;
}
