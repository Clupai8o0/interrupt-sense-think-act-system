# SIT315 - Interrupt-Based Sense-Think-Act System (4 QP HD Submission)

## Project Overview
This project implements a **real-time embedded Sense–Think–Act system** using an Arduino-compatible microcontroller. It integrates **multiple sensors (moisture, ultrasonic, button)** and actuators (LEDs) with an **interrupt-driven design**, demonstrating both **event-driven (PCI)** and **time-driven (Timer1)** logic.

The system follows a **modular and flag-based architecture**, ensuring responsiveness and scalability while adhering to best practices for interrupt-safe programming.

---

## Features
- **Moisture Sensor (Analog):** Continuously measures soil moisture.
- **Ultrasonic Sensor (Digital PCI):** Calculates distance using echo edge detection.
- **Button Input (PCI):** Captures user input instantly without polling.
- **Conditional LED (Event-Driven):** Turns ON when button is pressed, object is close, and soil is dry.
- **Blinking LED (Timer1 Interrupt):** Demonstrates periodic time-driven logic independent of PCI events.
- **Flag-Based ISR Design:** ISRs perform minimal work (timestamping/flag updates) while main loop handles logic.
- **Concurrent Interrupt Handling:** PCI and Timer1 run concurrently without blocking.

---

## System Architecture
1. **Sense:** 
   - Moisture via `analogRead()`
   - Ultrasonic distance via PCI timestamping (rising/falling edges)
   - Button state via PCI
2. **Think:** 
   - Evaluates conditions using `MOISTURE_THRESHOLD` and `DISTANCE_THRESHOLD`
   - Encapsulated logic in `shouldTurnOnLED()`
3. **Act:** 
   - Updates LEDs based on evaluated conditions
   - Timer1 toggles blinking LED independently

---

## Hardware Requirements
- Arduino Uno (or compatible board)
- Moisture sensor (Analog)
- Ultrasonic sensor (HC-SR04 or equivalent)
- Push button (Digital input)
- 2x LEDs + Resistors (220Ω recommended)
- Breadboard and jumper wires

---

## Pin Configuration
| Component         | Pin      |
|--------------------|----------|
| Button Input       | D8 (PCI) |
| Ultrasonic Trigger | D6       |
| Ultrasonic Echo    | D9 (PCI) |
| Moisture Sensor    | A0       |
| Conditional LED    | D13      |
| Blinking LED       | D12      |

---

## Setup Instructions
1. **Clone the repository** (private as per SIT315 requirements).
   ```bash
   git clone <repo-url>
   ```
2. Open `main.cpp` in the Arduino IDE or VS Code (PlatformIO).
3. Connect the circuit using the pin mapping above.
4. Upload the code to the Arduino board.

5. Open Serial Monitor at 9600 baud to view sensor readings and LED logic outputs.

## Usage

- Press the button while an object is <100 cm away and the soil is dry to turn on the Conditional LED.
- Observe the Blinking LED toggling every second (Timer1 interrupt).
- View live moisture, distance, and button state data in the Serial Monitor.

## Demonstration

Serial Monitor Output: 
![Serial Monitor Output](serial-monitor.png)

TinkerCad Circuit: 
![Circuit diagram](board.png)