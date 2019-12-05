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

#define mled_Delay    150

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
