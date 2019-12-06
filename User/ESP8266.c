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

unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

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
    
    ESP8266_Clear();
    
    while (ESP8266_SendCmd("AT\r\n", "OK")) {
        msleep(500);
    }
    uprintln("esp8266 AT.");
    
    while (ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK")) {
        msleep(500);
    }
    uprintln("esp8266 set work mode is Station mode.");
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
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

//==========================================================
//    函数名称：    ESP8266_SendCmd
//
//    函数功能：    发送命令
//
//    入口参数：    cmd：命令
//                res：需要检查的返回指令
//
//    返回参数：    0-成功    1-失败
//
//    说明：
//==========================================================
char ESP8266_SendCmd(char *cmd, char *res) {
    unsigned char timeOut = 200;
    
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, strlen(cmd), 1000);
    while(timeOut--) {
        if(ESP8266_WaitRecive() == REV_OK) {                      // 如果收到数据
            if(strstr((const char *)esp8266_buf, res) != NULL) {  // 如果检索到关键词
                ESP8266_Clear();                                  // 清空缓存
                return 0;
            }
        }
        msleep(10);
    }
    return 1;
}

//==========================================================
//    函数名称：    ESP8266_WaitRecive
//
//    函数功能：    等待接收完成
//
//    入口参数：    无
//
//    返回参数：    REV_OK-接收完成        REV_WAIT-接收超时未完成
//
//    说明：        循环调用检测是否接收完成
//==========================================================
char ESP8266_WaitRecive(void) {
    if(esp8266_cnt == 0) {                  // 如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
        return REV_WAIT;
    }
    if(esp8266_cnt == esp8266_cntPre) {     // 如果上一次的值和这次相同，则说明接收完毕
        esp8266_cnt = 0;                    // 清0接收计数
        return REV_OK;                      // 返回接收完成标志
    }
    
    esp8266_cntPre = esp8266_cnt;           // 置为相同
    return REV_WAIT;                        // 返回接收未完成标志
}
