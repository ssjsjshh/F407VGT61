#include "bsp_exti.h"
#include "stm32f4xx_hal.h"
/* ---------------------------中断处理函数指针表初始化----------------------------------------------- */
static void(*exti_handler[16])(void)={NULL};
/* ---------------------------注册外部回调函数----------------------------------------------- */
void bsp_exti_register(uint16_t pin, void(*handler)(void))
{
    uint8_t line=GPIO_PIN_TO_EXTI_LINE(pin);
    if(line<16&&handler!=NULL)
    exti_handler[line]=handler;
}
/* ---------------------------触发全局回调函数----------------------------------------------- */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t line=GPIO_PIN_TO_EXTI_LINE(GPIO_Pin);
    if(line<16&&exti_handler[line]!=NULL)
    exti_handler[line]();
}
