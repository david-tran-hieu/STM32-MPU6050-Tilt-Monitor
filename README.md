# STM32 MPU6050 Tilt Monitor

An embedded systems project that uses an STM32 microcontroller, an MPU6050 6-axis motion sensor, and an OLED display to monitor tilt angles in real time.

The firmware is written in C using STM32 HAL libraries and is configured for development with Keil MDK-ARM.

## Features

- **MPU6050 communication:** Read accelerometer and gyroscope measurements through I2C.
- **Tilt angle estimation:** Calculate Roll and Pitch angles using accelerometer data.
- **Complementary filter:** Combine accelerometer-based angles with gyroscope measurements to improve angle estimation.
- **OLED display:** Display Roll, Pitch, and tilt status.
- **LED warning:** Activate the LED connected to GPIO PA8 when the absolute Roll or Pitch angle exceeds 20 degrees.
- **Error handling:** Display sensor initialization and communication errors on the OLED.

## Hardware Requirements

- STM32F103xB-series microcontroller board (confirm the exact device in the Keil project settings)
- MPU6050 accelerometer and gyroscope module
- I2C-compatible OLED display
- USB-to-UART adapter or ST-Link programmer/debugger, as appropriate
- Jumper wires and a suitable power supply

## Software Requirements

- Keil MDK-ARM / Keil uVision
- STM32 HAL libraries
- CMSIS device support
- STM32CubeMX, if needed to modify the peripheral configuration

## How It Works

1. Initialize the STM32 HAL, system clock, GPIO, and I2C1 peripheral.
2. Initialize the OLED display.
3. Configure the MPU6050 by waking it from sleep and selecting the accelerometer and gyroscope measurement ranges.
4. Read accelerometer and gyroscope data from the MPU6050.
5. Calculate Roll and Pitch angles using accelerometer measurements and gyroscope integration, combined through a complementary filter.
6. Display the angles and status messages on the OLED.
7. Turn on the LED at PA8 when the absolute Roll or Pitch angle exceeds 20 degrees.
8. Repeat the measurement and display process approximately every 50 ms, subject to processing and communication time.

## Tilt Warning Logic

The firmware uses a 20-degree threshold:

- **NORMAL:** Both absolute Roll and Pitch angles are at or below 20 degrees.
- **TILT WARNING:** Either absolute angle exceeds 20 degrees.
- **MPU ERROR / CHECK MPU:** Displayed when sensor initialization or measurement fails.

The LED connected to PA8 is activated when a valid measurement exceeds the threshold.

## Building and Flashing

1. Clone or download this repository.
2. Open `MDK-ARM/Tilt Monitor.uvprojx` in Keil uVision.
3. Check the selected STM32 device, source files, include paths, and HAL/CMSIS dependencies.
4. Verify the I2C configuration and the actual wiring of the MPU6050 and OLED.
5. Build the project.
6. Connect the development board and flash the firmware.
7. Power on the board and observe the tilt measurements on the OLED.

## Notes

- The current firmware estimates Roll and Pitch; it does not implement a full sensor-fusion orientation system.
- Angle accuracy depends on sensor calibration, mounting orientation, noise, and the selected filter parameters.
- The tilt threshold is currently set to 20 degrees in the firmware.
- The exact I2C pins, OLED address, and display configuration should be verified against the complete source code and hardware setup.

## Author

GitHub: [hieuhk06-star](https://github.com/hieuhk06-star)

## License

No separate project license has been specified. Add a license file if you intend to define the terms under which others may use or redistribute this project.
