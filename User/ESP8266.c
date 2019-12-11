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
#include "stm32f103xb.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include <string.h>

#define ESP8266BUF_SIZE 511
unsigned char esp8266_buf[ESP8266BUF_SIZE + 1];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
typedef enum
{
    ESP8266_CMD_COMMON      = 0x00,
    ESP8266_CMD_CWLAP       = 0x01
} ESP8266_CMD_Type;
ESP8266_CMD_Type esp8266CmdType = ESP8266_CMD_COMMON;

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
    msleep(250);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    msleep(500);
    
    usart2Callback = ESP8266_DataRecved;
    ESP8266_Clear();
    ESP8266_DataRecvInit();
    
    uprintln("ESP8266 AT begain.");
    while (ESP8266_SendCmd_D("AT\r\n", "OK")) {
        uprintln("ESP8266 AT CMD NOT OK.");
        msleep(500);
    }
    uprintln("esp8266 AT.");
    
    while (ESP8266_SendCmd_D("AT+CWMODE=1\r\n", "OK")) {
        msleep(500);
    }
    uprintln("esp8266 set work mode is Station mode.");
}

/**
 * 获取当前的WiFi列表
 */
void ESP8266_getWiFiList(void) {
    const char *cmd = "AT+CWLAP\r\n";
    esp8266CmdType = ESP8266_CMD_CWLAP;
    HAL_UART_Transmit(&huart1, (uint8_t*)cmd, strlen(cmd), 1000);
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
    esp8266_cnt = 0;
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
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), 1000);
    if (status != HAL_OK) {
        uprintf("ESP8266_SendCmd error, status = %d ", status);
    }
    if(ESP8266_WaitRecive(timeout) == REV_OK) {                      // 如果收到数据
        if(strstr((const char *)esp8266_buf, res) != NULL) {  // 如果检索到关键词
            ESP8266_Clear();                                  // 清空缓存
            return 0;
        }
    }
    return 1;
}

//==========================================================
//    函数名称：    ESP8266_WaitRecive
//
//    函数功能：    等待接收完成
//
//    入口参数：    timeout 等待超时时间 ms
//
//    返回参数：    REV_OK-接收完成        REV_WAIT-接收超时未完成
//
//    说明：        循环调用检测是否接收完成
//==========================================================
char ESP8266_WaitRecive(uint16_t timeout) {
    uint16_t mdelay = 10;
    
    timeout = timeout / mdelay;
    while (timeout-- > 0) {
        if(esp8266_cnt != 0 && esp8266_cnt == esp8266_cntPre) {     // 如果接收计数不为0，且和上一次的值相同，则说明接收完毕
            ESP8266_DataRecvInit();             // 重置数据接收
            return REV_OK;                      // 返回接收完成标志
        }
        esp8266_cntPre = esp8266_cnt;           // 置为相同
        msleep(mdelay);
    }
    return REV_TIMEOUT;
}

void ESP8266_DataRecved(void) {
    if (++esp8266_cnt > ESP8266BUF_SIZE) {
        esp8266_cnt = 0;
    }
    ESP8266_DataRecvInit();
}

void ESP8266_DataRecvInit(void) {
    HAL_StatusTypeDef status;
    do {
        status = HAL_UART_Receive_IT(&huart2, esp8266_buf + esp8266_cnt, 1);
        if (status != HAL_OK) {
            uprintf("HAL_UART_Receive_IT error. status = %d ", status);
            msleep(500);
        }
    } while (status == HAL_OK);
}

void ESP8266_DataRecvReInit(void) {
    HAL_StatusTypeDef status;
    do {
        status = HAL_UART_AbortReceive_IT(&huart2);
        if (status != HAL_OK) {
            uprintf("HAL_UART_AbortReceive_IT error. status = %d ", status);
            msleep(500);
        }
    } while (status == HAL_OK);
    ESP8266_Clear();
    ESP8266_DataRecvInit();
}
