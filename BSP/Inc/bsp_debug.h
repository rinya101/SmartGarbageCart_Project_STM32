/*******************************************************************************
 *      文    件: bsp_debug.h
 *      说    明: 适用于STM32F4(具体为 STM32F411CEU6) 的 调试接口 (USART1)
 *      特别说明： 使用之前 注意配置 USART1 相关内容
 *      版    本: V1.0
 *      作    者: Rinya101(http://github.com/rinya101) 学号：220802040137
 *      版权说明：本程序代码仅用于 2026 本科毕业设计，不得用于其他商业用途，
 *               可以作为个人参考学习使用。
*******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H
#include "PeripheralParamConfig.h"
#include "stdio.h"


#if DEBUG == 1
    #define SGCS_ERROR(format, ...) \
        printf("[ERROR] " format " | file: %s | fun: %s | line: %d\r\n", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)

    #define SGCS_WARN(format, ...) \
        printf("[WARN]  " format " | file: %s | fun: %s | line: %d\r\n", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)

    #define SGCS_INFO(format, ...) \
        printf("[INFO]  " format " | file: %s | fun: %s | line: %d\r\n", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)

    #if DETAIL == 1
        #define SGCS_LOG(format, ...) \
            printf("[LOG]   " format " | file: %s | fun: %s | line: %d\r\n", ##__VA_ARGS__, __FILE__, __FUNCTION__, __LINE__)
    #else
        #define SGCS_LOG(...)
    #endif

#else
    #define SGCS_ERROR(...)
    #define SGCS_WARN(...)
    #define SGCS_INFO(...)
    #define SGCS_LOG(...)
#endif

#endif