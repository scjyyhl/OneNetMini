//
//  ESP8266.h
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/6.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#ifndef ESP8266_h
#define ESP8266_h

#include <stdio.h>

#define REV_OK            0    // 接收完成标志
#define REV_TIMEOUT       1    // 接收未完成标志

#define SSID        "WLJS_MY_00"
#define PASSWORD    "wlmyxfd666"
#define WIFI_INFO   "AT+CWJAP=\""SSID"\",\""PASSWORD"\"\r\n"

extern volatile unsigned short esp8266_cnt;

void ESP8266_Init(void);
void ESP8266_getWiFiList(void);
void ESP8266_Clear(void);

char ESP8266_SendCmd(char *cmd, char *res, uint16_t timeout);
#define ESP8266_SendCmd_D(cmd, res)   ESP8266_SendCmd(cmd, res, 3000)
#define ESP8266_SendCmd_NOACK(cmd)    ESP8266_SendCmd(cmd, NULL, 0)

void ESP8266_DataRecved(void);
void ESP8266_DataRecvInit(void);
void ESP8266_DataRecvReInit(void);
void printBuffer(void);

void ESP8266_doHttpRequest(const char *ip, uint16_t port, const char *requestData, uint16_t requestDataLen, char *responseData, uint16_t responseLen);

#endif /* ESP8266_h */
