#ifndef __SPI_OLED_H
#define __SPI_OLED_H
#include "stm32f4xx_hal.h"
#include "stdint.h"
/* ---------------------------引脚宏定义----------------------------------------------- */
#define     OLED_CS_GPIO_Port       GPIOA
#define     OLED_CS_GPIO_Pin        GPIO_PIN_2
#define     OLED_RST_GPIO_Port      GPIOA
#define     OLED_GPIO_Pin           GPIO_PIN_1
#define     OLED_DC_GPIO_Port       GPIOA
#define     OLED_DC__Pin            GPIO_PIN_0
/* ---------------------------OLED函数宏定义----------------------------------------------- */
#define     OLED_CS_H()         (OLED_CS_GPIO_Port->BSRR=OLED_CS_Pin)   
#define     OLED_CS_L()         (OLED_CS_GPIO_Port->BSRR=(uint32_t)OLED_CS_Pin<<16)
#define     OLED_RST_H()        (OLED_RST_GPIO_Port->BSRR=OLED_RST_Pin)      
#define     OLED_RST_L()        (OLED_RST_GPIO_Port->BSRR=(uint32_t)OLED_RST_Pin<<16)
#define     OLED_DC_H()         (OLED_DC_GPIO_Port->BSRR=OLED_DC_Pin)      
#define     OLED_DC_L()         (OLED_DC_GPIO_Port->BSRR=(uint32_t)OLED_DC_Pin<<16)       
#endif

