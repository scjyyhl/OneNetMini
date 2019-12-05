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
#include "uuart.h"

float temperatureValue = 0.0;
float humidityValue = 0.0;
const int16_t POLYNOMIAL = 0x131;

/**
 * 从温湿度传感器获取最新的温度值
 *
 * @return 成功返回0 失败返回-1
 */
char getTemperature(void) {
    uint16_t srcValue;
    uint8_t cmd;
    HAL_StatusTypeDef status;
    char rTemperature[3] = {0};
    
    cmd = SHT20_Measurement_T_HM;
//    status = HAL_I2C_Master_Transmit(&hi2c2, SHT20_ADDRESS, &cmd, 1, 1000);
//    if (status != HAL_OK) {
//        uprintf("HAL_I2C_Master_Transmit error! status = %d \n", status);
//    }
//    status = HAL_I2C_Master_Receive(&hi2c2, SHT20_ADDRESS, (uint8_t*)rTemperature, 3, 1000);
//    if (status != HAL_OK) {
//        uprintf("HAL_I2C_Master_Receive error! status = %d \n", status);
//    }
    status = HAL_I2C_Mem_Read(&hi2c2, SHT20_ADDRESS, cmd, 1, (uint8_t*)rTemperature, 3, 0xffff);
    if (status != HAL_OK) {
        uprintf("HAL_I2C_Mem_Read error! status = %d \n", status);
    }
    
    if (SHT2x_CheckCrc(rTemperature, 2, rTemperature[2]) != 0) {
        uprint("i2c read temperature value crc faild.\n");
        return -1;
    }
    srcValue = (rTemperature[0] << 8) + rTemperature[1];
//    uprintf("get humidity original data is %X", srcValue);
    temperatureValue = SHT2x_CalcTemperatureC(srcValue);
    
    return 0;
}

char getHumidity() {
    uint16_t srcValue;
    uint8_t cmd;
    HAL_StatusTypeDef status;
    char rHumidity[3] = {0};
    
    cmd = SHT20_Measurement_RH_HM;
    status = HAL_I2C_Mem_Read(&hi2c2, SHT20_ADDRESS, cmd, I2C_MEMADD_SIZE_8BIT, (uint8_t*)rHumidity, 3, 0xffff);
    if (status != HAL_OK) {
        uprintf("HAL_I2C_Mem_Read error! status = %d \n", status);
    }
    
    if (SHT2x_CheckCrc(rHumidity, 2, rHumidity[2]) != 0) {
        uprint("i2c read humidity value crc faild.\n");
        return -1;
    }
    srcValue = (rHumidity[0] << 8) + rHumidity[1];
    uprintf("get humidity original data is %X", srcValue);
    humidityValue = SHT2x_CalcRH(srcValue);
    
    return 0;
}

char getValue() {
    getTemperature();
    msleep(50);
    getHumidity();
    
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

/*
 ************************************************************
 *    函数名称：    SHT2x_CalcRH
 *
 *    函数功能：    湿度计算
 *
 *    入口参数：    u16sRH：读取到的湿度原始数据
 *
 *    返回参数：    计算后的湿度数据
 *
 *    说明：
 ************************************************************
 */
float SHT2x_CalcRH(uint16_t u16sRH)
{
    
    float humidityRH = 0;              // variable for result
    
    u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
    //-- calculate relative humidity [%RH] --
    //humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
    humidityRH = ((float)u16sRH * 0.00190735) - 6;
    
    return humidityRH;
    
}

/*
 ************************************************************
 *    函数名称：    SHT20_reset
 *
 *    函数功能：    SHT20复位
 *
 *    入口参数：    无
 *
 *    返回参数：    无
 *
 *    说明：
 ************************************************************
 */
void SHT20_reset(void)
{
    HAL_StatusTypeDef status;
    uint8_t cmd = SHT20_SOFT_RESET;
    status = HAL_I2C_Master_Transmit(&hi2c2, SHT20_ADDRESS, &cmd, 1, 1000);
//    HAL_I2C_Mem_Write(&hi2c2, SHT20_ADDRESS, SHT20_SOFT_RESET, I2C_MEMADD_SIZE_8BIT,(void*)0, 1, 1000);
    if (status != HAL_OK) {
        uprintf("HAL_I2C_Master_Transmit error! status = %d \n", status);
    }
    msleep(100);
}
