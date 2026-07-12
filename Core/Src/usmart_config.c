#include "usmart_config.h"
#include "main.h"
#include "oled_api.h"
#include "usart.h"
/* ---------------------------------自定义命令start----------------------------------------- */
/* ---------------------------------LED翻转函数----------------------------------------- */
void LED_Toggle()
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}
/* ---------------------------------OLED显示函数----------------------------------------- */
void OLED_Draw(uint8_t x, uint8_t y,const char* str,uint8_t size)
{
    if (str == NULL || *str == '\0')
    OLED_Clear_AreaBuf(x,y,OLED_WIDTH);          
    if (x>=OLED_WIDTH||y>=OLED_PAGES) return;
    switch (size) 
    {
        case 1:
        {
            OLED_Clear_AreaBuf(x,y,OLED_WIDTH);
            OLED_Buf(x, y, str, 1);
            OLED_Refresh(y,y+1);
        }
        break;
        case 2:
        {
            OLED_Clear_AreaBuf(x,y,OLED_WIDTH);
            if(y<7)
            OLED_Clear_AreaBuf(x,y+1,OLED_WIDTH);
            OLED_Buf(x, y, str, size);
            OLED_Refresh(y,y+(size==1?1:2));
        }
        break;
    }
}
/* ---------------------------------自定义命令end----------------------------------------- */
/* ---------------------------------命令表初始化----------------------------------------- */
const USMART_FUNC cmd_Table[]=
{
    {"LED_Toggle",(void(*)(void))LED_Toggle,0,"","控制LED翻转"},
    {"OLED_Draw" ,(void(*)(void))OLED_Draw,4,"iisi","在OLED上显示"}
};
/* ---------------------------------命令数定义----------------------------------------- */
const uint8_t cmd_num=sizeof(cmd_Table)/sizeof(cmd_Table[0]);
/* ---------------------------------命令队列循环处理----------------------------------------- */
void task_list()
{
    __disable_irq();
    uint8_t n = RX1_handle.num;
    uint8_t m =RX3_handle.num;
    __enable_irq();
    if(n!=0)
    {
        shell((char*)RX1_handle.buf[RX1_handle.ring_rd]);
         __disable_irq();
        RX1_handle.ring_rd = (RX1_handle.ring_rd + 1) % ring_frame_num;
        RX1_handle.num--;
        __enable_irq();
    }
    if(m!=0)
    {
        Usart_Print(&TX1_handle, "%s\r\n",RX3_handle.buf[RX3_handle.ring_rd]);
         __disable_irq();
        RX3_handle.ring_rd = (RX3_handle.ring_rd + 1) % ring_frame_num;
        RX3_handle.num--;
        __enable_irq();
    }
}