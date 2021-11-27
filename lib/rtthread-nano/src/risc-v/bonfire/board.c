/*
 * Copyright (c) 2021 Thomas Hornschuh
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 
 */

#include <rthw.h>
#include <rtthread.h>
#include "bonfire.h"
#include "uart.h"
#include "console.h"
#include <reent.h>


static volatile uint32_t *pmtime = (uint32_t*)MTIME_BASE; // Pointer to memory mapped RISC-V Timer registers
static uint32_t tick_interval=0;


static uint32_t mtime_setinterval(uint32_t interval)
{
// Implementation for 32 Bit timer in Bonfire. Need to be adapted in case of a 64Bit Timer

   tick_interval=interval;

   printk("Set tick interval to %ld\n",interval);

   if (interval >0) {
     pmtime[2]=pmtime[0]+interval;
     set_csr(mie,MIP_MTIP); // Enable Timer Interrupt
   } else {
     clear_csr(mie,MIP_MTIP); // Disable Timer Interrupt

   }
   return tick_interval;
}

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

void SystemIrqHandler(uint32_t mcause,uint32_t mepc,void *trapframe)
{
    if (mcause & 0x80000000) {
       // interrupt
       switch (mcause & 0x0ff) {
         case 0x07:
           //printk("Timer irq @%ld\n",pmtime[0]);
           pmtime[2]=pmtime[0]+tick_interval;  // Will as side effect clear the pending irq
           rt_os_tick_callback();
           break;
        default:
          printk("Unexpeced interupt %lx\n",mcause);    
       }  
    }  else {
        printk("Trap Exception %lx at %lx\n",mcause,mepc);
        uart_readchar();        
        rt_hw_cpu_shutdown();
    }

}


#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (15*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    printk("rt_heap_begin_get called\n");
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif


static struct rt_mutex malloc_mutex;

// Newlib hooks

void __malloc_lock(struct _reent *r)   {

   //printk("Malloc lock called\n");
   rt_mutex_take(&malloc_mutex,10);
};
void __malloc_unlock(struct _reent *r) {
    //printk("Malloc unlock called\n");
    rt_mutex_release(&malloc_mutex);
};



/**
 * This function will initial your board.
 */
void rt_hw_board_init(void)
{

    /* 
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */

    mtime_setinterval( ((long)(SYSCLK/RT_TICK_PER_SECOND)));

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    printk(" invoking rt_components_board_init\n");
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif

   rt_mutex_init(&malloc_mutex,"malloc",RT_IPC_FLAG_PRIO);
}

#ifdef RT_USING_CONSOLE

static int uart_init(void)
{

    uart_setBaudRate(PLATFORM_BAUDRATE);
    return 0;
}
INIT_BOARD_EXPORT(uart_init);

void rt_hw_console_output(const char *str)
{
//#warning "TODO 3: Output the string 'str' through the uart."
   while (*str) {
     if (*str=='\n') uart_writechar('\r');
     uart_writechar(*str++);      
   }    
}

#endif

