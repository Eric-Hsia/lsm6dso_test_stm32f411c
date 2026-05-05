# LSM6DSOX/LSM6DSO Test

* [中文版本](./README_CN.md)
  
> LSM6DSOX Sensor Module

![LSM6DSOX sensor module 3D View](./images/LSM6DSOWTR-Top.png)

## 1 - Introduction

Behold, the ST LSM6DSOX/LSM6DSOW: The latest in a long line of quality Accelerometer+Gyroscope 6-DOF IMUs from ST.

This IMU sensor has 6 degrees of freedom - 3 degrees each of linear acceleration and angular velocity at varying rates within a respectable range. For the accelerometer: ±2/±4/±8/±16 g at 1.6 Hz to 6.7KHz update rate. For the gyroscope: ±125/±250/±500/±1000/±2000 dps at 12.5 Hz to 6.7 KHz. There's also some nice extras, such as built-in tap detection, activity detection, pedometer/step counter and a programmable finite state machine / machine learning(LSM6DSOX support) core that can perform some basic gesture recognition.

For interfacing, you can use either SPI or I2C - there are two configurable interrupt pins. For advanced usage, you can attach additional devices to an external I2C/SPI port - used for optical image stabilization.

## 2 - Compile and Run

### 2.1 Clone Repository

```git
git clone --recursive https://github.com/Eric-Hsia/lsm6dso_test_stm32f411c
```

#### Other Examples

Refer to the lsm6dsox_STdC related examples in the [STMems_Standard_C_drivers](https://github.com/STMicroelectronics/STMems_Standard_C_drivers) repository for more features and usage examples.

### 2.2 Install STM32CubeIDE for Visual Studio Code

Follow the instructions in the [STM32CubeIDE for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=stmicroelectronics.stm32-vscode-extension) marketplace page.

### 2.3 Adding floating-point support for snprintf(cmake)  

In the newlib-nano (lightweight C library) of STM32 (ARM Cortex-M), snprintf does not support floating-point number formatting (%f, %4.2f, etc.) by default.

```bash
target_link_options(${PROJECT_NAME} PRIVATE
    -u _printf_float
)
```

## 3 - References

* [LSM6DSOX](https://www.st.com/en/mems-and-sensors/lsm6dsox.html)

* [LSM6DSOX device application note](https://www.st.com/resource/en/application_note/DM00571818.pdf)

* [LSM6DSOX finite state machine](https://www.st.com/resource/en/application_note/DM00572971.pdf)

* [LSM6DSOX machine learning core](https://www.st.com/resource/en/application_note/DM00563460.pdf)
  