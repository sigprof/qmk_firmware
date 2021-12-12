# Disable the Command feature, which is rather large and not very useful
# (most settings can be changed by using Bootmagic keycodes in the keymap).
COMMAND_ENABLE = no

# Enable mouse emulation.
MOUSEKEY_ENABLE = yes

# Enable RGB underglow (for some reason this is disabled in the default
# configuration for this keyboard).
RGBLIGHT_ENABLE = yes

# Enable NKRO (also disabled in the default configuration).
NKRO_ENABLE = yes

# Enable dynamic macros.
DYNAMIC_MACRO_ENABLE = yes

# Enable link-time optimization (required to make the firmware actually fit
# into flash with all enabled features).
LTO_ENABLE = yes
