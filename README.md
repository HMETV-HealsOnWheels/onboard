# Heals on Wheels – Onboard System

This repository contains the **onboard software and firmware** for the Heals on Wheels autonomous medical cart. It manages the physical movement of the cart, processes commands received over a TCP connection, and controls motor behavior using an Arduino and Raspberry Pi setup.

---


## Contents

- `arduino.ino` – Arduino sketch for controlling four servo motors with acceleration, cruise, and deceleration logic.
- `server.py` – Python TCP server running on the Raspberry Pi to handle incoming movement commands and relay them to the Arduino.

---

## Getting Started

### Hardware Setup

- Arduino Mega connected to motor controllers and servo motors.
- Raspberry Pi connected to the Arduino via USB (e.g., `/dev/ttyACM0`).
- Raspberry Pi connected to hospital’s secure network (**Skynet WiFi**).

### Network Setup

- Ensure you are connected to **Skynet WiFi**.
- The Raspberry Pi’s IP address may change, so use an **IP scanner** (e.g., [Advanced IP Scanner](https://www.advanced-ip-scanner.com/)) to find it.
- Typical IP format: `192.168.1.xx`

## Uploading Code to Arduino (from Raspberry Pi)
### 1. SSH into your Raspberry Pi:
```bash
ssh hmetv@<raspberry-pi-ip>
```
### 2. Compile:
```bash
arduino-cli compile --fqbn arduino:avr:mega /home/hmetv/path/to/arduino.ino
```
### 3. Upload:
```bash
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:mega /home/hmetv/path/to/arduino.ino
```

## Running the Server

### 1. (If you haven't already) SSH into your Raspberry Pi:
```bash
ssh hmetv@<raspberry-pi-ip>
```
### 2. Navigate to the folder containing `server.py`:
```bash
cd ~/TCPcode
```

### 3. Run the server:
```bash
python3 server.py
```
