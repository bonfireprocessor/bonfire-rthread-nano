/*
 * Copyright (c) 2021 Thomas Hornschuh
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 
 */

.section .init
.global _bonfire_init


.extern _start_local
.extern _stacktop
.extern IRQ_Handler

#include "bits.h"

 .macro save_tf
  # save gprs
  STORE  x1,1*REGBYTES(x2)
  STORE  x3,3*REGBYTES(x2)
  STORE  x4,4*REGBYTES(x2)
  STORE  x5,5*REGBYTES(x2)
  STORE  x6,6*REGBYTES(x2)
  STORE  x7,7*REGBYTES(x2)
  STORE  x8,8*REGBYTES(x2)
  STORE  x9,9*REGBYTES(x2)
  STORE  x10,10*REGBYTES(x2)
  STORE  x11,11*REGBYTES(x2)
  STORE  x12,12*REGBYTES(x2)
  STORE  x13,13*REGBYTES(x2)
  STORE  x14,14*REGBYTES(x2)
  STORE  x15,15*REGBYTES(x2)
  STORE  x16,16*REGBYTES(x2)
  STORE  x17,17*REGBYTES(x2)
  STORE  x18,18*REGBYTES(x2)
  STORE  x19,19*REGBYTES(x2)
  STORE  x20,20*REGBYTES(x2)
  STORE  x21,21*REGBYTES(x2)
  STORE  x22,22*REGBYTES(x2)
  STORE  x23,23*REGBYTES(x2)
  STORE  x24,24*REGBYTES(x2)
  STORE  x25,25*REGBYTES(x2)
  STORE  x26,26*REGBYTES(x2)
  STORE  x27,27*REGBYTES(x2)
  STORE  x28,28*REGBYTES(x2)
  STORE  x29,29*REGBYTES(x2)
  STORE  x30,30*REGBYTES(x2)
  STORE  x31,31*REGBYTES(x2)

  # get sr, epc, badvaddr, cause
  csrr t0,mscratch // orginal SP from trap
  csrr   s0,mstatus
  csrr   t1,mepc
  csrr   t2,mbadaddr

  csrr   t3,mcause
  STORE  t0,2*REGBYTES(x2)
  STORE  s0,32*REGBYTES(x2)
  STORE  t1,33*REGBYTES(x2)
  STORE  t2,34*REGBYTES(x2)
  STORE  t3,35*REGBYTES(x2)

  # get faulting insn
  lw t4,(t1)
  STORE t4,36*REGBYTES(x2)
1:
  .endm



_bonfire_init:
  // init all registers
  li x1, 0
  li x2, 0
  li x3, 0
  li x4, 0
  li x5, 0
  li x6, 0
  li x7, 0
  li x8, 0
  li x9, 0
  li x10, 0
  li x11, 0
  li x12, 0
  li x13, 0
  li x14, 0
  li x15, 0
  li x16, 0
  li x17, 0
  li x18, 0
  li x19, 0
  li x20, 0
  li x21, 0
  li x22, 0
  li x23, 0
  li x24, 0
  li x25, 0
  li x26, 0
  li x27, 0
  li x28, 0
  li x29, 0
  li x30, 0
  li x31, 0

  la sp, _stacktop -1024 // Leave room for interrupt stack


  la a1, IRQ_Handler
  csrw mtvec,a1

  call _start_local

  endless:
  j endless


