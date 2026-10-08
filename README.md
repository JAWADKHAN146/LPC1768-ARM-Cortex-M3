# LPC1768-ARM-Cortex-M3
A collection of my embedded systems projects, featuring source code developed using LPC1768
# Embedded Systems Projects (LPC1768)

A collection of embedded C programs written for the **NXP LPC1768 (ARM Cortex-M3)** microcontroller. The programs cover GPIO, LCD interfacing (4-bit parallel and I2C), ADC, UART, I2C, CAN,timer/counters and interfacing with sensors and communication modules.

All programs use the CMSIS device header (`LPC17xx.h`) and access the peripheral registers directly, without a vendor HAL.

## Contents

- [Hardware](#hardware)
- [Highlighted Projects](#highlighted-projects)
- [Getting Started](#getting-started)
- [Author](#author)

## Hardware

| Component | Used for |
|---|---|
| LPC1768 development board | Target microcontroller |
| 16x2 character LCD | Output display (4-bit parallel mode, and I2C mode through a PCF8574-style backpack) |
| LM35 temperature sensor | Analog temperature input read through the ADC |
| ADXL accelerometer | Motion/acceleration sensing over I2C |
| Ultrasonic sensor | Distance measurement (parking sensor) |
| RFID reader and cards | Card-based authentication over UART |
| GPS / GSM module | Location data over UART |
| LEDs, switches, buzzer, relay | Basic GPIO input/output |

## Highlighted Projects

### LM35 temperature monitor (`lcdtempadc.c`)
- Reads the LM35 output on ADC channel 0 (pin P0.23) with the ADC power-enabled and configured through `PCONP`, `PINSEL1` and `ADCR`.
- Converts the 12-bit ADC result to millivolts using a 3.3 V reference, then to degrees Celsius using the LM35's 10 mV/°C scale.
- Shows the result on a 16x2 LCD in 4-bit mode (data on P0.24 to P0.27, RS/RW/EN on P2.11 to P2.13).

### I2C LCD driver (`i2clcd.c`)
- Configures the **I2C0** peripheral for standard mode at 100 kHz (SDA0/SCL0 on P0.27/P0.28).
- Implements the START, STOP and write routines with status-register polling.
- Sends each LCD byte as two 4-bit nibbles to the I2C backpack at address `0x27`, using the backpack's RS and EN bits.

### Parking sensor over CAN (`ultrasonicCAN.tx.c`, `ultrasonicCANRx.c`)
- Two-node design: one node measures distance with the ultrasonic sensor and transmits it over CAN, and the other node receives it.

### RFID authentication and GPS tracking
- `rfidcard.c` reads a card ID over UART and checks it for access.
- `GPSJKWHILE.c` and `gsmcoordinats.c` receive and handle location data over UART.
### Many more

## Getting Started

1. Open your ARM toolchain or IDE (for example Keil uVision) and create a project for the **LPC1768**.
2. Add the CMSIS startup and system files (`LPC17xx.h`, `system_LPC17xx.c`) for the device.
3. Add one `.c` file from this repository as the project source. Each file has its own `main()`, so build **one program at a time**.
4. Wire the hardware as described in the pin comments at the top of each program, then build and flash it to the board.

> Each file is a standalone program. They are not meant to be compiled together.

## Author
**Jawad Khan**
B.Tech, Electronics and Communication Engineering, PES University
[LinkedIn](https://linkedin.com/in/jawad-khan-225a44355) | jawadkhann116@gmail.com
