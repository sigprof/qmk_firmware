# young_man_diy/sofle/f072_onboard

![young_man_diy/sofle/f072_onboard](imgur.com image replace me!)

A variant of the Sofle RGB keyboard sold by some shops on AliExpress (e.g., SpiderIsland), sometimes using the “DEBROGLIE” brand.  The PCBs are labeled as “Young Man_Diy Sofle V1.0 Left/Right”; the MCU is APM32F072CBT6 (a clone of STM32F072CBT6), installed on the bottom side of the keyboard PCB (**not** on a separate controller board).

**Warning**: Some AliExpress shops also sell a version of Sofle RGB which uses separate controller boards similar to Pro Micro, but with the APM32F072CBT6 chip instead of the traditional ATmega32U4; that version is **not** compatible with this firmware.

* Keyboard Maintainer: [Sergey Vlasov](https://github.com/sigprof)
* Hardware Supported: The “Young Man_Diy Sofle V1.0” PCB with APM32F072CBT6
* Hardware Availability: [AliExpress](https://www.aliexpress.us/item/3256807112304771.html)

Make example for this keyboard (after setting up your build environment):

    make young_man_diy/sofle/f072_onboard:default

Flashing example for this keyboard:

    make young_man_diy/sofle/f072_onboard:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the top left key on the left half or the top right key on the right half, then connect the USB cable to that half.
* **Physical button**: Hold down the button on the PCB (located near the USB-C socket used to connect the halves to each other), then connect the USB cable; release the button after about 2 seconds.
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available.

## Setting handedness

Using the bootmagic reset to enter the bootloader should result in setting the EEPROM handedness automatically.  However, if you are updating from the factory firmware, or the handedness configuration becomes wrong for any other reason, you can configure the handedness manually without reflashing the firmware:
* **Left half**: Hold down the leftmost key on the second row from top (below the bootmagic key; Esc in the default keymap), then connect the USB cable.
* **Right half**: Hold down the leftmost key on the second row from top (Y in the default keymap), then connect the USB cable.
