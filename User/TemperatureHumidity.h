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
extern float humidityValue;

char getValue();

char getTemperature(void);
char getHumidity();
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum);
float SHT2x_CalcTemperatureC(uint16_t u16sT);
float SHT2x_CalcRH(uint16_t u16sRH);
void SHT20_reset(void);

#endif /* TemperatureHumidity_h */
