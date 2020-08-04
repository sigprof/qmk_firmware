# Enable VIA support.  This also sets `BOOTMAGIC_ENABLE = lite`.
VIA_ENABLE = yes

# Enable link-time optimization, which is effectively required to make the
# firmware fit into the available flash space.
LTO_ENABLE = yes

# Disable the Command feature, which is rather large and not very useful
# (most settings can be changed by using Bootmagic keycodes in the keymap).
COMMAND_ENABLE = no

# Enable mouse emulation.
MOUSEKEY_ENABLE = yes

# Enable NKRO (disabled in the default configuration for some reason).
# Note that NKRO is still an option which needs to be enabled either in the
# EEPROM settings (e.g., by adding the “Toggle NKRO” key to the keymap and
# pressing it once), or using `#define FORCE_NKRO` in `config.h`.
NKRO_ENABLE = yes

# Enable RGB underglow (mostly invisible with the stock plastic case, but still
# useful for testing).
RGBLIGHT_ENABLE = yes

# Disable the debug console (apparently it has compatibility issues with the
# SteelSeries Engine 2 software in Windows).
CONSOLE_ENABLE = no

# Use a single endpoint for all keyboard-related HID reports (by default the
# Boot Keyboard protocol is handled using a separate endpoint, but recent UEFI
# firmware versions work fine even when using NKRO on a shared endpoint).
KEYBOARD_SHARED_EP = yes
