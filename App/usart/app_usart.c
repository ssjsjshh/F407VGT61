#include "app_usart.h"
#include "cmd_list.h"
#include "moudle_usart.h"
#include "service_usart.h"
void usart_init()
{
    usart_moudle_init(usart_port_1);
    usart_moudle_init(usart_port_3);
    app_usart_init();
}
void task_list()
{
    uint8_t *frame;
    uint16_t len;
    if (usart_get_frame(usart_port_1, &frame, &len) == 0) 
    {
        shell(usart_port_1,(char*)frame); 
        usart_moudle_printf(usart_port_1,"%s\r\n",frame);
    }
    

}