#ifndef __MOUDLE_USART_H
#define __MOUDLE_USART_H
#include <stdint.h>
/* ---------------------------构造端口枚举----------------------------------------------- */
typedef enum 
{
    usart_port_1,
    usart_port_3,
    usart_port_num
}usart_port_t;
/* ---------------------------统一上层接口----------------------------------------------- */
uint8_t usart_moudle_init(usart_port_t port);
uint8_t usart_moudle_send(usart_port_t port, const uint8_t *data, uint16_t len);
void usart_moudle_printf(usart_port_t port, const char *fmt, ...);
uint8_t usart_get_frame(usart_port_t port, uint8_t **data, uint16_t *len);
#endif