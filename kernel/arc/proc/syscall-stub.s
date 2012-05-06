;
;  Copyright (c) 2011-2012 Graham Edgecombe <graham@grahamedgecombe.com>
;
;  Permission to use, copy, modify, and/or distribute this software for any
;  purpose with or without fee is hereby granted, provided that the above
;  copyright notice and this permission notice appear in all copies.
;
;  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
;  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
;  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
;  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
;  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
;  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
;  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
;

[extern tty_puts]

[global syscall_stub]
syscall_stub:
  ; switch the GS base to the kernel's
  swapgs

  ; switch to the kernel stack (NB: when code calls SYSCALL, it must save R15
  ; somewhere along with RCX/R11)
  mov r15, rsp
  mov rsp, [gs:8]

  ; it is safe for to re-enable interrupts now, for information about the
  ; race condition see syscall.c where the SYSCALL flags mask is set
  sti

  ; preserve RCX and R11, these are used by SYSCALL/SYSRET
  push rcx
  push r11

  ; preserve registers that may be used by the function
  push r10
  push r9
  push r8
  push rdi
  push rsi
  push rdx
  push rax

  ; print the trace information
  call tty_puts

  ; restore the registers that were used by the function
  pop rax
  pop rdx
  pop rsi
  pop rdi
  pop r8
  pop r9
  pop r10

  ; restore the RCX and R11 registers
  pop r11
  pop rcx

  ; mask interrupts again, for the same race condition reasons
  cli

  ; switch back to the user stack
  mov rsp, r15

  ; switch the GS base to the user's
  swapgs

  ; return to user long mode
  o64 sysret

