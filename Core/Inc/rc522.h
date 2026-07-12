#ifndef __RC522_H
#define __RC522_H
#include <stdint.h>
#define     FIFO_MAX_LEN            64
#define     TRY_COUNT_MAX           10
#define     WR_FIFO                 0X12
#define     RD_FIFO                 0X92
/* ---------------------------引脚宏定义----------------------------------------------- */
#define     RC522_CS_GPIO_Port       GPIOD
#define     RC522_CS_Pin             GPIO_PIN_4
#define     RC522_RST_GPIO_Port      GPIOD
#define     RC522_RST_Pin            GPIO_PIN_3
/* ---------------------------RC522函数宏定义----------------------------------------------- */
#define     RC522_CS_H()         (RC522_CS_GPIO_Port->BSRR=RC522_CS_Pin)   
#define     RC522_CS_L()         (RC522_CS_GPIO_Port->BSRR=(uint32_t)RC522_CS_Pin<<16)
#define     RC522_RST_H()        (RC522_RST_GPIO_Port->BSRR=RC522_RST_Pin)      
#define     RC522_RST_L()        (RC522_RST_GPIO_Port->BSRR=(uint32_t)RC522_RST_Pin<<16)   
/* ---------------------------RC522命令字宏定义----------------------------------------------- */
#define     MI_OK               0
#define     MI_ERR              2
#define     Idle                0X00        //无动作，取消当前命令执行
#define     CalcCRC             0X03        //激活CRC协处理器或执行自测试
#define     Transmit            0X04        //发送FIFO缓冲区
#define     NoCmdChange         0X07        //无命令改变
#define     Recieve             0X08        //激活接收器电路
#define     Transecive          0X0C        //发送接收器激活
#define     MFAuthent           0X0E        //执行读卡器标准认证
#define     softRest            0X0F        //复位RC522
/* ---------------------------Mifare_One卡片命令字宏定义----------------------------------------------- */
#define     PICC_REQIDL         0x26        //寻天线区内未进入休眠状态
#define     PICC_REQALL         0x52        //寻天线区内全部卡
#define     PICC_ANTICOLL1      0x93        //防冲撞
#define     PICC_ANTICOLL2      0x95        //防冲撞
#define     PICC_AUTHENT1A      0x60        //验证A密钥
#define     PICC_AUTHENT1B      0x61        //验证B密钥
#define     PICC_READ           0x30        //读块
#define     PICC_WRITE          0xA0        //写块
#define     PICC_DECREMENT      0xC0        //扣款
#define     PICC_INCREMENT      0xC1        //充值
#define     PICC_RESTORE        0xC2        //调块数据到缓冲区
#define     PICC_TRANSFER       0xB0        //保存缓冲区中数据
#define     PICC_HALT           0x50        //休眠
/* ---------------------------RC522寄存器宏定义start----------------------------------------------- */
/* ---------------------------命令和状态寄存器----------------------------------------------- */
#define     CommandReg          0X01        //命令启停寄存器
#define     CommIEnReg          0X02        //中断使能寄存器
#define     DivEnReg            0X03        //中断使能寄存器
#define     CommIRqReg          0X04        //中断请求寄存器
#define     DivIRqReg           0X05        //中断请求寄存器
#define     ErrorReg            0X06        //错误标志寄存器
#define     Status1Reg          0X07        //状态标志寄存器
#define     Status2Reg          0X08        //状态标志寄存器
#define     FIFODataReg         0X09        //FIFO缓冲寄存器
#define     FIFOLevelReg        0X0A        //FIFO字节寄存器
#define     WaterLevelReg       0X0B        //FIFO深度寄存器
#define     ControlReg          0X0C        //控制命令寄存器
#define     BitFramingReg       0X0D        //帧调节寄存器
#define     CollReg             0X0E        //接口首位冲突寄存器
/* ---------------------------功能寄存器----------------------------------------------- */
#define     ModeReg             0X11        //模式定义寄存器
#define     TxModeReg           0X12        //发送速率定义寄存器
#define     RxModeReg           0X13        //接收速率定义寄存器
#define     TxControlReg        0X14        //天线驱动逻辑定义寄存器
#define     TxAutoReg           0x15        //调制深度寄存器
#define     TxSelReg            0X16        //模拟部分内部源选择寄存器
#define     RxSelReg            0X17        //内部接收器选择寄存器
#define     RxThresholdReg      0X18        //位译码器阈值选择寄存器
#define     DemoReg             0X19        //解调器设置定义寄存器
/* ---------------------------CFG寄存器----------------------------------------------- */
#define     CRCResultReg        0X21        //CRC高位结果寄存器
#define     ModWidthReg         0X24        //调制宽度控制寄存器
#define     RFCfgReg            0X26        //接收器增益配置寄存器
#define     GsNReg              0X27        //天线N电导选择寄存器
#define     CWGsPReg            0X28        //P驱动器电导定义寄存器
#define     ModeGsPReg          0X29        //P驱动器输出电导定义寄存器
#define     TModeReg            0X2A        //内部定时器定义寄存器
#define     TPrescaler          0X2B        //内部定时器定义寄存器
#define     TReloadHReg         0X2C        //自动装载值高位寄存器
#define     TReloadLReg         0X2D        //自动装载值低位寄存器
#define     TCounterValHReg     0X2E        //定时器当前值高位寄存器
#define     TCounterValLReg     0X2F        //定时器当前值低位寄存器
/* ---------------------------测试寄存器----------------------------------------------- */
#define     TestSel1Reg         0X31        //常用测试信号配置寄存器
#define     TestSel2Reg         0X32        //常用测试信号和PRBS控制寄存器
#define     TestPinEnReg        0X33        //测试总线输出驱动器使能寄存器
#define     TestPinValueReg     0X34        //测试端口IO定义寄存器
#define     TestBusReg          0X35        //内部测试总线状态显示寄存器
#define     AutoTestReg         0X36        //控制数字自测试寄存器
#define     VersionReg          0X37        //显示版本寄存器
#define     AnalogTestReg       0X38        //控制管脚寄存器
#define     TestDAC1Reg         0X39        //DAC1测试值定义寄存器
#define     TestDAC2Reg         0X3A        //DAC2测试值定义寄存器
#define     TestADCReg          0X3B        //ADCI Q通道实际值显示寄存器
/* ---------------------------RC522寄存器宏定义end----------------------------------------------- */
uint8_t PcdRequest(uint8_t card_type[2],uint8_t PICC_REQ);
uint8_t Pcd_Anticoll(uint8_t card_id[4]);
uint8_t Pcd_Select(const uint8_t card_id[4]);
void Pcd_Halt();
void RC522_Init();
#endif