#include "app_usart.h"
#include "moudle_usart.h"
void usart_init()
{
    usart_moudle_init(usart_port_1);
    usart_moudle_init(usart_port_3);
}
void task_list()
{
    uint8_t *frame;
    uint16_t len;
    if (usart_get_frame(usart_port_1, &frame, &len) == 0) {
        //shell((char*)frame);   // shell 解析执行，尽快处理完
        usart_moudle_printf(usart_port_1,"%s\r\n",frame);
    }
    
}