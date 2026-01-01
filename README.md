# ControlDeck

ControlDeck is a simple, multi-purpose, programmable volume knob built with an Arduino Pro Micro.  
It is designed as a compact hardware control interface for volume adjustment, mode switching, and custom hotkeys.

## Hardware

The project uses the following components:

- Arduino Pro Micro
- Rotary encoder with integrated push button
- 4 momentary push buttons
- SSD1306 OLED display (128×64)

### Circuit Setup and Wiring

| Component   | Pin | Description                                  |
| ----------- | --- | -------------------------------------------- |
| Encoder CLK | 5   | Clock signal for rotation detection          |
| Encoder DT  | 4   | Data signal for rotation direction detection |
| Encoder SW  | 6   | Encoder push button                          |
| UP Button   | 10  | Switch to previous mode                      |
| DOWN Button | 16  | Switch to next mode                          |
| # Button    | 14  | Special function button                      |
| \* Button   | 15  | Triggers Win + Ctrl + Alt + F5 hotkey        |

## Software

### Required Libraries

When using the Arduino IDE, install the following libraries via the Arduino Library Manager:

- **HID-Project** by NicoHood
- **Adafruit GFX Library** by Adafruit
- **Adafruit SSD1306** by Adafruit

### Driver Development

A new custom driver written in Rust is currently in early development to support more advanced features.  
See the `mk-driver` directory for details.
