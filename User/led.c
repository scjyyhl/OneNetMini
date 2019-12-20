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
#include <stdlib.h>

#define mled_Delay    100

uint16_t LedPos = GPIO_PIN_6;
GPIO_PinState LedOn = GPIO_PIN_RESET;

#define LED_CONTROL(pos, on)    HAL_GPIO_WritePin(GPIOB, pos, on)

#define LED_ON      GPIO_PIN_RESET
#define LED_OFF     GPIO_PIN_SET

#define WORK_LED    Led_Green_Pin
uint16_t workLedState = 0x1FE;

#define WIFI_LED    Led_Yellow_Pin
GPIO_PinState wifiLedState = LED_OFF;

void runLedTwinkle(void) {
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

void ledTwinkle(void) {
    if (LedPos > GPIO_PIN_9) {
        LedPos = GPIO_PIN_6;
        LedOn = LedOn == GPIO_PIN_RESET ? GPIO_PIN_SET : GPIO_PIN_RESET;
    }
    LED_CONTROL(LedPos, LedOn);
    LedPos = LedPos << 1;
}

void ledMainWork(void) {
    LED_CONTROL(WORK_LED, workLedState & 0x1);
    workLedState = ((workLedState >> 1) | workLedState << 15);
}

void ledWifiWork(void) {
    LED_CONTROL(WIFI_LED, wifiLedState);
    wifiLedState = wifiLedState == LED_OFF ? LED_ON : LED_OFF;
}

void sleepForWifi(int16_t util) {
    wifiLedState = LED_ON;
    while (--util < 0) {
        ledWifiWork();
        ledMainWork();
        msleep(100);
    }
    LED_CONTROL(WIFI_LED, LED_OFF);
    ledMainWork();
}
