# STM32 MPU6050 Tilt Monitor

An embedded systems project using the **STM32F103C8T6** microcontroller and the **MPU6050** motion sensor to measure tilt and orientation. The project is developed in C using Keil MDK-ARM and STM32 HAL libraries.

## Features

- Read accelerometer and gyroscope data from the MPU6050.
- Communicate with the sensor using I2C.
- Calculate tilt angles from accelerometer measurements.
- Display sensor data and tilt information on an OLED display.
- Use STM32 HAL drivers for peripheral configuration and communication.

## Hardware Requirements

- STM32F103C8T6 development board (Blue Pill)
- MPU6050 6-axis accelerometer and gyroscope module
- I2C-compatible OLED display
- USB-to-UART programmer or ST-Link debugger
- Jumper wires and a suitable power supply

## Software Requirements

- Keil MDK-ARM / Keil uVision
- STM32CubeMX (for project configuration, if needed)
- STM32 HAL and CMSIS libraries

## How It Works

1. The STM32 initializes the system clock and required peripherals.
2. The MPU6050 is accessed through the I2C interface.
3. Accelerometer measurements are used to estimate tilt angles.
4. The firmware processes the sensor data.
5. The OLED displays the measurements and orientation information.

## Getting Started

1. Clone or download this repository.
2. Open `MDK-ARM/Tilt Monitor.uvprojx` in Keil uVision.
3. Verify the device configuration, include paths, and required HAL/CMSIS libraries.
4. Connect the STM32 board, MPU6050, and OLED according to the project's actual pin configuration.
5. Build the project in Keil.
6. Flash the firmware to the STM32F103C8T6.
7. Power on the board and check the OLED output.

## Communication Interface

The MPU6050 typically communicates over I2C. The exact STM32 pins, OLED address, and sensor configuration depend on the firmware and hardware wiring used in this project.

## Future Improvements

- Add sensor calibration and filtering.
- Improve tilt estimation and display updates.
- Add configurable tilt thresholds and LED alerts.
- Document wiring with a circuit diagram and hardware photos.

## Author

**DavidTranHieu**

GitHub: https://github.com/hieuhk06-star

## License

No license has been specified yet. Please add a suitable license before allowing others to reuse or redistribute this project.
