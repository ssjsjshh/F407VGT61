#ifndef __BSP_I2C_H
#define __BSP_I2C_H
#include "stm32f4xx_hal.h"
#include <stdint.h>
/* ---------------------------I2C引脚宏定义·----------------------------------------------- */
#define     I2C1_SDA_GPIO_Port       GPIOB 
#define     I2C1_SDA_Pin             GPIO_PIN_7
#define     I2C1_SCL_GPIO_Port       GPIOB
#define     I2C1_SCL_Pin             GPIO_PIN_6
#define     I2C2_SDA_GPIO_Port       GPIOB
#define     I2C2_SDA_Pin             GPIO_PIN_11
#define     I2C2_SCL_GPIO_Port       GPIOB
#define     I2C2_SCL_Pin             GPIO_PIN_10
/* ---------------------------I2C基础函数宏定义·----------------------------------------------- */
#define SDA_H(I2C)      ((I2C)->SDA_GPIO_PORT->BSRR=(I2C)->SDA_PIN)
#define SDA_L(I2C)      ((I2C)->SDA_GPIO_PORT->BSRR=(uint32_t)(I2C)->SDA_PIN<<16)
#define SCL_H(I2C)      ((I2C)->SCL_GPIO_PORT->BSRR=(I2C)->SCL_PIN)
#define SCL_L(I2C)      ((I2C)->SCL_GPIO_PORT->BSRR=(uint32_t)(I2C)->SCL_PIN<<16)
#define SDA_READ(I2C)   (((I2C)->SDA_GPIO_PORT->IDR & (I2C)->SDA_PIN) ? 1 : 0)
/* ---------------------------定义模拟I2C结构体----------------------------------------------- */
typedef struct
{
    GPIO_TypeDef    *SDA_GPIO_PORT;
    GPIO_TypeDef    *SCL_GPIO_PORT;
    uint16_t        SDA_PIN;
    uint16_t        SCL_PIN;
    void            (*delay)(uint16_t);
    uint16_t        delay_parm;    
}bsp_i2c_t;
/* ---------------------------模拟I2C结构体声明----------------------------------------------- */
extern bsp_i2c_t bsp_i2c1;
extern bsp_i2c_t bsp_i2c2;
/* ---------------------------统一上层接口----------------------------------------------- */
void i2c_start(bsp_i2c_t *i2c);
void i2c_stop(bsp_i2c_t *i2c);
uint8_t i2c_send_byte(bsp_i2c_t *i2c,uint8_t byte);
uint8_t i2c_read_byte(bsp_i2c_t *i2c,uint8_t ack_nack);
#endif
