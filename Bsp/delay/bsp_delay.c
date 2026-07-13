#include "bsp_delay.h"
#include "stm32f4xx_hal.h"
#include "system_stm32f4xx.h"
#include <stdint.h>
uint32_t bsp_coreclock;
uint8_t ticks_per_us;
/* ----------------------------DWT初始化---------------------------------------------- */
void DWT_Init()
{
    // 使能DWT模块
    CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
    // 使能CYCCNT计数器
    DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk;
    ticks_per_us = SystemCoreClock / 1000000;
}
/* ----------------------------SYS初始化---------------------------------------------- */
void Sys_Init()
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}
/* ----------------------------us延时---------------------------------------------- */
void sys_delay_us(uint16_t us)
{
    uint32_t ticks=us*(SystemCoreClock/1000000);
    SysTick->LOAD=ticks;
    SysTick->VAL=0X00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    while((SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk)==0);
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL=0x00;
}
void DWT_delay_us(uint32_t us)
{
    uint32_t start_tickes=DWT->CYCCNT;
    uint32_t delay_tickes=us*(SystemCoreClock/1000000);
    while((DWT->CYCCNT-start_tickes)<delay_tickes);
}
/* ----------------------------ms延时---------------------------------------------- */
void DWT_delay_ms(uint16_t ms)
{
    while(ms--)
    {
    DWT_delay_us(1000);
    }
}
void sys_delay_ms(uint16_t ms)
{
    while(ms--)
    sys_delay_us(1000);
}
/* ----------------------------I2C专用延时---------------------------------------------- */
void DWT_delay_ns(uint16_t ns)
{
    uint32_t start_tickes=DWT->CYCCNT;
    uint32_t delay_tickes=(ns*ticks_per_us)/1000;
    if (delay_tickes==0)delay_tickes=1;
    while((DWT->CYCCNT-start_tickes)<delay_tickes);
}
/* ----------------------------DWT获得当前tick---------------------------------------------- */
uint32_t DWT_get_tick()
{
    return DWT->CYCCNT;
}
float DWT_Deltas(uint32_t now, uint32_t last)
{
    return (float)(now - last) / SystemCoreClock;
}