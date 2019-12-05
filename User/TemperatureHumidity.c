//
//  TemperatureHumidity.c
//  OneNetMini
//
//  Created by HaiLong Yang on 2019/12/4.
//  Copyright © 2019 com.dragonlis. All rights reserved.
//

#include "TemperatureHumidity.h"
#include "macro.h"
#include "i2c.h"

float temperatureValue;
const int16_t POLYNOMIAL = 0x131;

/**
 * 从温湿度传感器获取最新的温度值
 *
 * @return 成功返回0 失败返回-1
 */
int8_t getTemperature(void) {
    uint8_t cmd = SHT20_Measurement_T_HM;
    HAL_I2C_Master_Transmit(&hi2c2, SHT20_ADDRESS, &cmd, 1, 10);
    msleep(1);
    char rTemperature[3];
    HAL_I2C_Master_Receive(&hi2c2, SHT20_ADDRESS, (uint8_t*)rTemperature, 3, 10);
    if (SHT2x_CheckCrc(rTemperature, 2, rTemperature[2]) != 0) {
        return -1;
    }
    uint16_t srcValue = (rTemperature[0] << 8) | rTemperature[1];
    temperatureValue = SHT2x_CalcTemperatureC(srcValue);
    
    return 0;
}

float getHumidity() {
    return 0;
}

/*
 ************************************************************
 *    函数名称：    SHT2x_CheckCrc
 *
 *    函数功能：    检查数据正确性
 *
 *    入口参数：    data：读取到的数据
 *                nbrOfBytes：需要校验的数量
 *                checksum：读取到的校对比验值
 *
 *    返回参数：    校验结果
 *
 *    说明：        0-成功        1-失败
 ************************************************************
 */
char SHT2x_CheckCrc(char data[], char nbrOfBytes, char checksum) {
    
    char crc = 0;
    char bit = 0;
    uint8_t byteCtr = 0;
    
    //calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
    {
        crc ^= (data[byteCtr]);
        for ( bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else crc = (crc << 1);
        }
    }
    
    if(crc != checksum)
        return 1;
    else
        return 0;
    
}

/*
 ************************************************************
 *    函数名称：    SHT2x_CalcTemperatureC
 *
 *    函数功能：    温度计算
 *
 *    入口参数：    u16sT：读取到的温度原始数据
 *
 *    返回参数：    计算后的温度数据
 *
 *    说明：
 ************************************************************
 */
float SHT2x_CalcTemperatureC(uint16_t u16sT) {
    
    float temperatureC = 0;            // variable for result
    
    u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
    //-- calculate temperature [癈] --
    temperatureC = -46.85 + 175.72 / 65536 * (float)u16sT; //T= -46.85 + 175.72 * ST/2^16
    
    return temperatureC;
}
