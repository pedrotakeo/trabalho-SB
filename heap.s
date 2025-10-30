section .bss
    global initial_brk
    initial_brk: resq 1
    current_brk: resq 1

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

    mov QWORD [initial_brk], rax
    mov QWORD [current_brk], rax

    pop rbp
    ret

dismiss_brk:
    push rbp
    mov rbp, rsp

    mov rdi, QWORD [initial_brk]

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



worst_fit_address:  
    ;void* worst_fit_address (unsigned long int bytes(rdi), )
    push rbp
    mov rbp, rsp

    mov rax, QWORD [initial_brk]    ; rax = largest available space in heap (if any)
    mov rbx, QWORD [current_brk]    ; current brk position (end of heap)
    mov r10, QWORD [initial_brk]    ; address iterator

    xor rcx, rcx                  ; size of max block found (initialized as 0)

wfa_loop:
    cmp r10, rbx
    jge wfa_endloop  ; while (addressR10 < currentBRK)

    cmp BYTE [r10], 1
    je wfa_fini_loop  ;if (use = 1) go to "i++"

    cmp QWORD [r10 + 1], rdi
    jl wfa_fini_loop

    cmp QWORD [r10 + 1], rcx 
    jle wfa_fini_loop

    mov rax, r10    ;update max address
    mov rcx, QWORD [r10 + 1]  ;update max size

wfa_fini_loop:
    mov r9, QWORD[r10 + 1]
    add r10, 9
    add r10, r9
    jmp wfa_loop


wfa_endloop:

    cmp rcx, 0
    jne exit
    mov rax, 0

wfa_exit:
    pop rbp
    ret


;memory that properly allocates memory
memory_alloc:
    push rbp
    mov rbp, rsp

    ;already has parameter in rdi
    call worst_fit_address

    cmp rax, 0
    je alloc_top

alloc_middle:
    mov r8, QWORD [rax + 1]
    sub r8, rdi

    cmp r8, 10

    jmp alloc_exit
alloc_top:

alloc_exit:
    pop rbp 
    ret


