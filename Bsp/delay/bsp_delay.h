#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H
#include <stdint.h>
/* --------------------------------统一上层接口------------------------------------------ */
void DWT_Init();
void Sys_Init();
void sys_delay_us(uint16_t us);
void sys_delay_ms(uint16_t ms);
void DWT_delay_us(uint32_t us);
void DWT_delay_ms(uint16_t ms);
void DWT_delay_ns(uint16_t ns);
#endif