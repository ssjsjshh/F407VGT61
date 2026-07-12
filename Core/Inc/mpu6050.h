#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f4xx_hal.h"
#include <stdint.h>
/* ---------------------------引脚宏定义----------------------------------------------- */
#define     MPU_SDA_GPIO_Port       GPIOB
#define     MPU_SDA_Pin             GPIO_PIN_11
#define     MPU_SCL_GPIO_Port       GPIOB
#define     MPU_SCL_Pin             GPIO_PIN_10
/* ---------------------------通信地址以及读写地址----------------------------------------------- */
#define     MPU_ADDR    0X68
#define     MPU_WR      (MPU_ADDR<<1)
#define     MPU_RD      ((MPU_ADDR<<1)|0X01)
/* ---------------------------内部寄存器地址----------------------------------------------- */

#define     PWR_1           0X6B            //电源管理
#define     SMPLRT_DIV      0X19            //采样分频
#define     CONFIG          0X1A            //低通滤波
#define     GYRO_CONFIG     0X1B            //陀螺仪配置
#define     ACCEL_CONFIG    0X1C            //加速度计配置
#define     INT_CONFIG      0X37            //中断寄存器配置
#define     INT_ENABLE      0X38            //中断引脚使能
#define     INT_STATUS      0X3A            //中断状态寄存器
#define     ACCEL_XOUT      0X3B            //14字节数据读取起始位
#define     TEMP_OUT        0X41            //温度传感器高字节
#define     GYRO_XOUT       0X43            //陀螺仪x轴高字节
/* ---------------------------原始数据换算系数----------------------------------------------- */
#define     ACCEL_SCALE     16384.0f        //加速度对应±2g     
#define     GYRO_SCALE      131.0f          //陀螺仪对应±250°/s
#define     AVG_NUM         25              //刷新速率
/* ---------------------------定义原始数据结构体----------------------------------------------- */
typedef struct
{
    int16_t     ax,ay,az;                   //加速度xyz轴原始值
    int16_t     gx,gy,gz;                   //陀螺仪xyz轴原始值
    int16_t     temp;                       //片内温度原始值
}MPU_DataTypedef;
/* ---------------------------定义输出角结构体----------------------------------------------- */
typedef struct
{
    float       rol;                        //横滚角
    float       pitch;                      //俯仰角
    float       yaw;                        //偏航角
}MPU_AngleTypedef;
/* ---------------------------定义陀螺仪误差结构体----------------------------------------------- */
typedef struct
{
    float       gx_off; 
    float       gy_off; 
    float       gz_off;
} MPU_GyroOffTypedef;
/* ---------------------------变量声明----------------------------------------------- */
extern MPU_DataTypedef      MPU_Raw_Data;
extern MPU_AngleTypedef     MPU_Angle;
extern volatile uint8_t MPU_Flag;
/* ---------------------------函数声明----------------------------------------------- */
void MPU_Init();
void MPU_Updata();
#endif