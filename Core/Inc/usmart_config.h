#ifndef __USMART_CONFIG_H
#define __USMART_CONFIG_H
#include <stdint.h>
/* ---------------------------参数宏定义----------------------------------------------- */
#define         USMART_CMD_MAX          128 //命令长度
#define         USMART_FUN_MAX          20  //函数长度
#define         USMART_NAME_MAX         20  //函数名长度
#define         USMART_ARGS_MAX         6   //参数个数
/* ---------------------------命令结构体定义----------------------------------------------- */
typedef struct
{
    char        name[USMART_NAME_MAX];      //函数名称
    void        (*func)(void);              //函数指针
    uint8_t     arg_num;                    //参数个数
    const char  *arg_types;                 // 参数类型: 'i'=整数, 's'=字符串, 'h'=16进制
    const char  *desc;                      //描述
}USMART_FUNC;
/* ---------------------------变量声明----------------------------------------------- */
extern  const USMART_FUNC cmd_Table[];
extern  const uint8_t cmd_num;
/* ---------------------------函数声明----------------------------------------------- */
void shell(char* str);
void task_list();
#endif