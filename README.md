# esp32-circular-digital-level
An ESP32-based digital spirit level with a circular display, featuring real-time angle calculation and horizon animations.

MakerWorld 3D printed level:

https://makerworld.com/fr/models/2357932-macropad-with-joystick-for-arduino-pro-micro#profileId-2579549 
@champrouxantoin

## 🌟 Features
- Precision Leveling: Real-time angle display for 0°, 45°, 90°, and custom slopes.
- Horizon Animation: A smooth graphical ring that follows the physical horizon using IMU data.
- Portable Design: Powered by an internal battery with a custom 3D-printed enclosure.
- MakerWorld Integration: Fully 3D-printable housing optimized for assembly.

## 🛠️ Hardware Components
- Microcontroller: ESP32 round display (for fast dual-core processing).

## 💻 Software Logic
The firmware handles:
- Sensor Fusion: Filtering raw accelerometer/gyroscope data.
- Custom Graphics: Drawing dynamic UI elements on a circular coordinate system.
- Trigonometry: Calculating precise degrees on multiple axes.

## 🚀 Installation
1. Clone this repo.
2. Ensure you have the `TFT_eSPI` or `LVGL` library installed.
3. Upload `src/digital_level.ino` using the Arduino IDE.

## 📜 License
MIT License
