#ifndef __Motor42_H
#define __Motor42_H
#include "main.h"
int Motor42_Enable(UART_HandleTypeDef *huart, uint8_t ID);
int Motor42_SpeedControl(UART_HandleTypeDef *huart, uint8_t Direction, uint16_t Speed, uint8_t Acc_Speed, uint8_t ID);
int Motor42_Stop(UART_HandleTypeDef *huart);
int	Motor42_PositionControl(UART_HandleTypeDef *huart, uint8_t Direction, uint16_t Speed, uint8_t Acc_Speed, uint32_t Size, uint8_t Mod, uint8_t ID);
int Motor42_ReadSpeed(UART_HandleTypeDef *huart, uint8_t ID);
int Motor42_ReadPosition(UART_HandleTypeDef *huart, uint8_t ID);

#endif
