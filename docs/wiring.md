# Wiring Guide — RFID Door Lock

This document describes the hardware connections for the RFID Door Lock system.

---

## 🖥️ Microcontroller
- **Board:** Arduino Mega 2560  
- **Logic level:** 5 V  

---

## 🔑 RFID Reader (MFRC522)
| MFRC522 Pin | Arduino Mega Pin |
|-------------|------------------|
| SDA (SS)    | 53               |
| SCK         | 52 (SCK)         |
| MOSI        | 51 (MOSI)        |
| MISO        | 50 (MISO)        |
| RST         | 5                |
| VCC         | 3.3 V            |
| GND         | GND              |

> ⚠️ The MFRC522 requires **3.3 V**. Do **not** connect to 5 V.

---

## 📺 LCD (I²C, 20x4, addr 0x27)
| LCD Pin | Arduino Mega Pin |
|---------|------------------|
| SDA     | 20 (SDA)         |
| SCL     | 21 (SCL)         |
| VCC     | 5 V              |
| GND     | GND              |

---

## 📡 IR Receiver
| IR Receiver Pin | Arduino Mega Pin |
|-----------------|------------------|
| OUT             | 3 (`RECV_PIN`)   |
| VCC             | 5 V              |
| GND             | GND              |

---

## 🔢 LED Matrix (MAX7219)
| MAX7219 Pin | Arduino Mega Pin |
|-------------|------------------|
| DIN         | A5               |
| CS          | A6               |
| CLK         | A7               |
| VCC         | 5 V              |
| GND         | GND              |

---

## 🚪 Relay + Solenoid Lock
| Relay Module Pin | Arduino Mega Pin |
|------------------|------------------|
| IN               | 10 (`unlock`)    |
| VCC              | 5 V              |
| GND              | GND              |

- Relay switches a **12 V supply** to the solenoid lock.  
- Solenoid lock requires manual closing after 5 s energize.  
- **Flyback diode** (e.g. 1N4007) must be placed across solenoid coil to protect relay and MCU.  
- Use a separate 12 V supply rated for the solenoid current; connect **grounds together** with Arduino.

---

## 🔘 Other Components
- **Exit button** → Pin 49 (with `INPUT_PULLUP` recommended).  
- **Green LED (AccessLED)** → Pin 9 (with series resistor).  
- **Red LED (DenyLED)** → Pin 7 (with series resistor).  
- **Buzzer** → Pin 8 (PWM capable).

---

## 📝 Notes
- All grounds (**GND**) must be tied together: Arduino, MFRC522, relay module, solenoid PSU.  
- For safety, ensure relay is rated for solenoid current (including inrush).  
- Debounce may be required for the button if using `INPUT` instead of `INPUT_PULLUP`.  
- Double-check I²C LCD address with an I²C scanner (default 0x27 but may vary).  

---

✅ With this wiring, the system supports RFID card reading, IR remote admin functions, occupancy counting, LCD status messages, and solenoid-based door unlocking.
