#include "usmart.h"
#include "usmart_config.h"
#include "usart.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
/* ---------------------------------工具----------------------------------------- */
/* ---------------------------去除前后空格函数----------------------------------------------- */
static char* itrm(char* start)
{
    while (*start && isspace((unsigned char)*start)) 
    start++;
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) 
    *end-- = '\0';
    return start;
}
/* ---------------------------函数名寻找函数----------------------------------------------- */
static int find_fun(const char* name)
{
    for(uint8_t i=0;i<cmd_num;i++)
    {
        if(strcmp(cmd_Table[i].name, name)==0)
        return i;
    }
    return -1;
}
/* ---------------------------参数类型判断函数----------------------------------------------- */
static uint32_t parse(const char* str,char t)
{
    if(t=='s')
    return (uint32_t)(uintptr_t)str;
    if(t=='i')
    return (uint32_t)strtoul(str, NULL, 16);
    return (uint32_t)strtoul(str, NULL, 0);
}
/* ---------------------------参数分离函数----------------------------------------------- */
static uint8_t split_args(char*str,char**argv,uint8_t max)
{
    char *p=str;
    uint8_t n=0;
    while(*p&&n<max)
    {
        while(*p==' '||*p=='\t')
        p++;   
        if(!*p)break;
        argv[n]=p;
        char quote=0;
        while(*p)
        {
            if(quote)
            {
                if(*p==quote)
                quote=0;
            }
            else
            {
                if (*p == '"' || *p == '\'') quote = *p;
                else if (*p == ',') 
                { 
                    *p = '\0'; 
                    p++; 
                    break; 
                }
            }
            p++;
        }
        n++;
            if(!*p)break;
    }
    return n;

}
/* ---------------------------功能执行函数----------------------------------------------- */
static void call(const USMART_FUNC *c, uint32_t *a)
{
    void (*f)(void) = c->func;
    switch (c->arg_num) {
        case 0: f(); break;
        case 1: ((void (*)(uint32_t))f)(a[0]); break;
        case 2: ((void (*)(uint32_t,uint32_t))f)(a[0],a[1]); break;
        case 3: ((void (*)(uint32_t,uint32_t,uint32_t))f)(a[0],a[1],a[2]); break;
        case 4: ((void (*)(uint32_t,uint32_t,uint32_t,uint32_t))f)(a[0],a[1],a[2],a[3]); break;
        case 5: ((void (*)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t))f)(a[0],a[1],a[2],a[3],a[4]); break;
        case 6: ((void (*)(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t))f)(a[0],a[1],a[2],a[3],a[4],a[5]); break;
    }
}
/* ---------------------------函数声明----------------------------------------------- */
/* ---------------------------命令执行函数----------------------------------------------- */
void shell(char* str)
{
    char *p = itrm(str);
    if(*p=='\0')return;
    //分离名字和参数
    if(p[0]=='A'&&p[1]=='T')
    {
        Usart_Print(&TX3_handle, "%s\r\n",p);
        return;
    }
    char *sp=strchr(p,' ');
    char *br=strchr(p,'(');
    char name[USMART_NAME_MAX]={0};
    char *args=NULL;
    if(br)
    {
        int n=br-p;
        if(n>=USMART_NAME_MAX)
        n=USMART_NAME_MAX-1;
        memcpy(name, p, n);
        name[n]='\0';
        args=br+1;
        char *end=strchr(args,')');
        if(end)
        *end='\0';
    }
    else if(sp)
    {
        int n=sp-p;
        if(n>=USMART_NAME_MAX)
        n=USMART_NAME_MAX-1;
        memcpy(name, p, n);
        name[n]='\0';
        args = sp + 1; 
    }
    else
    {
        strncpy(name, p, USMART_NAME_MAX-1);
        name[USMART_NAME_MAX-1]='\0';
    }
    //内置命令
    if(strcmp(name,"list")==0){return;}
    if(strcmp(name,"help")==0){return;}
    //查找用户函数
    int id=find_fun(name);
    if(id<0)
    {
        Usart_Print(&TX1_handle, "error:%s not found\r\n",name);
        return;
    }
    const USMART_FUNC *cmd=&cmd_Table[id];
    if(cmd->arg_num==0)
    {
        call(cmd, NULL);
        return;
    }
    uint32_t a[USMART_ARGS_MAX]={0};
    uint8_t n=0;
    //参数处理
    if (args==NULL) 
    {
        Usart_Print(&TX1_handle, "error:%s takes no args\r\n",name);
        return;
    }
    else if(args)
    {
        char buf[USMART_CMD_MAX];
        strncpy(buf, args, USMART_CMD_MAX - 1);
        buf[USMART_CMD_MAX - 1] = '\0';
        char *argv[USMART_ARGS_MAX];
        n=split_args(buf, argv, cmd->arg_num);
        for(uint8_t i=0;i<n;i++)
        {
            char t=(cmd->arg_types&&cmd->arg_types[i])?cmd->arg_types[i]:'i';
            a[i]=parse(argv[i], t);
        }
    }
    else
    {
        Usart_Print(&TX1_handle, "error: %s needs %d args\r\n,",cmd->name,cmd->arg_num);
        return;
    }
    if(n!=cmd->arg_num)
    {
        Usart_Print(&TX1_handle, "error: %s needs %d args but got %d \r\n",cmd->name,cmd->arg_num,n);
        return;
    }
    call(cmd,a);
}
