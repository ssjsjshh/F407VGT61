#include "spi.h"
#include <stdint.h>
//非阻塞传输，但是没必要在oled上使用
void SPI_Dma_Ready(SPI_HandleTypeDef *hspi,uint32_t timeout)//用于检测是否dma是否正常
{
    uint32_t time=timeout;
    while((hspi->State==HAL_SPI_STATE_BUSY_TX||HAL_SPI_STATE_BUSY)&& time>0)
    {
        time--;
    }
    if(hspi->State == HAL_SPI_STATE_ERROR || timeout == 0)
    {
        HAL_SPI_Abort(hspi);
    }
}
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        //对应片选拉高
    }
}