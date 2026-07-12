#include "gpio.h"
#include "main.h"
#include "mpu6050.h"
/* ------------------------------MPU数据采集外部中断-------------------------------------------- */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{   
    if(GPIO_Pin==MPU_INT_Pin)
    {
    MPU_Flag = 1;
    }
}