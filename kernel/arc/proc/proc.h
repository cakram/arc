/*
 * Copyright (c) 2011-2014 Graham Edgecombe <graham@grahamedgecombe.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef ARC_PROC_PROC_H
#define ARC_PROC_PROC_H

#include <arc/mm/seg.h>
#include <arc/proc/thread.h>
#include <arc/lock/spinlock.h>
#include <arc/util/list.h>
#include <stdint.h>

typedef enum
{
  PROC_RUNNING
} proc_state_t;

typedef struct proc
{
  /* state */
  proc_state_t state;

  /* physical address of the pml4 table of this process */
  uintptr_t pml4_table;

  /* vmm address space lock */
  spinlock_t vmm_lock;

  /* list of threads in this process */
  list_t thread_list;

  /* memory segments */
  seg_t segments;
} proc_t;

proc_t *proc_create(void);
proc_t *proc_get(void);
void proc_switch(proc_t *proc);

/*
 * add/remove a thread from this process.
 *
 * these methods should _not_ be called directly. to make a new thread, use
 * thread_create() and specify the proc to attach it to.
 *
 * to stop a thread, use thread_kill(). it will be removed from the process
 * automatically after it is dead.
 */
void proc_thread_add(proc_t *proc, thread_t *thread);
void proc_thread_remove(proc_t *proc, thread_t *thread);

void proc_destroy(proc_t *proc);

#endif
