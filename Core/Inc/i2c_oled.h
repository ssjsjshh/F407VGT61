#ifndef __OLED_H
#define __OLED_H
/* ---------------------------引脚宏定义----------------------------------------------- */
#define     OLED_SDA_GPIO_Port      GPIOB 
#define     OLED_SDA_Pin            GPIO_PIN_7
#define     OLED_SCL_GPIO_Port      GPIOB
#define     OLED_SCL_Pin            GPIO_PIN_6
/* ---------------------------寄存器宏定义----------------------------------------------- */
#define     OLED_ADDR               0X78        //OLED地址
#define     OLED_WR_ORDER           0X00        //写命令控制字节
#define     OLED_WR_DAT             0X40        //写数据控制字节
#endif
