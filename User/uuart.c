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
#include "stm32f103xb.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LEN 512

USART_RecvCallback usart1Callback;
USART_RecvCallback usart2Callback;

#define U1_BUF_SIZE   7
unsigned char uart1_Buffer[U1_BUF_SIZE + 1];

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
    data[size++] = '\n';
    HAL_UART_Transmit(&huart1, data, size, 100);
}

void uuart1_RecvInit(void) {
//    HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart1, uart1_Buffer, 5);
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&huart1, uart1_Buffer, 1);
    if (status != HAL_OK) {
        uprintf("HAL_UART_Receive_IT uart1 error. status = %d ", status);
    }
    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        HAL_UART_Transmit(&huart1, uart1_Buffer, 1, 10);
    }
    else if (huart->Instance == USART2) {
        if (usart2Callback) {
            usart2Callback();
        }
    }
    else {
        uprintln("HAL_UART_RxCpltCallback Other");
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        uprintln("HAL_UART_ErrorCallback USART1");
    }
}
