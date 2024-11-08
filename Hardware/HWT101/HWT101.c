#include "HWT101.h"
#include <stdio.h>
#include "usart.h"
float InfiniteYaw(float Now_Yaw);
int flag=0;//记录偏航角突变次数
float Last_Yaw;

volatile float global_angle;
volatile uint8_t new_data_received;
volatile float angular_velocity_y;
volatile float angular_velocity_z;




IMU_Typedef IMU;


//参数：数组data，缓存区大小length
void ParseAndPrintData(uint8_t* data, uint16_t length)//解释陀螺仪数据包
{
    for (int i=0; i < length - 11; i++)
    {

        if (data[i] == 0x55 && data[i + 1] == 0x53)//帧头
        {
            uint8_t checksum = CalculateChecksum(data+i, 11 - 1);//和校验
            if (checksum != data[i + 11 - 1])
            {
                continue;
            }
            uint8_t yaw_l = data[i + 6];
            uint8_t yaw_h = data[i + 7];
            int16_t yaw = (int16_t)((yaw_h << 8) | yaw_l);

            // 将解析后的整数值转换为角度
            float angle = ((float)yaw / 32768.0) * 180.0;
            global_angle = angle;
        }
        else if (data[i] == 0x55 && data[i + 1] == 0x52)
        {
            uint8_t checksum = CalculateChecksum(data+i,11 - 1);
            if (checksum != data[i + 11 - 1])
            {
                continue;
            }
            uint8_t wy_l = data[i + 4];
            uint8_t wy_h = data[i + 5];
            int16_t wy = (int16_t)((wy_h << 8) | wy_l);
            angular_velocity_y = ((float)wy / 32768.0) * 2000.0;

            uint8_t wz_l = data[i + 6];
            uint8_t wz_h = data[i + 7];
            int16_t wz = (int16_t)((wz_h << 8) | wz_l);
            angular_velocity_z = ((float)wz / 32768.0) * 2000.0;
        }
    }
	
	IMU.YawAngle=global_angle;
	IMU.angular_velocity_y=angular_velocity_y;
	IMU.angular_velocity_z=angular_velocity_z;
	IMU.Full_YawAngle=InfiniteYaw(global_angle);
}

void ResetHWT101(void)
{
	uint8_t temp[]={0xff,0xAA,0X76,0x00,0x00};
	HAL_UART_Transmit(&IMU_Uart, temp, 5,0xffff);
	flag=0;
	Last_Yaw=0;
}
 
uint8_t CalculateChecksum(uint8_t *data, uint16_t length) //数组头+到校验位前的长度
{
    uint8_t checksum = 0;
    for (uint16_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
}

float InfiniteYaw(float Now_Yaw)
{
	
	if(Now_Yaw-Last_Yaw<-180)//如果存在角度突变
	{
		flag++;
	}
	else if(Now_Yaw-Last_Yaw>180)
	{
		flag--;
	}
 
    Last_Yaw = Now_Yaw;
    return Now_Yaw+ flag*360;
}

