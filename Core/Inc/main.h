/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HWT101_RX_Pin GPIO_PIN_2
#define HWT101_RX_GPIO_Port GPIOE
#define HWT101_TX_Pin GPIO_PIN_3
#define HWT101_TX_GPIO_Port GPIOE
#define CAN2_power_Pin GPIO_PIN_13
#define CAN2_power_GPIO_Port GPIOC
#define Power1_Pin GPIO_PIN_14
#define Power1_GPIO_Port GPIOC
#define Power2_Pin GPIO_PIN_15
#define Power2_GPIO_Port GPIOC
#define ACC_CS_Pin GPIO_PIN_0
#define ACC_CS_GPIO_Port GPIOC
#define GYRO_CS_Pin GPIO_PIN_3
#define GYRO_CS_GPIO_Port GPIOC
#define xianweikaigan_Pin GPIO_PIN_9
#define xianweikaigan_GPIO_Port GPIOE
#define ACC_INT_Pin GPIO_PIN_10
#define ACC_INT_GPIO_Port GPIOE
#define GYRO_INT_Pin GPIO_PIN_12
#define GYRO_INT_GPIO_Port GPIOE
#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */


//开始：串口DMA空闲中断配置

#define UART10_RX_LEN 32   //字节
extern uint8_t UART10_RX_BUF[UART10_RX_LEN];       //缓存一
extern uint8_t UART10_RX_Second_BUF[UART10_RX_LEN];//缓存二
extern __IO uint16_t UART10_RX_STA;              //字节数

#define UART9_RX_LEN 32   //字节
extern uint8_t UART9_RX_BUF[UART9_RX_LEN];       //缓存一
extern uint8_t UART9_RX_Second_BUF[UART9_RX_LEN];//缓存二
extern __IO uint16_t UART9_RX_STA;              //字节数

#define UART7_RX_LEN 32   //字节
extern uint8_t UART7_RX_BUF[UART7_RX_LEN];       //缓存一
extern uint8_t UART7_RX_Second_BUF[UART7_RX_LEN];//缓存二
extern __IO uint16_t UART7_RX_STA;              //字节数


#define UART8_RX_LEN 32   //字节
extern uint8_t UART8_RX_BUF[UART8_RX_LEN];       //缓存一
extern uint8_t UART8_RX_Second_BUF[UART8_RX_LEN];//缓存二
extern __IO uint16_t UART8_RX_STA;              //字节数

extern char UART9_flag;
//结束
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
