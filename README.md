# 🎮 Interactive PCB Business Card & Pocket Console

## 📖 Overview
This project is a fully functional, credit-card-sized (53x85 mm) interactive business card that doubles as a minimalist handheld game console. Powered by an STM32 (prototype STM32F411RE, final version STM32C562KET) microcontroller and a LiPo battery, it was designed from scratch in KiCad to showcase complete hardware-software co-design. 

<img width="300"  alt="retroGameCard_Front" src="https://github.com/user-attachments/assets/17681c40-c87e-4877-8dce-67af8a3b0631" />

The goal of this project was to merge aesthetics with robust embedded engineering, tackling challenges such as mechanical constraints, power path optimization, and bare-metal/RTOS firmware development.

## ⚙️ Hardware Architecture
The board is designed with a focus on stability and signal integrity, featuring a sleek matte black soldermask and hidden testing interfaces.

* **Microcontroller:** STM32 series (prototype STM32F411RE, final version STM32C562KET), managing game logic and peripheral communication.
* **Display:** I2C OLED screen for high-contrast, low-power graphics.
* **Power Management:** Custom power path featuring a LiPo charging IC and a Low-Dropout Regulator (LDO) optimized for maximum battery life (minimizing voltage dropout).
* **PCB Layout Highlights:**
  * Mechanical collision avoidance (precise component placement avoiding mounting holes).
  * Concealed bottom-layer SWD Test Points (SWDIO, SWCLK, GND, 3.3V) to maintain a clean aesthetic on the front panel.

## 💻 Firmware & Software
The firmware is written in **C**, focusing on hardware abstraction and efficient resource management.
* Custom made liblaries for menu and games.
* (Optional for future) Power-saving modes to prevent LiPo over-discharge.
* (Optional for future) RTOS implementation for task scheduling (e.g., separating rendering tasks from button input polling).


The view of the PCB

<img width="600"  alt="retroGameCard_Front" src="https://github.com/user-attachments/assets/17681c40-c87e-4877-8dce-67af8a3b0631" />

<img width="600"  alt="retroGameCard_Back" src="https://github.com/user-attachments/assets/2c10b8b1-eb60-4a5e-b3a9-96ff21d6ba4c" />

Schematic
[Schematic.pdf](https://github.com/user-attachments/files/29943936/Schematic.pdf)

Layout
[Layout front.pdf](https://github.com/user-attachments/files/29943942/Layout.front.pdf)
[Lauout back.pdf](https://github.com/user-attachments/files/29943943/Lauout.back.pdf)



Basic schematic of STM32C562KET6

<img width="300" height="300" alt="obraz" src="https://github.com/user-attachments/assets/bb8996ee-ce79-494b-9226-b3f72d0ef945" />


## 🗺️ System Block Diagram
```text
[ LiPo Battery ] --> [ Charger IC ] --> [ Low-Dropout (LDO) 3.3V ] 
                                                 |
                                                 v
[ User Buttons ] ------------------------> [ STM32 MCU ]
                                                 |
[ SWD Test Points ] <--------------------/       v
                                           [ OLED Display ]

