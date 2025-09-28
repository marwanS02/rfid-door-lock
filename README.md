# RFID Door Lock (Arduino)

Firmware for a solenoid door lock that supports **RFID**, **IR remote**, and an **inside button**, with LCD status, people counter, LEDs, and buzzer.

## Features
- RFID access control with admin add/remove by scanning unknown cards
- IR remote:
  - **CH** → open door (enter)
  - **+**  → grant access to scanned RFID
  - **-**  → revoke access to scanned RFID
  - **EQ** → toggle “everyone has access”
- Inside button → open door (exit) and decrement counter
- LCD messages, LED+beeper feedback, 8×8 display as people counter
- Solenoid via relay (12 V), auto-close after 5 s

## Hardware
- Arduino **Mega** (SPI pins used for MFRC522)
- MFRC522 RFID reader
- IR receiver (connected to `RECV_PIN`)
- 20×4 I²C LCD (addr `0x27`)
- 8×8 LED matrix (MAX7219)
- Relay module + **12 V solenoid** (with flyback diode)
- LEDs (green/red), piezo buzzer, momentary button

See [`docs/wiring.md`](docs/wiring.md) for pin table and diagram.

## Libraries (tested versions)
- `MFRC522` (miguelbalboa)
- `IRremote` (v3+ API)
- `LiquidCrystal_I2C`
- `LedControl`

## Build/Upload
1. Arduino IDE → Board: **Arduino Mega 2560**.
2. Install libraries above.
3. Open `rfid-door-lock.ino` and upload.

## Usage (admin operations)
- Present a card and press **+** to grant; **-** to revoke.
- Press **EQ** to toggle “everyone access”.
- LCD + LEDs + buzzer indicate success/failure.

## Safety notes
- Solenoid is inductive: use **flyback diode** across the coil.
- Relay contacts must match the solenoid current.
- Use a separate 12 V supply for the solenoid; **common ground** with Arduino.

## Roadmap
- EEPROM storage of allowed UIDs
- Non-blocking timing (no `delay`)
- Proper state machine, button debounce, UID length agnostic

## Dependencies

The following Arduino libraries are required:

- [MFRC522](https://github.com/miguelbalboa/rfid) – RFID reader module
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) – I²C LCD display
- [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) – IR remote decoding
- [LedControl](https://github.com/wayoda/LedControl) – MAX7219 LED matrix control

📦 Install them via Arduino IDE → **Sketch → Include Library → Manage Libraries...**  
Search each name and install the latest tested version.
