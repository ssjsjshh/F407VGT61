#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H
#include <stdint.h>
/* ---------------------------检测中断映射线函数宏定义----------------------------------------------- */
#define     GPIO_PIN_TO_EXTI_LINE(pin)  (uint8_t)(__builtin_ctz(pin))
/* ---------------------------统一上层接口----------------------------------------------- */    
void bsp_exti_register(uint16_t pin, void(*handler)(void));
#endif