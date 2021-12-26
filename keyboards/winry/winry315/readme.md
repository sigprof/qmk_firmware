# Winry315

![Winry315](https://i.imgur.com/nWE5mbXh.jpeg)

Winry315 is a macropad with 3 rotary encoders and 15 keys (arranged as 3 rows of 5 keys).

This board may also be known as “YD3xn15mx”, “YD315” or “YD 3x15m”.  Boards sold by the [SpiderIsland shop on AliExpress](https://www.aliexpress.com/store/5241107) may have “DEBROGLIE” on the bottom side of the case.

More photos can be found [in this Imgur album](https://imgur.com/a/0xf9pju).

* Keyboard Maintainer: [Sergey Vlasov](https://github.com/sigprof)
* Hardware Supported: Winry315 PCB
* Hardware Availability: [Taobao](https://world.taobao.com/item/657924681898.htm), [AliExpress](https://www.aliexpress.com/item/1005003500083583.html)

Make example for this keyboard (after setting up your build environment):

    make winry/winry315:default

Flashing example for this keyboard:

    make winry/winry315:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the top left key (not the encoder) and plug in the keyboard.  This apparently also works with the vendor VIA firmware.
* **Physical reset button**: Briefly press the button on the back of the PCB (the acrylic bottom cover should have a hole to access that button).
* **Keycode in layout**: Press the key mapped to `RESET` if it is available (the default keymap does not have that keycode assigned to any key, but you can use that keycode in your custom keymap if you want to have easier access to the bootloader).
