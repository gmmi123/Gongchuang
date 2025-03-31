/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_uart7_rx;
extern DMA_HandleTypeDef hdma_uart8_rx;
extern DMA_HandleTypeDef hdma_uart9_rx;
extern DMA_HandleTypeDef hdma_usart10_rx;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;
extern UART_HandleTypeDef huart10;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart10_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart9_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart7_rx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart8_rx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles FDCAN1 interrupt 0.
  */
void FDCAN1_IT0_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN1_IT0_IRQn 0 */

  /* USER CODE END FDCAN1_IT0_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan1);
  /* USER CODE BEGIN FDCAN1_IT0_IRQn 1 */

  /* USER CODE END FDCAN1_IT0_IRQn 1 */
}

/**
  * @brief This function handles FDCAN2 interrupt 0.
  */
void FDCAN2_IT0_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN2_IT0_IRQn 0 */

  /* USER CODE END FDCAN2_IT0_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan2);
  /* USER CODE BEGIN FDCAN2_IT0_IRQn 1 */

  /* USER CODE END FDCAN2_IT0_IRQn 1 */
}

/**
  * @brief This function handles FDCAN1 interrupt 1.
  */
void FDCAN1_IT1_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN1_IT1_IRQn 0 */

  /* USER CODE END FDCAN1_IT1_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan1);
  /* USER CODE BEGIN FDCAN1_IT1_IRQn 1 */

  /* USER CODE END FDCAN1_IT1_IRQn 1 */
}

/**
  * @brief This function handles FDCAN2 interrupt 1.
  */
void FDCAN2_IT1_IRQHandler(void)
{
  /* USER CODE BEGIN FDCAN2_IT1_IRQn 0 */

  /* USER CODE END FDCAN2_IT1_IRQn 0 */
  HAL_FDCAN_IRQHandler(&hfdcan2);
  /* USER CODE BEGIN FDCAN2_IT1_IRQn 1 */

  /* USER CODE END FDCAN2_IT1_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles UART7 global interrupt.
  */
void UART7_IRQHandler(void)
{
  /* USER CODE BEGIN UART7_IRQn 0 */
	if(__HAL_UART_GET_FLAG(&huart7, UART_FLAG_IDLE) != RESET)  // �����жϱ�Ǳ���λ
	{
	    __HAL_UART_CLEAR_IDLEFLAG(&huart7);  // ����жϱ��
	    HAL_UART_DMAStop(&huart7);           // ֹͣDMA����
	    UART7_RX_STA = UART7_RX_LEN - __HAL_DMA_GET_COUNTER(huart7.hdmarx);  // ����������ȥδ���յ���������Ϊ�Ѿ����յ���������
	    UART7_RX_BUF[UART7_RX_STA] = 0;  // ��ӽ�����
			memcpy(UART7_RX_Second_BUF, UART7_RX_BUF, UART7_RX_LEN);
			memset(UART7_RX_BUF, 0, sizeof(UART7_RX_BUF)); 
			//ʹ�û�����
			//���(����)


			//
	    UART7_RX_STA |= 0X8000;         // ��ǽ��ս���
	    HAL_UART_Receive_DMA(&huart7, UART7_RX_BUF, UART7_RX_LEN);  // ��������DMA����
	}
  /* USER CODE END UART7_IRQn 0 */
  HAL_UART_IRQHandler(&huart7);
  /* USER CODE BEGIN UART7_IRQn 1 */

  /* USER CODE END UART7_IRQn 1 */
}

/**
  * @brief This function handles UART8 global interrupt.
  */
void UART8_IRQHandler(void)
{
  /* USER CODE BEGIN UART8_IRQn 0 */
	if(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_IDLE) != RESET)  // �����жϱ�Ǳ���λ
	{
	    __HAL_UART_CLEAR_IDLEFLAG(&huart8);  // ����жϱ��
	    HAL_UART_DMAStop(&huart8);           // ֹͣDMA����
	    UART8_RX_STA = UART8_RX_LEN - __HAL_DMA_GET_COUNTER(huart8.hdmarx);  // ����������ȥδ���յ���������Ϊ�Ѿ����յ���������
	    UART8_RX_BUF[UART8_RX_STA] = 0;  // ��ӽ�����
			memcpy(UART8_RX_Second_BUF, UART8_RX_BUF, UART8_RX_LEN);
			memset(UART8_RX_BUF, 0, sizeof(UART8_RX_BUF)); 
			//ʹ�û�����
			//���(����)

			//
	    UART8_RX_STA |= 0X8000;         // ��ǽ��ս���
	    HAL_UART_Receive_DMA(&huart8, UART8_RX_BUF, UART8_RX_LEN);  // ��������DMA����
	}
  /* USER CODE END UART8_IRQn 0 */
  HAL_UART_IRQHandler(&huart8);
  /* USER CODE BEGIN UART8_IRQn 1 */

  /* USER CODE END UART8_IRQn 1 */
}

