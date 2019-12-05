/**
 ******************************************************************************
 * @file           : macro.h
 * @brief          : 通用宏定义
 *                   
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019	Yanghl. All rights reserved.
 *
 ******************************************************************************
 */

#include "stm32f1xx_hal.h"

#ifndef sleep
#define sleep(sec)  HAL_Delay(sec * 1000)
#endif

#ifndef msleep
#define msleep(sec)  HAL_Delay(sec)
#endif

#ifndef MIN
#define MIN(x,y)    (x < y ? x : y)
#endif
