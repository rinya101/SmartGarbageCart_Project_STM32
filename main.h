#include "stm32f4xx.h"
#include "SysClockConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_usart.h"
#include <string.h>
#include "bsp_encoder.h"
#include "PeripheralParamConfig.h"
#include "SysBspInit.h"


extern usart_handle_t* g_usart1_debug_handle;