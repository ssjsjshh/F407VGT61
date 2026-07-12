#ifndef __I2C_OLED_H
#define __I2C_OLED_H
#include "stm32f4xx_hal.h"
#include "delay.h"
#include <stdint.h>
/* ---------------------------宏定义函数·----------------------------------------------- */
#define SDA_H(I2C)      ((I2C)->SDA_GPIO_PORT->BSRR=(I2C)->SDA_PIN)
#define SDA_L(I2C)      ((I2C)->SDA_GPIO_PORT->BSRR=(uint32_t)(I2C)->SDA_PIN<<16)
#define SCL_H(I2C)      ((I2C)->SCL_GPIO_PORT->BSRR=(I2C)->SCL_PIN)
#define SCL_L(I2C)      ((I2C)->SCL_GPIO_PORT->BSRR=(uint32_t)(I2C)->SCL_PIN<<16)
#define SDA_READ(I2C)   (((I2C)->SDA_GPIO_PORT->IDR & (I2C)->SDA_PIN) ? 1 : 0)
#define I2C_DELAY()      DWT_delay_ns()
/* ---------------------------定义I2C句柄----------------------------------------------- */
typedef struct
{
    GPIO_TypeDef    *SDA_GPIO_PORT;
    GPIO_TypeDef    *SCL_GPIO_PORT;
    uint16_t        SDA_PIN;
    uint16_t        SCL_PIN;    
}Soft_I2C_Typedef;
/* ---------------------------变量声明----------------------------------------------- */
extern Soft_I2C_Typedef I2C_OLED;
extern Soft_I2C_Typedef I2C_MPU;
/* ---------------------------函数声明----------------------------------------------- */
void I2C_Start(Soft_I2C_Typedef *I2C);
void I2C_Stop(Soft_I2C_Typedef *I2C);
uint8_t I2C_Send_Byte(Soft_I2C_Typedef *I2C,uint8_t byte);
uint8_t I2C_Read_Byte(Soft_I2C_Typedef *I2C,uint8_t ack_nack);
#endif
