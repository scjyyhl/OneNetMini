/**
 ******************************************************************************
 * @file           : uuart.h
 * @brief          : 串口相关的操作
 *                   
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019	Yanghl. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef uuart_h
#define uuart_h

typedef void(*USART_RecvCallback)(void);

extern USART_RecvCallback usart1Callback;
extern USART_RecvCallback usart2Callback;

/**
 * 通过串口格式化输出，末尾自动加换行
 */
void uprintf(const char *fmt,...);

/**
 * 通过串口输出，末尾不会自动加换行
 */
void uprint(const char *msg);

/**
 * 通过串口输出，末尾会自动加换行
 * @return 返回通过串口输出的字符串长度
 */
void uprintln(const char *msg);

void uuart1_RecvInit(void);
#endif
