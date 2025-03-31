#include "Motor42.h"
#include "usart.h"
#include <string.h>

// 使能电机
int Motor42_Enable(UART_HandleTypeDef *huart, uint8_t ID)
{
	uint8_t temp[6] = {0, 0xF3, 0xAB, 0x01, 0x00, 0x6B};		// 发送命令
	uint8_t correctReturn[] = {0x01, 0xF3, 0x02, 0x6B};			// 命令正确
	uint8_t conditionNotMetReturn[] = {0x01, 0xF3, 0xE2, 0x6B}; // 条件不满足
	uint8_t errorCommandReturn[] = {0x01, 0x00, 0xEE, 0x6B};	// 错误命令
	temp[0] = ID;
	HAL_UART_Transmit(huart, temp, 6, 0xfff); // 发送命令
	// 等待命令传回
	HAL_UART_Receive(huart, temp, 4, 0xff);
	// 判断接收情况
	if (memcmp(temp, correctReturn, 4) == 0)
	{
		// 正确返回：01 F3 02 6B
		// 处理正确返回的情况
		return 0;
	}
	else if (memcmp(temp, conditionNotMetReturn, 4) == 0)
	{
		// 条件不满足返回：01 F3 E2 6B
		// 处理条件不满足的情况
		return 1;
	}
	else if (memcmp(temp, errorCommandReturn, 4) == 0)
	{
		// 错误命令返回：01 00 EE 6B
		// 处理错误命令的情况
		return 2;
	}
	else
	{
		// 未知或错误的数据包
		// 处理未知或错误的情况
		return -1;
	}
}

/**
  * 函    数：电机速度控制
* 参    数：
			  Direction  	方向：0~1
			  Speed			速度：0~65535
			  Acc_Speed     加速度：0~255
			  ID            电机地址:1~16

  * 返 回 值：无
  */

int Motor42_SpeedControl(UART_HandleTypeDef *huart, uint8_t Direction, uint16_t Speed, uint8_t Acc_Speed, uint8_t ID)
{
	uint8_t temp[8] = {0, 0xf6, 0, 0, 0, 0, 0x00, 0x6B};
	uint8_t correctReturn[] = {0x01, 0xF6, 0x02, 0x6B};
	uint8_t conditionNotMetReturn[] = {0x01, 0xF6, 0xE2, 0x6B};
	uint8_t errorCommandReturn[] = {0x01, 0x00, 0xEE, 0x6B};
	uint8_t *Speed_h = (uint8_t *)&Speed;
	temp[0] = ID;
	temp[2] = Direction;
	temp[3] = *++Speed_h;
	temp[4] = *--Speed_h;
	temp[5] = Acc_Speed;

	HAL_UART_Transmit(huart, temp, 8, 0xfff); // 发送命令
	HAL_UART_Receive(huart, temp, 4, 0xff);	  // 等待命令传回
											  // 使用memcmp函数比较接收到的数据包
	if (memcmp(temp, correctReturn, 4) == 0)
	{
		// 命令正确返回：01 F6 02 6B
		// 处理命令正确返回的情况
		return 0;
	}
	else if (memcmp(temp, conditionNotMetReturn, 4) == 0)
	{
		// 条件不满足返回：01 F6 E2 6B
		// 处理条件不满足的情况
		return 1;
	}
	else if (memcmp(temp, errorCommandReturn, 4) == 0)
	{
		// 错误命令返回：01 00 EE 6B
		// 处理错误命令的情况
		return 2;
	}
	else
	{
		// 未知或错误的数据包
		// 处理未知或错误的情况
		return -1;
	}
}

/**
 * 函    数：控制电机停止
 * 参    数：
 * 返 回 值：无
 */

int Motor42_Stop(UART_HandleTypeDef *huart)
{
	uint8_t temp[5] = {0x01, 0xfe, 0x98, 0x00, 0x6B};
	uint8_t correctReturn[] = {0x01, 0xFE, 0x02, 0x6B};
	uint8_t conditionNotMetReturn[] = {0x01, 0xFE, 0xE2, 0x6B};
	uint8_t errorCommandReturn[] = {0x01, 0x00, 0xEE, 0x6B};
	HAL_UART_Transmit(huart, temp, 5, 0xfff); // 发送命令
	HAL_UART_Receive(huart, temp, 4, 0xff);	  // 等待命令传回
	// 使用memcmp函数比较接收到的数据包
	if (memcmp(temp, correctReturn, 4) == 0)
	{
		// 正确返回：01 FE 02 6B
		// 处理正确返回的情况
		return 0;
	}
	else if (memcmp(temp, conditionNotMetReturn, 4) == 0)
	{
		// 条件不满足返回：01 FE E2 6B
		// 处理条件不满足的情况
		return 1;
	}
	else if (memcmp(temp, errorCommandReturn, 4) == 0)
	{
		// 错误命令返回：01 00 EE 6B
		// 处理错误命令的情况
		return 2;
	}
	else
	{
		// 未知或错误的数据包
		// 处理未知或错误的情况
		return -1;
	}
}


 /**
				* 函    数：电机位置控制
				* 参    数：
				*          huart         串口句柄
				*          Direction     方向：0~1
				*          Speed         速度：0~65535
				*          Acc_Speed     加速度：0~255
				*          Size          位置：0~65535圈（16细分下）
				*          Mod           运动模式：0~1 （0为相对位置模式 1为绝对位置模式）
				*          ID            电机地址:1~16
				* 返 回 值：无
				*/


