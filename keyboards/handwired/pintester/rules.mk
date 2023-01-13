# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = no        # Audio control and System control
CONSOLE_ENABLE = yes        # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
# Do not enable SLEEP_LED_ENABLE. it uses the same timer as BACKLIGHT_ENABLE
SLEEP_LED_ENABLE = no       # Breathing sleep LED during USB suspend
# if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
NKRO_ENABLE = no            # USB Nkey Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

# Disable unneeded features
SPACE_CADET_ENABLE = no
MAGIC_ENABLE = no
GRAVE_ESC_ENABLE = no

# Allow overriding some options from the make command line
OPT_DEFS += $(if $(PINTESTER_IGNORE_PINS),-DPINTESTER_IGNORE_PINS="$(PINTESTER_IGNORE_PINS)")
OPT_DEFS += $(if $(PINTESTER_INPUT_PINS),-DPINTESTER_INPUT_PINS="$(PINTESTER_INPUT_PINS)")
OPT_DEFS += $(if $(PINTESTER_OUTPUT_PINS),-DPINTESTER_OUTPUT_PINS="$(PINTESTER_OUTPUT_PINS)")

CUSTOM_MATRIX = lite
SRC += matrix.c
