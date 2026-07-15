#include "cmd_list.h"
#include "service_usart.h"
#include "service_oled.h"
#include "gpio.h"
/* ---------------------------------自定义命令start----------------------------------------- */
/* ---------------------------------LED翻转函数----------------------------------------- */
void LED_Toggle()
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

/* ---------------------------------自定义命令end----------------------------------------- */
static const func_t app_cmds[]={
    {"LED_Toggle",(void(*)(void))LED_Toggle,0,"","控制LED翻转"},
    {"OLED_Draw" ,(void(*)(void))oled_draw,4,"iisi","在OLED上显示"}
};
void app_usart_init(void)
{
    uint8_t n = sizeof(app_cmds) / sizeof(app_cmds[0]);
    for (uint8_t i = 0; i < n; i++) {
        shell_register(&app_cmds[i]);
    }
}
