#ifndef __USART_PRIV_H
#define __USART_PRIV_H
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include <stdint.h>
/* ---------------------------------FIFO发送缓冲区大小参数定义----------------------------------------- */
#define         send_buf_size           256
#define         ring_buf_size           1024
/* ---------------------------------FIFO接收缓冲区大小参数定义----------------------------------------- */
#define         ring_frame_num          8
#define         ring_frame_size         128
/* ---------------------------------发送结usart构体定义----------------------------------------- */
typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t ring_buf[ring_buf_size];
    uint8_t buf[send_buf_size];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t flag;
}tx_handle_t;
/* ---------------------------------接收usart结构体定义----------------------------------------- */
typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t buf[ring_frame_num][ring_frame_size];
    volatile uint16_t frame_len[ring_frame_num];
    volatile uint8_t ring_wr;
    volatile uint8_t ring_rd;
    volatile uint8_t num;
}rx_handle_t;
typedef struct
{
    UART_HandleTypeDef *huart;
    tx_handle_t         *tx;
    rx_handle_t         *rx;
}usart_handle_t;
#endif