/**
  * @brief This function handles UART9 global interrupt.
  */
void UART9_IRQHandler(void)
{
  /* USER CODE BEGIN UART9_IRQn 0 */
	if(__HAL_UART_GET_FLAG(&huart9, UART_FLAG_IDLE) != RESET)  // �����жϱ�Ǳ���λ
	{
	    __HAL_UART_CLEAR_IDLEFLAG(&huart9);  // ����жϱ��
	    HAL_UART_DMAStop(&huart9);           // ֹͣDMA����
	    UART9_RX_STA = UART9_RX_LEN - __HAL_DMA_GET_COUNTER(huart9.hdmarx);  // ����������ȥδ���յ���������Ϊ�Ѿ����յ���������
		UART9_RX_BUF[UART9_RX_STA] = 0;  // ��ӽ�����
			
		if(UART9_flag==0)
		{
			memcpy(UART9_RX_Second_BUF, UART9_RX_BUF, UART9_RX_LEN);
		}
			
			
			memset(UART9_RX_BUF, 0, sizeof(UART9_RX_BUF)); 
			//ʹ�û�����
			//���(����)


			//
	    UART9_RX_STA |= 0X8000;         // ��ǽ��ս���
	    HAL_UART_Receive_DMA(&huart9, UART9_RX_BUF, UART9_RX_LEN);  // ��������DMA����
	}
  /* USER CODE END UART9_IRQn 0 */
  HAL_UART_IRQHandler(&huart9);
  /* USER CODE BEGIN UART9_IRQn 1 */

  /* USER CODE END UART9_IRQn 1 */
}

/**
  * @brief This function handles USART10 global interrupt.
  */
void USART10_IRQHandler(void)
{
  /* USER CODE BEGIN USART10_IRQn 0 */
	if(__HAL_UART_GET_FLAG(&huart10, UART_FLAG_IDLE) != RESET)  // �����жϱ�Ǳ���λ
	{
	    __HAL_UART_CLEAR_IDLEFLAG(&huart10);  // ����жϱ��
	    HAL_UART_DMAStop(&huart10);           // ֹͣDMA����
	    UART10_RX_STA = UART10_RX_LEN - __HAL_DMA_GET_COUNTER(huart10.hdmarx);  // ����������ȥδ���յ���������Ϊ�Ѿ����յ���������
	    UART10_RX_BUF[UART10_RX_STA] = 0;  // ��ӽ�����
			memcpy(UART10_RX_Second_BUF, UART10_RX_BUF, UART10_RX_LEN);
			memset(UART10_RX_BUF, 0, sizeof(UART10_RX_BUF)); 
			//ʹ�û�����
			//���(����)


			//
	    UART10_RX_STA |= 0X8000;         // ��ǽ��ս���
	    HAL_UART_Receive_DMA(&huart10, UART10_RX_BUF, UART10_RX_LEN);  // ��������DMA����
	}

  /* USER CODE END USART10_IRQn 0 */
  HAL_UART_IRQHandler(&huart10);
  /* USER CODE BEGIN USART10_IRQn 1 */

  /* USER CODE END USART10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
