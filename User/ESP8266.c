//
//  ESP8266.c
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/6.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#include "ESP8266.h"
#include "main.h"
#include "macro.h"
#include "usart.h"
#include "uuart.h"
#include "led.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include <string.h>

#define ESP8266BUF_SIZE 511
unsigned char esp8266_buf[ESP8266BUF_SIZE + 1];
typedef enum
{
    ESP8266_CMD_COMMON      = 0x00,
    ESP8266_CMD_CWLAP       = 0x01
} ESP8266_CMD_Type;
ESP8266_CMD_Type esp8266CmdType = ESP8266_CMD_COMMON;
uint16_t currentBufferSize = ESP8266BUF_SIZE;

char **wifiList;

/**
 * 初始化esp8266芯片
 */
void ESP8266_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    sleepForWifi(3);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    sleepForWifi(5);
    
    usart2Callback = ESP8266_DataRecved;
    ESP8266_DataRecvInit();
    
    while (ESP8266_SendCmd_D("AT\r\n", "OK")) {
        uprintln("ESP8266 AT CMD NOT OK.");
        sleepForWifi(5);
    }
    uprintln("esp8266 AT.");
    
    while (ESP8266_SendCmd_D("AT+CWMODE=1\r\n", "OK")) {
        sleepForWifi(5);
    }
    uprintln("esp8266 set work mode is Station mode.");
    
    while (ESP8266_SendCmd_D(WIFI_INFO, "GOT IP")) {
        sleepForWifi(5);
        uprintln("connect faild. try again.");
    }
    
    while (ESP8266_SendCmd_D("AT+CIFSR\r\n", "OK")) {
        sleepForWifi(5);
    }
    sleepForWifi(11);
    uprintln((const char *)esp8266_buf);
}

/**
 * 获取当前的WiFi列表
 */
void ESP8266_getWiFiList(void) {
    const char *cmd = "AT+CWLAP\r\n";
    esp8266CmdType = ESP8266_CMD_CWLAP;
    ESP8266_SendCmd_NOACK((char*)cmd);
}

void printBuffer(void) {
    uprint((char*)esp8266_buf);
}

void doWiFiInfoData(void) {
    
}

//==========================================================
//    函数名称：    ESP8266_Clear
//
//    函数功能：    清空缓存
//
//    入口参数：    无
//
//    返回参数：    无
//
//    说明：
//==========================================================
void ESP8266_Clear(void) {
    memset(esp8266_buf, 0, ESP8266BUF_SIZE + 1);
}

//==========================================================
//    函数名称：    ESP8266_SendCmd
//
//    函数功能：    发送命令
//
//    入口参数：    cmd：命令
//                res：需要检查的返回指令
//                timeout: 等待返回指令的超时时间
//
//    返回参数：    0-成功    1-失败
//
//    说明：
//==========================================================
char ESP8266_SendCmd(char *cmd, char *res, uint16_t timeout) {
    HAL_StatusTypeDef status;
    ESP8266_DataRecvReInit();
    status = HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), 1000);
    if (status != HAL_OK) {
        uprintf("ESP8266_SendCmd error, status = %d ", status);
    }
    if (res == NULL) {
        return 0;
    }
    while(timeout > 0) {                      // 如果收到数据
        if(strstr((const char *)esp8266_buf, res) != NULL) {  // 如果检索到关键词
            uprintln((const char *)esp8266_buf);
            return 0;
        }
        msleep(10);
        timeout -= 10;
    }
    uprintln((const char *)esp8266_buf);
    return 1;
}

void ESP8266_DataRecved(void) {
    uprintln("ESP8266_DataRecved");
    ESP8266_DataRecvInit();
}

void ESP8266_DataRecvInit_Buf(uint8_t *buf, uint16_t bufLen) {
    HAL_StatusTypeDef status;
    currentBufferSize = bufLen;
    do {
        status = HAL_UART_Receive_IT(&huart2, buf, bufLen);
        if (status != HAL_OK) {
            uprintf("HAL_UART_Receive_IT error. status = %d ", status);
            sleepForWifi(5);
        }
    } while (status != HAL_OK);
}

void ESP8266_DataRecvInit(void) {
    ESP8266_Clear();
    ESP8266_DataRecvInit_Buf(esp8266_buf, ESP8266BUF_SIZE);
}

void ESP8266_DataRecvReInit_Buf(uint8_t *buf, uint16_t bufLen) {
    HAL_StatusTypeDef status;
    do {
        status = HAL_UART_AbortReceive_IT(&huart2);
        if (status != HAL_OK) {
            uprintf("HAL_UART_AbortReceive_IT error. status = %d ", status);
            sleepForWifi(5);
        }
    } while (status != HAL_OK);
    msleep(10);
    ESP8266_DataRecvInit_Buf(buf, bufLen);
}

void ESP8266_DataRecvReInit(void) {
    ESP8266_Clear();
    ESP8266_DataRecvReInit_Buf(esp8266_buf, ESP8266BUF_SIZE);
}

void ESP8266_TcpConnect(const char *ip, uint16_t port) {
    char tempCmd[54];
    sprintf(tempCmd, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip, port);
    while(ESP8266_SendCmd_D(tempCmd, "CONNECT") == 1) {
        sleepForWifi(5);
    }
}

void ESP8266_SendData(uint8_t *data, uint16_t len) {
    char cmdBuf[32];
    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);
    while(ESP8266_SendCmd_D(cmdBuf, ">")) {
        sleepForWifi(11);
    }
    HAL_UART_Transmit(&huart2, data, len, 10 * len);
}

void ESP8266_doHttpRequest(const char *ip, uint16_t port, const char *requestData, uint16_t requestDataLen, char *responseData, uint16_t responseLen) {
    ESP8266_TcpConnect(ip, port);
    ESP8266_SendData((uint8_t*)requestData, requestDataLen);
    ESP8266_DataRecvReInit_Buf((uint8_t*)responseData, responseLen);
    uprintln(requestData);
}
