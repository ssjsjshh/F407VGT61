#include "delay.h"
#include "main.h"
#include "stm32f407xx.h"
/* ----------------------------DWT初始化---------------------------------------------- */
void DWT_Init()
{
    // 使能DWT模块
    CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
    // 使能CYCCNT计数器
    DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk;
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
void DWT_delay_ns()
{
    uint32_t start_tickes=DWT->CYCCNT;
    uint32_t delay_tickes=168;
    while((DWT->CYCCNT-start_tickes)<delay_tickes);
}
