/* Bonfire Board specific configuration */
/*
 * Copyright (c) 2021 Thomas Hornschuh
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>

#define RT_HEAP_SIZE (256*1024) // Size of RT-Thread Default Heap

#ifdef RT_DEBUG
#define BOARD_DEBUG(...) rt_kprintf(__VA_ARGS__)
#else
#define BOARD_DEBUG(...)
#endif


#endif