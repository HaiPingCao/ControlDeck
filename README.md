# ControlDeck

A simple, multi-purpose, programmable volume knob using Arduino Pro Micro.

## Hardware

- Arduino Pro Micro
- Rotary encoder with push button
- 4 push buttons
- SSD1306 OLED display (128x64)

### Circuit Setup and Wiring

| Component   | Pin | Description                         |
| ----------- | --- | ----------------------------------- |
| Encoder CLK | 5   | Clock signal for rotation detection |
| Encoder DT  | 4   | Data signal for direction detection |
| Encoder SW  | 6   | Push button switch                  |
| UP Button   | 10  | Previous mode                       |
| DOWN Button | 16  | Next mode                           |
| # Button    | 14  | Special function                    |
| \* Button   | 15  | Win+Ctrl+Alt+F5 hotkey              |

## Software

### Required Libraries

Install the following libraries via Arduino Library Manager:

- `HID-Project` by NicoHood
- `Adafruit` GFX Library by Adafruit
- `Adafruit` SSD1306 by Adafruit
