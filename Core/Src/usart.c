#include "usart.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* ---------------------------------结构体初始化----------------------------------------- */
TX_HandleTypeDef TX1_handle;
RX_HandleTypeDef RX1_handle;
TX_HandleTypeDef TX3_handle;
RX_HandleTypeDef RX3_handle;
/* ---------------------------------发送缓存初始化----------------------------------------- */
static char temp_buf1[buf_max_size] = {0};

/* ---------------------------------发送串口初始化----------------------------------------- */
void TX_Init(UART_HandleTypeDef* huart,TX_HandleTypeDef* handle)
{
    handle->huart=huart;
    handle->flag=0;
    memset(handle->buf, 0,buf_max_size);
}
/* ---------------------------------环形缓冲区写入数据函数----------------------------------------- */
static uint8_t ring_buffer_write(TX_HandleTypeDef *handle,const uint8_t *data,uint16_t len)
{
    if(len==0||handle==NULL||data==NULL)
    return 1;
     __disable_irq();
    uint16_t head = handle->head;
    uint16_t tail = handle->tail;
    uint16_t used,free_space;
     __enable_irq();
    if (head >= tail) 
    used = head - tail;
    else 
    used = ring_buf_max_size - tail + head;
    free_space = ring_buf_max_size - 1 - used;
    if (len > free_space) 
    return 1; // 缓冲区满
    for (uint16_t i = 0; i < len; i++) 
    {
        handle->ring_buf[head] = data[i];
        head++;
        if (head >= ring_buf_max_size) 
        head = 0;
    }
    __disable_irq();
    handle->head=head;
    __enable_irq();
    return 0;
}
/* ---------------------------------环形缓冲区读出数据函数----------------------------------------- */
static uint16_t ring_buffer_read(TX_HandleTypeDef *handle, uint8_t *out_buf, uint16_t max_len)
{
     __disable_irq();
    uint16_t head = handle->head;
    uint16_t tail = handle->tail;
     __enable_irq();
    uint16_t used;
    if (head >= tail) 
    used = head - tail;
    else 
    used = ring_buf_max_size - tail + head;
    if (used == 0) 
    return 0;
    uint16_t read_len = (used > max_len) ? max_len : used;
    for (uint16_t i = 0; i < read_len; i++) 
    {
        out_buf[i] = handle->ring_buf[tail];
        tail++;
        if (tail >= ring_buf_max_size) 
        tail = 0;
    }
    __disable_irq();
    handle->tail = tail;
    __enable_irq();
    return read_len;
}
/* ---------------------------------环形缓冲区dma启动函数----------------------------------------- */
static void TX_Start_Dma(TX_HandleTypeDef*handle)
{
    uint16_t len=ring_buffer_read(handle, handle->buf, buf_max_size);
    if(len==0)
    {
        handle->flag=0;
        return;
    }
    handle->flag=1;
    HAL_UART_Transmit_DMA(handle->huart, handle->buf, len);
}
/* ---------------------------------串口发送函数----------------------------------------- */
static uint8_t TX_Send(TX_HandleTypeDef*handle,const uint8_t* data,uint16_t len)
{
    if (ring_buffer_write(handle, data, len) != 0) 
    return 1;
    if (handle->flag==0) 
    {
        TX_Start_Dma(handle);
    }
    return 0;
}
/* ---------------------------------接收串口初始化----------------------------------------- */
void RX_Init(UART_HandleTypeDef* huart,RX_HandleTypeDef*handle)
{
    handle->huart=huart;
    handle->flag=0;
    handle->num-0;
    handle->ring_rd=0;
    handle->ring_wr=0;
    for(uint8_t i=0; i<ring_frame_num; i++)memset(handle->buf[i], 0, ring_frame_size);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UARTEx_ReceiveToIdle_DMA(huart, handle->buf[0], ring_frame_size-1);
}
/* ---------------------------------串口发送中断----------------------------------------- */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)
    {
        TX_Start_Dma(&TX1_handle);
    }
    if(huart->Instance==USART3)
    {
        TX_Start_Dma(&TX3_handle);
    }
}
/* ---------------------------------串口接收IDEL空闲中断----------------------------------------- */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance==USART1) 
    {
        if (Size>0) 
        {
            RX1_handle.buf[RX1_handle.ring_wr][Size]='\0';
            RX1_handle.ring_wr=(RX1_handle.ring_wr+1)%ring_frame_num;
            RX1_handle.num+=1;
        }
        HAL_UARTEx_ReceiveToIdle_DMA(huart, RX1_handle.buf[RX1_handle.ring_wr], ring_frame_size);
    }
    if (huart->Instance==USART3) 
    {
        if (Size>0) 
        {
            RX3_handle.buf[RX3_handle.ring_wr][Size]='\0';
            RX3_handle.ring_wr=(RX3_handle.ring_wr+1)%ring_frame_num;
            RX3_handle.num+=1;
        }
        HAL_UARTEx_ReceiveToIdle_DMA(huart, RX3_handle.buf[RX3_handle.ring_wr], ring_frame_size);
    }
}
/* ---------------------------------自定义printf----------------------------------------- */
void Usart_Print(TX_HandleTypeDef* handle,const char *fmt,...)
{
    va_list ap;
    uint16_t len;
    va_start(ap, fmt);
    vsnprintf(temp_buf1, buf_max_size, fmt, ap);
    va_end(ap);
    len=strlen(temp_buf1);
    if(len==0)
    return;
    TX_Send(handle, (uint8_t*)temp_buf1, len);
}