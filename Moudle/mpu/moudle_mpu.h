#ifndef __MOUDLE_MPU_H
#define __MOUDLE_MPU_H
#include "stdint.h"
#include <stdint.h>
/* ---------------------------原始数据结构体----------------------------------------------- */
typedef struct
{
    int16_t     ax,ay,az;                   //加速度xyz轴原始值
    int16_t     gx,gy,gz;                   //陀螺仪xyz轴原始值
    int16_t     temp;                       //片内温度原始值
}moudle_raw_data_t;
/* ---------------------------数据结构体对外声明----------------------------------------------- */
extern moudle_raw_data_t raw_data;
extern volatile uint8_t mpu_flag;
/* ---------------------------统一上层接口----------------------------------------------- */
uint8_t MPU_WR_REG(uint8_t reg,uint8_t data);
uint8_t MPU_RD_REG(uint8_t reg);
void MPU_RD_Raw_Data();
void MPU_Clear_SATUS();
void MPU_Moudle_Init();
#endif