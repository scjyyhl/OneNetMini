/**
 ******************************************************************************
 * @file           : uuart.c
 * @brief          : 串口相关的操作
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019	Yanghl. All rights reserved.
 *
 ******************************************************************************
 */

#include "uuart.h"
#include "usart.h"
#include "macro.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LEN 511

void uprintf(const char *fmt,...) {
    va_list ap;
    int16_t pSize;
    char printfBuf[MAX_LEN + 1] = {0};
    // format string
    va_start(ap, fmt);
    pSize = vsnprintf(printfBuf, MAX_LEN, fmt, ap);
    va_end(ap);
    if (pSize < 0) {
        memcpy(printfBuf, "vsnprintf return -1", 19);
        pSize = 19;
    }
    // add new line char
    pSize = MIN(pSize, MAX_LEN);
    printfBuf[pSize++] = '\n';
    
    HAL_UART_Transmit(&huart1, (uint8_t *)printfBuf, pSize, 50);
}

void uprint(const char *msg) {
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
}

void uprintln(const char *msg) {
    uint8_t *data;
    uint16_t size = strlen(msg);
    
    data = malloc(size + 1);
    memcpy(data, msg, size);
    data[++size] = '\n';
    HAL_UART_Transmit(&huart1, data, size, 100);
}

