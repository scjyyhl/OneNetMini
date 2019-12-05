//
//  TemperatureHumidity.h
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/4.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#ifndef TemperatureHumidity_h
#define TemperatureHumidity_h

#include <stdio.h>

extern float temperatureValue;

int8_t getTemperature(void);

char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum);
float SHT2x_CalcTemperatureC(uint16_t u16sT);

#endif /* TemperatureHumidity_h */
