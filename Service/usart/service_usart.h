#ifndef __SERVICE_USART_H
#define __SERVICE_USART_H
#include "moudle_usart.h"
#include <stdint.h>
/* ---------------------------参数宏定义----------------------------------------------- */
#define         cmd_max          128        //命令长度
#define         fun_max          20         //函数长度
#define         name_max         20         //函数名长度
#define         args_max         6          //参数个数
#define         shell_cmd_max    20         //命令表所最大命令数
/* ---------------------------命令结构体定义----------------------------------------------- */
typedef struct
{
    char        name[name_max];      //函数名称
    void        (*func)(void);              //函数指针
    uint8_t     arg_num;                    //参数个数
    const char  *arg_types;                 // 参数类型: 'i'=整数, 's'=字符串, 'h'=16进制
    const char  *desc;                      //描述
}func_t;
/* ---------------------------统一上层接口----------------------------------------------- */
uint8_t shell_register(const func_t *cmd);
void shell(usart_port_t port, char* str);
#endif