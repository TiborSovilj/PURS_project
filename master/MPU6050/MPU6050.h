#ifndef _MPU6050_
#define _MPU6050_

#include "mbed.h"
#define ADDRESS 0x68

void initialState();
float accelReading();
float gyroReading();

//  void printData();

#endif