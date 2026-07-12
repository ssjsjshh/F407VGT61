#ifndef __DELAY_H
#define __DELAY_H
#include <stdint.h>
/* --------------------------------函数声明------------------------------------------ */
void DWT_Init();
void Sys_Init();
void sys_delay_us(uint16_t us);
void sys_delay_ms(uint16_t ms);
void DWT_delay_us(uint32_t us);
void DWT_delay_ms(uint16_t ms);
void DWT_delay_ns();
#endif