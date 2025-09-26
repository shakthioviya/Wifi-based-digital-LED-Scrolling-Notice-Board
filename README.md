# Wifi-based-digital-LED-Scrolling-Notice-Board

WiFi Based Digital LED Scrolling Notice Board

📌 Project Overview
This project implements an IoT-enabled smart notice board that allows users to send messages remotely via Adafruit IO. The messages are then displayed in real-time on a MAX7219 LED scrolling matrix and an I²C LCD display, with a buzzer alert for new updates. The system uses ESP8266 (NodeMCU) for WiFi + Adafruit IO integration and Arduino Nano for display control.

✨ Features

✅ Remote message posting via Adafruit IO dashboard

✅ Real-time scrolling on MAX7219 LED matrices

✅ Parallel message output on LCD (I²C)

✅ Buzzer alert when new message arrives

✅ Low-cost and scalable solution for schools, colleges, and public spaces

🛠️ Tech Stack

Hardware: Arduino Nano, ESP8266 NodeMCU, MAX7219 LED Matrix, I²C LCD, Buzzer

Software: Arduino IDE, Adafruit IO, MQTT Protocol

Libraries:

MD_Parola & MD_MAX72xx → LED matrix scrolling

LiquidCrystal_I2C → LCD handling

PubSubClient → MQTT communication with Adafruit IO

🔌 Circuit Overview

ESP8266 connects to WiFi and subscribes to Adafruit IO feed.

Arduino Nano drives the LED matrix and LCD.

Messages flow: Adafruit IO → ESP8266 → Arduino Nano → LED Matrix + LCD + Buzzer.

Common GND required between modules.


📄 Publication

📖 This project was published in the International Journal of Novel Research and Development (IJNRD), Volume 9, Issue 11, November 2024:
“WiFi Based Digital LED Scrolling Notice Board.”

👩‍💻 Author

Shakthi Oviya P (2022504303)
🎓 MIT, Anna University | ECE Final Year

This project showcases the power of IoT and embedded systems by enabling real-time, remote communication through a low-cost and scalable smart notice board.

