section .bss
    global initial_brk
    initial_brk: resq 1

section .text
    global setup_brk
    global dismiss_brk
    global get_brk

setup_brk:
    push rbp
    mov rbp, rsp

    mov rax, 12
    xor rdi, rdi
    syscall

    mov [initial_brk], rax

    pop rbp
    ret

dismiss_brk:
    push rbp
    mov rbp, rsp

    mov rdi, [initial_brk]

    mov rax, 12
    syscall

    pop rbp
    ret

get_brk:
    push rbp
    mov rbp, rsp

    mov rax, 12 
    xor rdi, rdi
    syscall

    pop rbp
    ret