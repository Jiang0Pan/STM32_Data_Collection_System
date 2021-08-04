/**
  ******************************************************************************
  * @file           : malloc.c
  * @brief          : memory mangement 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 hopewellgroup.
  * All rights reserved.
  *
  * This software is licensed by hopewellgroup under Ultimate Liberty license
  *
  ******************************************************************************
*/

#ifndef __MALLOC_H
#define __MALLOC_H

#include "stm32f4xx_hal.h"
  
#ifndef NULL
	#define NULL 0
#endif

#define MEM_BLOCK_SIZE           100      /* �ڴ���СΪ32�ֽ� */
#define MEM_MAX_SIZE            16*1024    /*�������ڴ�2K */
#define MEM_ALLOC_TABLE_SIZE    MEM_MAX_SIZE/MEM_BLOCK_SIZE /* �ڴ���С */

typedef uint32_t  u32;
typedef uint8_t   u8;
typedef uint16_t  u16;   


/* �ڴ��������� */
struct _m_mallco_dev
{
    void (*init)(void);                    /* ��ʼ�� */
    u8   (*perused)(void);                 /* �ڴ�ʹ���� */
    u8   *membase;                         /* �ڴ�� */
    u16  *memmap;                          /* �ڴ����״̬�� */
    u8   memrdy;                           /* �ڴ�����Ƿ���� */
};

extern struct _m_mallco_dev mallco_dev;      /* ��mallco.c���涨�� */

void mymemset(void *s,u8 c,u32 count);     /*�����ڴ� */
void mymemcpy(void *des,void *src,u32 n);  /* �����ڴ� */

void mem_init(void);                     /* �ڴ�����ʼ������ */
u32 mem_malloc(u32 size);      		     /* �ڴ���� */
u8 mem_free(u32 offset);      			 /* �ڴ��ͷ� */
u8 mem_perused(void);                    /* ����ڴ�ʹ����        */
/* ****************************************************** */
/* �û����ú��� */
void myfree(void *ptr);                  /* �ڴ��ͷ� */
void *mymalloc(u32 size);                /* �ڴ���� */
void *myrealloc(void *ptr,u32 size);  /* ���·����ڴ� */

#endif

/************************ (C) COPYRIGHT Hopewellgroup ***********END OF FILE***/

