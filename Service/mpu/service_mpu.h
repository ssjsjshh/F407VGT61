#ifndef __SERVICE_MPU_H
#define __SERVICE_MPU_H
#include "stdint.h"
#include <stdint.h>
/* ---------------------------原始数据换算系数----------------------------------------------- */
#define     ACCEL_SCALE     16384.0f        //加速度对应±2g     
#define     GYRO_SCALE      131.0f          //陀螺仪对应±250°/s
#define     AVG_NUM         30              //刷新速率 1/(NUM*8ms)
/* ---------------------------定义输出角结构体----------------------------------------------- */
typedef struct
{
    float       rol;                        //横滚角
    float       pitch;                      //俯仰角
    float       yaw;                        //偏航角
}service_angle_t;
/* ---------------------------定义陀螺仪误差结构体----------------------------------------------- */
typedef struct
{
    float       gx_off; 
    float       gy_off; 
    float       gz_off;
} service_gyroff_t;
/* ---------------------------定义显示输出角结构体----------------------------------------------- */
typedef struct
{
    float       rol;                        //横滚角
    float       pitch;                      //俯仰角
    float       yaw;                        //偏航角
}service_show_t;
/* ---------------------------变量结构体声明----------------------------------------------- */
extern service_show_t show;
extern volatile uint8_t show_flag;
/* ---------------------------统一上层接口----------------------------------------------- */
extern void MPU_Updata();
extern void MPU_Service_Init();
#endif
