# LSM6DSOX/LSM6DSOW Test

* [中文版本](./README_CN.md)
  
> LSM6DSOW Sensor Module

![LSM6DSOW sensor module 3D View](./images/LSM6DSOWTR-Top.png)

## 1 - Introduction

Behold, the ST LSM6DSOX/LSM6DSOW: The latest in a long line of quality Accelerometer+Gyroscope 6-DOF IMUs from ST.

This IMU sensor has 6 degrees of freedom - 3 degrees each of linear acceleration and angular velocity at varying rates within a respectable range. For the accelerometer: ±2/±4/±8/±16 g at 1.6 Hz to 6.7KHz update rate. For the gyroscope: ±125/±250/±500/±1000/±2000 dps at 12.5 Hz to 6.7 KHz. There's also some nice extras, such as built-in tap detection, activity detection, pedometer/step counter and a programmable finite state machine / machine learning(LSM6DSOX support) core that can perform some basic gesture recognition.

For interfacing, you can use either SPI or I2C - there are two configurable interrupt pins. For advanced usage, you can attach additional devices to an external I2C/SPI port - used for optical image stabilization.

## 2 - References

* [LSM6DSO](https://www.st.com/en/mems-and-sensors/lsm6dso.html)
  