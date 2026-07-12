#include "rc522.h"
#include "delay.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include <stdint.h>
#include <string.h>
/* ---------------------------变量定义及引用声明----------------------------------------------- */
extern SPI_HandleTypeDef hspi1;
static uint8_t rx_buf[FIFO_MAX_LEN+1]={0};
static uint8_t tx_buf[FIFO_MAX_LEN+1]={0};
/* ---------------------------RC522复位----------------------------------------------- */
static void RC522_Reset()
{
    RC522_RST_L();
    DWT_delay_us(1);
    RC522_RST_H();
    DWT_delay_ms(50);
}
/* ---------------------------SPI收发单字节数据命令----------------------------------------------- */
static uint8_t SPI_ReadWrite_Byte(uint8_t byte)
{
    uint8_t dat=0;
    HAL_SPI_TransmitReceive(&hspi1, &byte, &dat, 1, 10);
    return dat;
}
/* ---------------------------RC522写寄存器----------------------------------------------- */
static void RC522_WR_Reg(uint8_t reg,uint8_t dat)
{
    RC522_CS_L();
    SPI_ReadWrite_Byte((reg<<1)&0X7E);
    SPI_ReadWrite_Byte(dat);
    RC522_CS_H();
}
/* ---------------------------RC522读寄存器----------------------------------------------- */
static uint8_t RC522_RD_Reg(uint8_t reg)
{
    uint8_t dat=0;
    RC522_CS_L();
    SPI_ReadWrite_Byte(((reg<<1)&0X7E)|0X80);
    dat=SPI_ReadWrite_Byte(0x00);
    RC522_CS_H();
    return dat;
}
/* ---------------------------RC522连续写入FIFO----------------------------------------------- */
static void RC522_WRFIFO_Burst(uint8_t *dat,uint8_t len)
{
    if(len>FIFO_MAX_LEN||dat==NULL||len==0)return;
    tx_buf[0]=WR_FIFO;
    memcpy(&tx_buf[1], dat, len);
    RC522_CS_L();
    HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, len+1, 10);
    RC522_CS_H();
}
/* ---------------------------RC522连续读出FIFO----------------------------------------------- */
static void RC522_RDFIFO_Burst(uint8_t *dat,uint8_t len)
{
    if(len>FIFO_MAX_LEN||dat==NULL||len==0)return;
    memset(tx_buf, RD_FIFO, len+1);
    RC522_CS_L();
    HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, len+1, 10);
    RC522_CS_H();
    memcpy(dat, &rx_buf[1], len);
}
/* ---------------------------RC522寄存器指定位置位----------------------------------------------- */
static void RC522_Set_Bit(uint8_t reg, uint8_t mask)
{
    uint8_t dat=RC522_RD_Reg(reg);
    RC522_WR_Reg(reg, dat|mask);
}
/* ---------------------------RC522寄存器指定位置0----------------------------------------------- */
static void RC522_Reset_Bit(uint8_t reg, uint8_t mask)
{
    uint8_t dat=RC522_RD_Reg(reg);
    RC522_WR_Reg(reg, dat&(~mask));
}
/* ---------------------------RC522开启天线----------------------------------------------- */
static void Pcd_Antenna_On()
{
    uint8_t dat=RC522_RD_Reg(TxControlReg);
    if(!(dat&0X03))
    {
        RC522_Set_Bit(TxControlReg, 0X03);
    }
}
/* ---------------------------RC522关闭天线----------------------------------------------- */
static void Pcd_Antenna_Off()
{
    RC522_Reset_Bit(TxControlReg, 0X03);
}
/* ---------------------------RC522通讯状态机----------------------------------------------- */
static uint8_t Communicate(uint8_t command,uint8_t *send_data,uint8_t send_len,uint8_t *back_data,uint16_t *back_len)
{
    uint8_t satus=MI_ERR;
    uint8_t IRQEn=0X00;
    uint8_t waitIRQ=0X00;
    uint8_t lastBits,n;
    uint16_t i;
    
    if (command==Transecive)
    {
        IRQEn=0X77;
        waitIRQ=0X30;
    }
    RC522_WR_Reg(CommIEnReg, IRQEn|0X80);
    RC522_WR_Reg(CommIRqReg, 0X7F);
    RC522_Set_Bit(FIFOLevelReg, 0X80);
    RC522_WR_Reg(CommandReg, Idle);
    for (i=0; i<send_len; i++) 
    {
        RC522_WR_Reg(FIFODataReg, send_data[i]);
    }
    RC522_WR_Reg(CommandReg, command);
    if (command==Transecive) 
    {
        RC522_Set_Bit(BitFramingReg, 0X80);
    }
    i=2000;
    do {
        n=RC522_RD_Reg(CommIRqReg);
        i--;
    }while ((i != 0) && !(n & 0x01) && !(n & waitIRQ));
    RC522_Reset_Bit(BitFramingReg, 0X80);
    if (i!=0) 
    {
        if (!(RC522_RD_Reg(ErrorReg)&0X1B)) 
        {
            satus=MI_OK;
            //超时
            if (n&IRQEn&0X01) 
            {
                satus=MI_ERR;
            }
            if(command==Transecive)
            {
                n=RC522_RD_Reg(FIFOLevelReg);
                lastBits=RC522_RD_Reg(ControlReg)&0X07;
                if(n==0)*back_len=0;
                else
                {
                    if(lastBits)*back_len=(n-1)*8+lastBits;
                    else
                    *back_len=n*8;
                }
                if(n==0)n=1;
                if(n>16)n=16;
                RC522_RDFIFO_Burst(back_data,n);
            }
        }else 
        {
        satus=MI_ERR;
        }
    }
    RC522_WR_Reg(CommandReg, Idle);
    return satus;
}
/* ---------------------------RC522的CRC计算----------------------------------------------- */
static void Calulate_CRC(uint8_t *send_buf, uint8_t len)
{
    uint8_t i, status;

    if (len < 2) return;
    RC522_WR_Reg(CommandReg, Idle);
    RC522_Set_Bit(FIFOLevelReg, 0x80);
    RC522_WRFIFO_Burst(send_buf,len-2);
    RC522_WR_Reg(DivIRqReg, 0x04);
    RC522_WR_Reg(CommandReg, CalcCRC);
    i = 0xFF;
    do {
        status = RC522_RD_Reg(DivIRqReg);
        i--;
    } while (!(status & 0x04) && (i != 0));
    send_buf[len - 2] = RC522_RD_Reg(CRCResultReg+1);
    send_buf[len - 1] = RC522_RD_Reg(CRCResultReg);
}
/* ---------------------------RC522寻找未休眠的卡----------------------------------------------- */
uint8_t PcdRequest(uint8_t card_type[2],uint8_t PICC_REQ)
{
    if(card_type==NULL)return MI_ERR;
    uint8_t status;
    uint8_t buf[2]={0};
    uint8_t send_buf[1];
    uint16_t len;

    RC522_WR_Reg(BitFramingReg, 0X07);
    send_buf[0]=PICC_REQ;
    status=Communicate(Transecive, send_buf, 1, buf, &len);
    if(status==MI_OK&&(len==16))
    {
        card_type[0]=buf[0];
        card_type[1]=buf[1];
    }
    else 
    {
        status=MI_ERR;
    }
    return status;
}
/* ---------------------------RC522防冲突----------------------------------------------- */
uint8_t Pcd_Anticoll(uint8_t card_id[4])
{
    uint8_t status,check=0,i;
    uint8_t send_buf[2],buf[6];
    uint16_t len;
    //标准帧
    RC522_WR_Reg(BitFramingReg, 0X00);
    send_buf[0]=PICC_ANTICOLL1;
    send_buf[1]=0X20;
    status=Communicate(Transecive, send_buf, 2, buf, &len);
    if(status==MI_OK&&len==40)
    {
        for(i=0;i<4;i++)
        check^=buf[i];
        if(check!=buf[4])
        {
            return MI_ERR;
        }
        for(i=0;i<4;i++)card_id[i]=buf[i];
        return MI_OK;
    }
    return MI_ERR;
}
/* ---------------------------RC522选卡----------------------------------------------- */
uint8_t Pcd_Select(const uint8_t card_id[4])
{
    uint8_t status,i;
    uint8_t send_buf[9], buf[6];
    uint16_t len; 
    send_buf[0]=PICC_ANTICOLL1;
    send_buf[1]=0X70;
    RC522_WR_Reg(BitFramingReg, 0X00);
    for(i=0;i<4;i++)send_buf[i+2]=card_id[i];
    send_buf[6]=card_id[0]^card_id[1]^card_id[2]^card_id[3];
    Calulate_CRC(send_buf, 9);
    status=Communicate(Transecive, send_buf, 9, buf, &len);
    if(status==MI_OK&&len==24)return MI_OK;
    return MI_ERR;
}
/* ---------------------------RC522休眠指令----------------------------------------------- */
void Pcd_Halt()
{
    uint8_t status;
    uint8_t send_buf[4], buf[4];
    uint16_t len; 
    send_buf[0]=PICC_HALT;
    send_buf[1]=0X00;
    RC522_WR_Reg(BitFramingReg, 0X00);
    Calulate_CRC(send_buf, 4);
    status=Communicate(Transecive, send_buf, 4, buf, &len);
}
/* ---------------------------RC522初始化----------------------------------------------- */
void RC522_Init()
{
    //RC522双重复位
    RC522_Reset();
    RC522_WR_Reg(CommandReg,softRest);
    while (RC522_RD_Reg(CommandReg) & 0x08);
    //配置工作模式
    RC522_WR_Reg(ModeReg, 0x3D);
    //配置调制深度
    RC522_WR_Reg(TxAutoReg, 0x40);
    //配置定时器7.5ms
    RC522_WR_Reg(TModeReg, 0X8D);
    RC522_WR_Reg(TPrescaler, 0x3E);
    RC522_WR_Reg(TReloadLReg, 0X1E);
    RC522_WR_Reg(TReloadHReg, 0X00);
    //配置收发速率
    RC522_WR_Reg(TxModeReg, 0X00);
    RC522_WR_Reg(RxModeReg, 0X00);
    //配置调制宽度
    RC522_WR_Reg(ModWidthReg, 0x26);
    // 配置天线驱动
    RC522_WR_Reg(RFCfgReg, 0x70);       
    RC522_WR_Reg(GsNReg, 0xFF);       
    RC522_WR_Reg(ModeGsPReg, 0x3F);    
    RC522_WR_Reg(RxThresholdReg, 0x55); 
    //清空中断标志
    RC522_WR_Reg(CommIEnReg, 0x00);
    RC522_WR_Reg(DivEnReg, 0x00);
    RC522_WR_Reg(CommIRqReg, 0xFF);
    RC522_WR_Reg(DivIRqReg, 0xFF);
    //清空FIFO
    RC522_Set_Bit(FIFOLevelReg, 0x80);
    //开启天线
    Pcd_Antenna_On();
}