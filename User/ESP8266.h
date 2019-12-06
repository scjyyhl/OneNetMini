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

#define REV_OK        0    // 接收完成标志
#define REV_WAIT      1    // 接收未完成标志

void ESP8266_Clear(void);
char ESP8266_SendCmd(char *cmd, char *res);
char ESP8266_WaitRecive(void);

#endif /* ESP8266_h */
