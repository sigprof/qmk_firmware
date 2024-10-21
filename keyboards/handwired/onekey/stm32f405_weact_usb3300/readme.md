# WeAct STM32F405 CoreBoard + USB3300 ULPI onekey

* Supported Hardware:
  - [WeAct STM32F405RGT6 CoreBoard](https://github.com/WeActStudio/WeActStudio.STM32F4_64Pin_CoreBoard)
  - [WaveShare USB3300 USB HS Board](https://www.waveshare.com/wiki/USB3300_USB_HS_Board)

Note that the WaveShare USB3300 USB HS Board cannot supply power from VBUS to the MCU (the VBUS power switch works only in one direction, therefore that board can only work as a VBUS source), so you need to supply power to the WeAct board from some other source (e.g., using its USB FS connector).

To trigger keypress, short together pins *A1* and *A2*.
