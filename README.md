# Smart Home Project

This project was developed as the graduation project for our Embedded Systems diploma. As a team, we designed and implemented a **Smart Home system** using the ATMEGA32A microcontroller. The project integrates various modules to demonstrate a modern, automated home setup.

## Table of Contents
- [About](#about)
- [Features](#features)
- [Tools and Components](#tools-and-components)
- [Installation](#installation)
- [Usage](#usage)
- [Team](#team)
- [Demo](#demo)
- [Contact](#contact)

## About

The **Smart Home Project** utilizes the ATMEGA32A microcontroller to create a system capable of automating home appliances and monitoring environmental conditions. The system integrates various modules for input, output, and communication to offer a comprehensive solution for smart home control.

## Features

- **Two Modes**: Admin mode and User mode, with secure login functionality.
- **Temperature Monitoring**: Using an LM35 temperature sensor.
- **Lighting Control**: Includes LEDs and a dimmer circuit.
- **Device Control**: Operates devices via relay, servo motor, and DC motor.
- **User Interface**: Controlled via 16x2 LCD and 4x4 Keypad.
- **Bluetooth Connectivity**: Enables remote control of the system.

## Tools and Components

- **ATmega Evaluation KIT System**:
  - General Input/Output Module
  - SPI Programmer Module
  - I2C EEPROM Module
  - A/D Converter Module
  - Triple Pushbuttons Module
  - 16x2 Character LCD Module
  - Two-Digit Display Module
  - 3-Bit LED Module
  - Buzzer Module
  - Relay Module
  - TivaC Compatible Connector
- **External Components**:
  - LCD 16x2
  - 4x4 Keypad
  - Bluetooth Module
  - Dimmer Circuit
  - LEDs (x5)
  - Servo Motor
  - DC Motor
  - LM35 Temperature Sensor

## Installation
**Schematic**:
![Screenshot 2024-11-26 145122](https://github.com/user-attachments/assets/7a67eb5e-e9d9-4e18-9580-40eb8b4f1ce6) 

1. Assemble the hardware according to the past schematic.  
2. Flash the firmware to the ATMEGA32A microcontroller using an SPI programmer.  
3. Power on the system and follow the usage instructions.  

## Usage


1. **Login**:  
   - The system requires a secure login via the **4x4 Keypad**.  
   - Admins access additional privileges compared to users.  

2. **Modes**:  
   - **User Mode**:  
     - View room temperature using the LM35 sensor.  
     - Control the AC power.  
     - Toggle house lamps ON/OFF.  
     - Adjust lighting levels with the dimmer circuit.  
   - **Admin Mode**:  
     - Access all User Mode features.  
     - Additional privileges, such as controlling the door via the servo motor.  

3. **Bluetooth Control**:  
   - Once logged in, use the Bluetooth Module to interact with components remotely.  

## Team

The following team members developed this project:
1. Hussin Hesham Ali - Hussinhisham79@yahoo.com 
2. Mostafa Ehab Gamal Farag  -  sasa.e.afifi@gmail.com
3. Ayman Mohamed Mohamed  -  aymanmohamed3313@outlook.com
4. Mahmoud Mohamed Salah  - mahmoudms2003@gmail.com
5. Ziad Refaie Abu Alftooh  - zaidrar200@gmail.com

## Demo

🎥 A video demonstration of the Smart Home system will be uploaded soon. Stay tuned!

