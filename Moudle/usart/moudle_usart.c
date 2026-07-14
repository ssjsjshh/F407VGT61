#include "moudle_usart.h"
#include "usart_priv.h"
#include "stm32f4xx_hal.h"
#include "usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
/* ---------------------------结构体以及串口列表实例化----------------------------------------------- */
static tx_handle_t tx1_handle,tx3_handle;
static rx_handle_t rx1_handle,rx3_handle;
static usart_handle_t usart_handle[usart_port_num]={{&huart1,&tx1_handle,&rx1_handle},{&huart3,&tx3_handle,&rx3_handle}};
/* ---------------------------------发送FIFO写入数据----------------------------------------- */
static uint8_t ring_buffer_write(tx_handle_t *handle,const uint8_t *data,uint16_t len)
{
    if(len==0||handle==NULL||data==NULL)
    return 1;
     __disable_irq();
    uint16_t head = handle->head;
    uint16_t tail = handle->tail;
    uint16_t used,free_space;
    if (head >= tail) 
    used = head - tail;
    else 
    used = ring_buf_size - tail + head;
    free_space = ring_buf_size - 1 - used;
    if (len > free_space) 
    {
        __enable_irq();
        return 1; // 缓冲区满
    }
    for (uint16_t i = 0; i < len; i++) 
    {
        handle->ring_buf[head] = data[i];
        head++;
        if (head >= ring_buf_size) 
        head = 0;
    }
    handle->head=head;
    __enable_irq();
    return 0;
}
/* ---------------------------------发送FIFO读出数据----------------------------------------- */
static uint16_t ring_buffer_read_raw(tx_handle_t *handle, uint8_t *out_buf, uint16_t max_len)
{
    uint16_t head = handle->head;
    uint16_t tail = handle->tail;
    uint16_t used;
    if (head >= tail) 
    used = head - tail;
    else 
    used = ring_buf_size - tail + head;
    if (used == 0)return 0;
    uint16_t read_len = (used > max_len) ? max_len : used;
    for (uint16_t i = 0; i < read_len; i++) 
    {
        out_buf[i] = handle->ring_buf[tail];
        tail++;
        if (tail >= ring_buf_size) 
        tail = 0;
    }
    handle->tail = tail;
    return read_len;
}
/* ---------------------------------发送FIFO读出数据----------------------------------------- */
static uint16_t ring_buffer_read(tx_handle_t *handle, uint8_t *out_buf, uint16_t max_len)
{
    __disable_irq();
    uint16_t len = ring_buffer_read_raw(handle, out_buf, max_len);
    __enable_irq();
    return len;
}
/* ---------------------------------环形缓冲区dma启动函数----------------------------------------- */
static void tx_start_dma(tx_handle_t *handle)
{
    __disable_irq();
    uint16_t len=ring_buffer_read_raw(handle, handle->buf, send_buf_size);
    if(len==0)
    {
        handle->flag=0;
        __enable_irq();
        return;
    }
    handle->flag=1;
    __enable_irq();
    HAL_UART_Transmit_DMA(handle->huart, handle->buf, len);
}
/* ---------------------------------串口发送函数----------------------------------------- */
static uint8_t tx_send(tx_handle_t *handle,const uint8_t* data,uint16_t len)
{
    if (ring_buffer_write(handle, data, len) != 0) 
    return 1;
    if (handle->flag==0) 
    {
        tx_start_dma(handle);
    }
    return 0;
}
/* ---------------------------------端口初始化----------------------------------------- */
uint8_t usart_moudle_init(usart_port_t port)
{
    if(port>=usart_port_num)return 1;
    usart_handle_t *handle=&usart_handle[port];
    tx_handle_t *tx=handle->tx;
    rx_handle_t *rx=handle->rx;
    tx->huart=handle->huart;
    tx->head=0;
    tx->tail=0;
    tx->flag=0;
    memset(tx->buf, 0, send_buf_size);
    memset(tx->ring_buf, 0, ring_buf_size);
    rx->huart=handle->huart;
    rx->ring_rd=0;
    rx->ring_wr=0;
    rx->num=0;
    __HAL_UART_ENABLE_IT(handle->huart, UART_IT_IDLE);
    HAL_UARTEx_ReceiveToIdle_DMA(handle->huart, rx->buf[rx->ring_rd], ring_frame_size-1);
    return 0;
}
/* ---------------------------------端口发送----------------------------------------- */
uint8_t usart_moudle_send(usart_port_t port, const uint8_t *data, uint16_t len)
{
    if (port>=usart_port_num)return 1;
    tx_send(usart_handle[port].tx, data, len);
    return 0;
}
/* ---------------------------------自定义端口printf----------------------------------------- */
void usart_moudle_printf(usart_port_t port, const char *fmt, ...)
{
    if (port >= usart_port_num) return;
    static char temp_buf[send_buf_size];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(temp_buf, send_buf_size, fmt, ap);
    va_end(ap);    
    uint16_t len = strlen(temp_buf);
    if (len > 0) 
    tx_send(usart_handle[port].tx, (uint8_t*)temp_buf, len);
}
/* ---------------------------------接收取帧----------------------------------------- */
uint8_t usart_get_frame(usart_port_t port, uint8_t **data, uint16_t *len)
{
    if (port >= usart_port_num) return 1;
    rx_handle_t *rx_handle = usart_handle[port].rx;
    __disable_irq();
    if (rx_handle->num == 0) 
    {
        __enable_irq();
        return 1;
    }
    
    *data = rx_handle->buf[rx_handle->ring_rd];
    *len = rx_handle->frame_len[rx_handle->ring_rd];  // 或记录实际长度
    rx_handle->ring_rd = (rx_handle->ring_rd + 1) % ring_frame_num;
    rx_handle->num--;
    __enable_irq();
    return 0;
}
/* ---------------------------------usart发送中断----------------------------------------- */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (uint8_t i=0; i<usart_port_num; i++) 
    {
        if (usart_handle[i].huart==huart) 
        {
            tx_start_dma(usart_handle[i].tx);
        }
    
    }
}
/* ---------------------------------usart空闲中断接收----------------------------------------- */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    for (uint8_t i = 0; i < usart_port_num; i++) 
    {
        if (usart_handle[i].huart == huart) 
        {
            rx_handle_t *rx = usart_handle[i].rx;            
            if (Size > 0) 
            {
                rx->buf[rx->ring_wr][Size] = '\0';
                rx->frame_len[rx->ring_wr] = Size;        
                rx->ring_wr = (rx->ring_wr + 1) % ring_frame_num;
                rx->num++;
            }
            
            HAL_UARTEx_ReceiveToIdle_DMA(huart, rx->buf[rx->ring_wr], ring_frame_size - 1);
            return;
        }
    }
}

