/**
 ******************************************************************************
 * @file           : led.c
 * @brief          : LED灯控制
 *                   
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019	Yanghl. All rights reserved.
 *
 ******************************************************************************
 */

#include "led.h"
#include "uuart.h"

#define mled_Delay    100

uint16_t LedPos = GPIO_PIN_6;
GPIO_PinState LedOn = GPIO_PIN_RESET;

#define LED_CONTROL(pos, on)    HAL_GPIO_WritePin(GPIOB, pos, on)

void runLedTwinkle() {
  while (1)
  {
    uprint("led twinkle.\n");
    LED_R_ON;
    msleep(mled_Delay);
    LED_G_ON;
    msleep(mled_Delay);
    LED_Y_ON;
    msleep(mled_Delay);
    LED_B_ON;
    msleep(mled_Delay);
    LED_R_OFF;
    msleep(mled_Delay);
    LED_G_OFF;
    msleep(mled_Delay);
    LED_Y_OFF;
    msleep(mled_Delay);
    LED_B_OFF;
    msleep(mled_Delay);
  }
}

void ledTwinkle() {
    if (LedPos > GPIO_PIN_9) {
        LedPos = GPIO_PIN_6;
        LedOn = LedOn == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }
    LED_CONTROL(LedPos, LedOn);
    LedPos = LedPos << 1;
}
