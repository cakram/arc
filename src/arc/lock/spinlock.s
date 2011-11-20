;
;  Copyright (c) 2011 Graham Edgecombe <graham@grahamedgecombe.com>
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

[global spin_lock]
spin_lock:
  push rbp
  mov rbp, rsp

  .try:
    mov rax, 1
    lock xchg [rdi], rax
    test rax, rax
    jz .acquired
    pause
    jmp .try

  .acquired:
    pop rbp
    ret

[global spin_try_lock]
spin_try_lock:
  push rbp
  mov rbp, rsp

  mov rax, 1
  lock xchg [rdi], rax
  test rax, rax
  jz .acquired
  mov rax, 0
  jmp .exit

  .acquired:
    mov rax, 1

  .exit:
    pop rbp
    ret

[global spin_unlock]
spin_unlock:
  push rbp
  mov rbp, rsp

  mov qword [rdi], 0

  pop rbp
  ret

