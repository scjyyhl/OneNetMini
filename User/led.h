/**
 ******************************************************************************
 * @file           : led.h
 * @brief          : LED控制头文件
 *                   
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019	Yanghl. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef led_h
#define led_h

#include "gpio.h"
#include "macro.h"
#include <stm32f103xb.h>

#define LED_ON HAL_GPIO_WritePin(GPIOB, Led_Blue_Pin|Led_Yellow_Pin|Led_Green_Pin|Led_Red_Pin, GPIO_PIN_RESET)
#define LED_OFF HAL_GPIO_WritePin(GPIOB, Led_Blue_Pin|Led_Yellow_Pin|Led_Green_Pin|Led_Red_Pin, GPIO_PIN_SET)

#define LED_R_ON HAL_GPIO_WritePin(GPIOB, Led_Red_Pin, GPIO_PIN_RESET)
#define LED_R_OFF HAL_GPIO_WritePin(GPIOB, Led_Red_Pin, GPIO_PIN_SET)

#define LED_G_ON HAL_GPIO_WritePin(GPIOB, Led_Green_Pin, GPIO_PIN_RESET)
#define LED_G_OFF HAL_GPIO_WritePin(GPIOB, Led_Green_Pin, GPIO_PIN_SET)

#define LED_B_ON HAL_GPIO_WritePin(GPIOB, Led_Blue_Pin, GPIO_PIN_RESET)
#define LED_B_OFF HAL_GPIO_WritePin(GPIOB, Led_Blue_Pin, GPIO_PIN_SET)

#define LED_Y_ON HAL_GPIO_WritePin(GPIOB, Led_Yellow_Pin, GPIO_PIN_RESET)
#define LED_Y_OFF HAL_GPIO_WritePin(GPIOB, Led_Yellow_Pin, GPIO_PIN_SET)

void runLedTwinkle();
void ledTwinkle();

#endif
