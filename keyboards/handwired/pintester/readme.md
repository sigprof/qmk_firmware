# Pin Tester

This “keyboard” firmware can be used to test pins of a controller board such as Pro Micro, or to determine the layout of some unknown keyboard matrix.

* Keyboard Maintainer: QMK Community
* Hardware Supported: Various (see subfolders)
* Hardware Availability: Various

**See each individual board for pin information**

## How it works

Custom matrix code is used to scan all controller pins and try to find connections between them in both directions:

- Initially all pins are set to the “input with pull-up” state, and any pins which read as active (low) are assumed to be active direct pins; these pins are reported and then excluded from the subsequent scan.
- For each pin that was not detected as an active direct pin:
  - The pin is set to the “output low” state.
  - All other non-excluded pins (including those that were previously used as outputs) are read; for any pins which read as active (low) the pair of output and input pins is reported.
  - The pin is returned to the “input with pull-up” state.

It is also possible to reverse all logic levels (use the “input with pull-down” state as inactive, and the “output high” state as active) by adding `#define PINTESTER_ACTIVE_HIGH` to `config.h`; the simplest way to do that is to use the provided `active_high` keymap instead of `default`.

!> The `PINTESTER_ACTIVE_HIGH` code and the `active_high` keymap won't compile for MCUs which do not support the “input with pull-down” state for GPIO pins; in particular, AVR MCUs do not support that.  Please do not report these build errors as bugs.

Scan results are reported as keyboard input in a special text format.  This reporting assumes that the computer uses the standard US QWERTY layout; you may be able to add a `config.h` file with the appropriate [sendstring configuration](https://docs.qmk.fm/#/reference_keymap_extras?id=sendstring-support) to make it work with some other national layout.

Assuming that the MCU has pins named `B0` and `B1`:
- If the pin `B0` was detected as an active direct pin, the report on press will be `[B0`, and the report on release will be `-B0]`.  However, if the release event comes immediately after the press event for the same pin, the release report will be just `]`, so the whole report for the press+release sequence will be `[B0]`.
- If a connection between the output pin `B0` and the input pin `B1` was detected, the report on press will be `(B0,B1`, and the report on release will be `-B0,B1)`.  However, if the release event comes immediately after the press event for the same pin pair, the release report will be just `)`, so the whole report for the press+release sequence will be `(B0,B1)`.
- If two connections between the same pair of pins in both directions were detected within the debounce time (which would happen if the pins were connected without a diode — e.g., when testing with a piece of wire, or when using a key matrix without diodes), the second press report will be shortened to `(<>` instead of repeating the same pin names in a different order.  If both release reports for those pins also arrive within the debounce time, they will be shortened to `))`, so the report for the whole press+release sequence without diodes will be `(B0,B1 (<>))`.

For some controller boards the reported pin names have extra suffixes (pin labels) after `/` that show the pin name as written on the board itself.  E.g., in the `handwired/pintester/promicro` firmware the `D0` pin would be reported as `D0/3`, and the `D3` pin would be reported as `D3/TXO/1`; in this case the suffixes correspond to the Arduino pin names which are often written on the silkscreen of those controller boards.

The start of matrix scanning is delayed by 2 seconds (configurable through `PINTESTER_STARTUP_DELAY`), so that any pins and connections that appear to be always active would actually be reported to the host.

!> You may get some keypresses from this keyboard firmware almost immediately after connecting the USB cable, even if you don't actually perform any actions, especially if you are using this firmware to scan some unknown matrix.  Make sure that any such keypresses won't result in the computer software doing some undesired things.  The firmware never sends the Enter key, so accidentally getting that input on your command line should be mostly safe, but some software like `vi` in normal mode may do something bad when interpreting such input.

## Compiling the firmware

The `handwired/pintester` keyboard code cannot be compiled by itself; you need to choose the subfolder for your specific controller — e.g., `handwired/pintester/promicro`.

Make example for this keyboard (after setting up your build environment):

    make handwired/pintester/promicro:default

Flashing example for this keyboard:

    make handwired/pintester/promicro:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Extra compile options

### Ignoring some pins

In some cases you may want to skip scanning some MCU pins (e.g., if you are using pintester to scan some unknown keyboard matrix and already know that some pins are used for other purposes).  You may configure the pintester firmware to ignore some MCU pins by defining the `PINTESTER_IGNORE_PINS` macro, e.g., in `config.h`:

```c
#define PINTESTER_IGNORE_PINS E2, D0, D1
```

However, the pintester code provides an easy way to do it on the `make` or `qmk compile`/`qmk flash` command line — by defining a make variable with the same name:

    make handwired/pintester/atmega32u4:default:flash PINTESTER_IGNORE_PINS=E2,D0,D1

or

    qmk flash -kb handwired/pintester/atmega32u4 -km default -e PINTESTER_IGNORE_PINS=E2,D0,D1

Note that some variants of the pintester firmware already have some pins in the ignore list by default; if you are specifying `PINTESTER_IGNORE_PINS` yourself, you will need to list those pins in your value if you still want them to be ignored.  If you want to remove all pins from the ignore list, use `PINTESTER_IGNORE_PINS=NO_PIN` (setting the variable to an empty string won't work).

## Bootloader

This special firmware does not provide any way to enter the bootloader (all controller pins are scanned as a keyboard matrix, no user-provided keymap is used, and the Bootmagic feature is not enabled).  The only ways to enter the bootloader would be either a hardware reset button or some bootloader-specific method (which may not exist for some bootloaders).

!> Do not flash this firmware into a device for which you don't know a method to enter the bootloader that is independent from the firmware — e.g., if you can enter the bootloader only using the Bootmagic feature of some existing firmware.  For some devices entering the bootloader without any cooperation from the firmware may be possible only by using some extra hardware (ISP, ST-Link, JTAG, …).
