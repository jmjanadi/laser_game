# Laser Game (in progress)

## Introduction
This project consists of the design for a game in which the user attempts to reach a destination as quickly as possible while avoiding detection by lasers. The project is developed in the C programming language on ARM Cortex-M0+ microcontrollers. The code consists of firmware that operates an LCD and US sensors. It also makes use of the Bluetooth Low Energy communication protocol to establish wireless connections between the multiple MCUs.

## Game Description
The game is played in the following manner:
1. The home board and laser boards are placed in the desired locations and turned on.
   - The laser boards measure and record the distance to the opposing wall/objects.
   - The home board and laser boards establish wireless connections.
4. The user pushes the button to begin.
5. The user has 10 seconds to move to their desired start position.
6. After the 10 seconds are up, the buzzer sounds, signaling that the timer has started and the game has begun.
7. The user attempts to reach the home board and push the button as quickly as possible without being detected by the lasers on the laser boards.
   - If a laser board detects the user, it sends a signal to the home board, the buzzer sounds, and the game is lost.
   - If the user reaches the home board without being detected, the timer stops and the game is won.

## Required Materials
- Development Kits (Texas Instruments LP-EM-CC2340R5)
  - CC2340R5 LaunchPad™ development kit for SimpleLink™ Bluetooth® 5.3 Low Energy MCU
  - Includes:
    - CC2340R5 — SimpleLink™ 32bit Arm® Cortex®-M0+ 2.4GHz wireless MCU with 512kB flash
    - CC2340R5-Q1 — Automotive-qualified SimpleLink™ Bluetooth® Low Energy wireless MCU with 512-kB flash
- Backlit LCD (NHD-0420H1Z-FL-GBW-33V3)
- Push Button
- Buzzer
- Ultrasonic Sensors (Adafruit 4007)
  - Measuring range: 2cm - 450cm
- Laser Diodes (Quarton Inc. VLM-650-03 LPT)
  - Visible red light (650nm)
  - Class 2 laser (1mW)
