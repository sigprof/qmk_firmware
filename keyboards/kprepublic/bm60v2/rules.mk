LTO_ENABLE = yes

# RGB Matrix is required to support per-key LEDs connected to IS31FL3733.
RGB_MATRIX_ENABLE = yes

# The simpler RGB Lighting code can be enabled to control just the underglow
# LEDs from it; leave it disabled to make the RGB Matrix code control all LEDs,
# including underglow.
RGBLIGHT_ENABLE = no

# The custom RGB Matrix driver combines IS31FL3733 and WS2812; things that are
# normally done by common_features.mk for both of these drivers need to be done
# here manually.
RGB_MATRIX_DRIVER = custom
COMMON_VPATH += $(DRIVER_PATH)/led/issi
SRC += is31fl3733.c
QUANTUM_LIB_SRC += i2c_master.c
WS2812_DRIVER_REQUIRED = yes
