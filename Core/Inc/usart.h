#ifndef __USART_H
#define __USART_H
#include "stm32f4xx_hal.h"
#include <stdint.h>
/* ---------------------------------缓冲区大小参数定义----------------------------------------- */
#define         buf_max_size            256
#define         ring_buf_max_size       1024
#define         ring_frame_num          8
#define         ring_frame_size         128
/* ---------------------------------发送结构体定义----------------------------------------- */
typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t ring_buf[ring_buf_max_size];
    uint8_t buf[buf_max_size];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t flag;
}TX_HandleTypeDef;
/* ---------------------------------接收结构体定义----------------------------------------- */
typedef struct
{
    UART_HandleTypeDef *huart;
    uint8_t buf[ring_frame_num][ring_frame_size];
    volatile uint8_t ring_wr;
    volatile uint8_t ring_rd;
    volatile uint8_t num;
    volatile uint8_t flag;
}RX_HandleTypeDef;
extern RX_HandleTypeDef RX1_handle;
/* ---------------------------------变量声明----------------------------------------- */
extern TX_HandleTypeDef TX1_handle;
extern RX_HandleTypeDef RX1_handle;
extern TX_HandleTypeDef TX3_handle;
extern RX_HandleTypeDef RX3_handle;
/* ---------------------------------函数声明----------------------------------------- */
void TX_Init(UART_HandleTypeDef* huart,TX_HandleTypeDef* handle);
void RX_Init(UART_HandleTypeDef* huart,RX_HandleTypeDef*handle);
void Usart_Print(TX_HandleTypeDef* handle,const char*fmt,...);
#endif