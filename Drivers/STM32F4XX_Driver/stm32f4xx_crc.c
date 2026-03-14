/**
  ******************************************************************************
  * @file    stm32f4xx_crc.c
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   This file provides all the CRC firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_crc.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup CRC 
  * @brief CRC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CRC_Private_Functions
  * @{
  */

/**
  * @brief  Resets the CRC Data register (DR).
  * @param  None
  * @retval None
  */
/**
  * @brief  重置CRC数据寄存器（DR）。
  * @param  无
  * @retval 无
  */
void CRC_ResetDR(void)
{
  /* Reset CRC generator */
  CRC->CR = CRC_CR_RESET;
}

/**
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param  Data: data word(32-bit) to compute its CRC
  * @retval 32-bit CRC
  */
/**
  * @brief  计算给定数据字（32位）的32位循环冗余校验（CRC）。
  * @param  Data: 要计算其CRC的32位数据字
  * @retval 32位CRC
  */
uint32_t CRC_CalcCRC(uint32_t Data)
{
  CRC->DR = Data;
  
  return (CRC->DR);
}

/**
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  pBuffer: pointer to the buffer containing the data to be computed
  * @param  BufferLength: length of the buffer to be computed					
  * @retval 32-bit CRC
  */
/**
  * @brief  计算给定数据字（32位）缓冲区的32位循环冗余校验（CRC）。
  * @param  pBuffer：指向包含待计算数据的缓冲区的指针
  * @param  BufferLength：待计算缓冲区的长度					
  * @retval 32位循环冗余校验（CRC）
  */
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
  uint32_t index = 0;
  
  for(index = 0; index < BufferLength; index++)
  {
    CRC->DR = pBuffer[index];
  }
  return (CRC->DR);
}

/**
  * @brief  Returns the current CRC value.
  * @param  None
  * @retval 32-bit CRC
  */
/**
  * @brief  返回当前的CRC值。
  * @param  无
  * @retval 32位CRC
  */
uint32_t CRC_GetCRC(void)
{
  return (CRC->DR);
}

/**
  * @brief  Stores a 8-bit data in the Independent Data(ID) register.
  * @param  IDValue: 8-bit value to be stored in the ID register 					
  * @retval None
  */
/**
  * @brief  在独立数据（ID）寄存器中存储一个8位数据。
  * @param  IDValue：要存储在ID寄存器中的8位值
  * @retval 无
  */
void CRC_SetIDRegister(uint8_t IDValue)
{
  CRC->IDR = IDValue;
}

/**
  * @brief  Returns the 8-bit data stored in the Independent Data(ID) register
  * @param  None
  * @retval 8-bit value of the ID register 
  */
/**
  * @brief  返回存储在独立数据（ID）寄存器中的8位数据
  * @param  无
  * @retval ID寄存器的8位值
  */
uint8_t CRC_GetIDRegister(void)
{
  return (CRC->IDR);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
