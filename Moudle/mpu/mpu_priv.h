#ifndef __MPU_PRIV_H
#define __MPU_PRIV_H
#include "bsp_i2c.h"
#include "stm32f4xx_hal.h"
/* MPU绑定I2C总线 */
#define     I2C_MPU         bsp_i2c2 
/* MPU外部中断引脚 */
#define     MPU_INT_PIN     GPIO_PIN_12       
/* 通信地址以及读写地址 */
#define     MPU_ADDR    0X68
#define     MPU_WR      (MPU_ADDR<<1)
#define     MPU_RD      ((MPU_ADDR<<1)|0X01)
/* 内部寄存器地址 */
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
#endif