int	Motor42_PositionControl(UART_HandleTypeDef *huart, uint8_t Direction, uint16_t Speed, uint8_t Acc_Speed, uint32_t Size, uint8_t Mod, uint8_t ID)
{
	uint8_t temp[13] = {0, 0xFD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x6B};
	uint8_t correctReturn[] = {0x01, 0xFD, 0x02, 0x6B};
	uint8_t conditionNotMetReturn[] = {0x01, 0xFD, 0xE2, 0x6B};
	uint8_t errorCommandReturn[] = {0x01, 0x00, 0xEE, 0x6B};
	uint8_t *Speed_h = (uint8_t *)&Speed;
	uint8_t *Size_h = (uint8_t *)&Size;

	temp[0] = ID;
	temp[2] = Direction;
	temp[3] = *++Speed_h;
	temp[4] = *--Speed_h;
	temp[5] = Acc_Speed;
	temp[6] = *Size_h++;
	temp[7] = *Size_h++;
	temp[8] = *Size_h++;
	temp[9] = *Size_h;
	temp[10] = Mod;
	temp[11] = 0x00; // 多机同步标志

	HAL_UART_Transmit(huart, temp, 14, 0xfff); // 发送命令
	HAL_UART_Receive(huart, temp, 4, 0xff);	   // 等待命令传回

	// 使用memcmp函数比较接收到的数据包
	if (memcmp(temp, correctReturn, 4) == 0)
	{
		// 命令正确返回：01 FD 02 6B
		// 处理命令正确返回的情况
		return 0;
	}
	else if (memcmp(temp, conditionNotMetReturn, 4) == 0)
	{
		// 条件不满足返回：01 FD E2 6B
		// 处理条件不满足的情况
		return 1;
	}
	else if (memcmp(temp, errorCommandReturn, 4) == 0)
	{
		// 错误命令返回：01 00 EE 6B
		// 处理错误命令的情况
		return 2;
	}
	else
	{
		// 未知或错误的数据包
		// 处理未知或错误的情况
		return -1;
	}
}

//////////////////////////////////////////////////////////////以下未测试
/**
 * 函    数：读取电机实时转速
 * 参    数：
 *          huart         串口句柄
 *          ID            电机地址:1~16
 * 返 回 值：电机实时转速（RPM），错误时返回-1
 */
int Motor42_ReadSpeed(UART_HandleTypeDef *huart, uint8_t ID)
{
    uint8_t temp[5] = {ID, 0x35, 0x6B};
    uint8_t correctReturn[7] = {ID, 0x35, 0x00, 0x00, 0x00, 0x00, 0x6B};
    uint8_t errorCommandReturn[] = {ID, 0x00, 0xEE, 0x6B};

    HAL_UART_Transmit(huart, temp, 3, 0xfff); // 发送命令
    HAL_UART_Receive(huart, temp, 7, 0xff);   // 等待命令传回

    // 使用memcmp函数比较接收到的数据包
    if (memcmp(temp, correctReturn, 2) == 0 && temp[2] == 0x35)
    {
        // 正确返回：地址 + 0x35 + 符号 + 电机实时转速 + 校验字节
        int16_t speed = (temp[4] << 8) | temp[3];
        if (temp[2] == 0x01) // 负数
        {
            speed = -speed;
        }
        return speed;
    }
    else if (memcmp(temp, errorCommandReturn, 4) == 0)
    {
        // 错误命令返回：地址 + 0x00 + EE + 校验字节
        return -1;
    }
    else
    {
        // 未知或错误的数据包
        return -1;
    }
}

/**
 * 函    数：读取电机实时位置
 * 参    数：
 *          huart         串口句柄
 *          ID            电机地址:1~16
 * 返 回 值：电机实时位置（角度），错误时返回-1
 */
int Motor42_ReadPosition(UART_HandleTypeDef *huart, uint8_t ID)
{
    uint8_t temp[9] = {ID, 0x36, 0x6B};
    uint8_t correctReturn[9] = {ID, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6B};
    uint8_t errorCommandReturn[] = {ID, 0x00, 0xEE, 0x6B};

    HAL_UART_Transmit(huart, temp, 3, 0xfff); // 发送命令
    HAL_UART_Receive(huart, temp, 9, 0xff);   // 等待命令传回

    // 使用memcmp函数比较接收到的数据包
    if (memcmp(temp, correctReturn, 2) == 0 && temp[2] == 0x36)
    {
        // 正确返回：地址 + 0x36 + 符号 + 电机实时位置 + 校验字节
        int32_t position = (temp[6] << 24) | (temp[5] << 16) | (temp[4] << 8) | temp[3];
        if (temp[2] == 0x01) // 负数
        {
            position = -position;
        }
        return (position * 360) / 65536; // 转换为角度
    }
    else if (memcmp(temp, errorCommandReturn, 4) == 0)
    {
        // 错误命令返回：地址 + 0x00 + EE + 校验字节
        return -1;
    }
    else
    {
        // 未知或错误的数据包
        return -1;
    }
}
