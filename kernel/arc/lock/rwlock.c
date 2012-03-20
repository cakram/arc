/*
 * Copyright (c) 2011-2012 Graham Edgecombe <graham@grahamedgecombe.com>
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

#include <arc/lock/rwlock.h>
#include <limits.h>

void rw_rlock(rwlock_t *lock)
{
  bool acquired = false;
  do
  {
    spin_lock(&lock->lock);
    if (!lock->writing && lock->read_permits != INT_MAX)
    {
      lock->read_permits++;
      acquired = true;
    }
    spin_unlock(&lock->lock);
  } while (!acquired);
}

void rw_runlock(rwlock_t *lock)
{
  spin_lock(&lock->lock);
  lock->read_permits--;
  spin_unlock(&lock->lock);
}

void rw_wlock(rwlock_t *lock)
{
  bool acquired = false;
  do
  {
    spin_lock(&lock->lock);
    if (!lock->writing && lock->read_permits == 0)
    {
      lock->writing = true;
      acquired = true;
    }
    spin_unlock(&lock->lock);
  } while (!acquired);
}

void rw_wunlock(rwlock_t *lock)
{
  spin_lock(&lock->lock);
  lock->writing = false;
  spin_unlock(&lock->lock);
}
