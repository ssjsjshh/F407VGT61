#include "tim.h"
#include "main.h"
/* ---------------------------us延时·----------------------------------------------- */
void tim_delay_us(uint16_t us)
{
    uint16_t start=TIM7->CNT;
    while((TIM7->CNT-start)<us);
}
/* ---------------------------ms延时·----------------------------------------------- */
void tim_delay_ms(uint16_t ms)
{
 while(ms--)
 tim_delay_us(1000);   
}
