/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include "gd32f10x.h"
#include "drv_uart.h"
#include "drv_people.h"
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/* cortex-m �ܹ�ʹ�� SysTick_Handler() */
void SysTick_Handler()
{
    rt_os_tick_callback();
}

/* ϵͳʱ�ӳ�ʼ������ */
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{
	/* ��ʼ��ϵͳʱ�� */
	systick_config();
	rt_people_init();
	pwm1_siet_init();
	
	
	
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

#ifdef RT_USING_CONSOLE

//static int uart_init(void)
//{
//	dbgusart_config();
//    return 0;
//}
//INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
	    /* �����ٽ�� */
    rt_enter_critical();

    /* ֱ���ַ������� */
    while (*str!='\0')
    {
        /* ���� */
        if (*str=='\n')
        {
            usart_data_transmit(DEBUG_USART, '\r');
            while (usart_flag_get(DEBUG_USART, USART_FLAG_TC) == RESET);
        }

        usart_data_transmit(DEBUG_USART, *str++);
        while (usart_flag_get(DEBUG_USART, USART_FLAG_TC) == RESET);
    }

    /* �˳��ٽ�� */
    rt_exit_critical();
}

/* ����Finshʱ������������ */
#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    rt_hw_cpu_reset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */

#endif

