
## Overview

This example demonstrates basic usage of I2C driver by running two tasks on I2C bus:

1. Read external I2C sensor (MPU6050).
2. Use one of ESP32’s I2C port (master mode) to read the sensor.

### Hardware Required

To run this example, you should have one ESP32 dev board (e.g. ESP32-WROVER Kit) or ESP32 core board (e.g. ESP32-DevKitC). Optionally, you can also connect an external sensor, here we choose the MPU6050 just for an example.

#### Pin Assignment:

**Note:** The following pin assignments are used by default, yout can change these  in the `menuconfig` .

|                  | SDA    | SCL    |
| ---------------- | ------ | ------ |
| ESP32 I2C Master | GPIO21 | GPIO22 |
| MPU6050 Sensor    | SDA    | SCL    |

- master:
  - GPIO21 is assigned as the data signal of I2C master port
  - GPIO22 is assigned as the clock signal of I2C master port

- Connection:
  - connect SDA/SCL of MPU6050 sensor with GPIO21/GPIO22

**Note: ** There’s no need to add an external pull-up resistors for SDA/SCL pin, because the driver will enable the internal pull-up resistors.

### Configure the project

Open the project configuration menu (`idf.py menuconfig`). Then go into `Example Configuration` menu.

- In the `I2C Master` submenu, you can set the pin number of SDA/SCL according to your board. Also you can modify the I2C port number and freauency of the master.
- In the `MPU6050 Sensor` submenu, you can choose the slave address of BH1750 accroding to the pin level of ADDR pin (if the pin level of ADDR is low then the address is `0x68`). 

### Build and Flash

Enter `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

### C files

There are two c files one is 
