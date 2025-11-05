section .bss
    global ini_brk
    global cur_brk
    ini_brk: resq 1
    cur_brk: resq 1

section .text
    global setup_brk
    global dismiss_brk
    global get_brk
    global memory_alloc
    global memory_free

setup_brk:
    push rbp
    mov rbp, rsp                        ; function setup

    mov rax, 12
    xor rdi, rdi
    syscall                             ; sys_brk(0)

    mov QWORD [ini_brk], rax            ; stores the initial brk in ini_brk
    mov QWORD [cur_brk], rax

    pop rbp
    ret

dismiss_brk:
    push rbp
    mov rbp, rsp                        ; function setup

    mov rdi, QWORD [ini_brk]

    mov rax, 12
    syscall                             ; sys_brk(initial brk)

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

    mov rax, QWORD [ini_brk]        ; rax = largest available space in heap (if any)
    mov rbx, QWORD [cur_brk]        ; current brk position (end of heap)
    mov r10, QWORD [ini_brk]        ; address iterator

    xor rcx, rcx                    ; size of max block found (initialized as 0)

wfa_loop:
    cmp r10, rbx
    jge wfa_endloop                 ; while (addressR10 < currentBRK)

    cmp BYTE [r10], 1
    je wfa_fini_loop                ;if (use = 1)

    cmp QWORD [r10 + 1], rdi
    jl wfa_fini_loop                ;if size too small

    cmp QWORD [r10 + 1], rcx 
    jl wfa_fini_loop                ;if sufficient space but not worst fit

    mov rax, r10                    ;update max address
    mov rcx, QWORD [r10 + 1]        ;update max size

wfa_fini_loop:
    mov r9, QWORD[r10 + 1]          ; r9 = Data Size
    add r9, 9 
    add r10, r9

    jmp wfa_loop


wfa_endloop:

    cmp rcx, 0
    jne wfa_exit
    mov rax, 0

wfa_exit:
    pop rbp
    ret


;function that properly allocates memory
memory_alloc:
    push rbp
    mov rbp, rsp

    cmp rdi, 0
    je alloc_exit

    ;already has parameter in rdi
    call worst_fit_address

    cmp rax, 0
    je alloc_top

alloc_middle:
    mov r8, QWORD [rax + 1]
    sub r8, rdi

    cmp r8, 10
    jl am_exit                      ; if less then 10 extra bytes are available, just returns whole block

    mov QWORD [rax + 1], rdi        ; saves size of block

    mov r9, 9
    add r9, rdi                     ; size of current block

    sub r8, 9                       ; available extra space - 9

    mov BYTE [rax + r9], 0
    mov QWORD [rax + r9 + 1], r8

am_exit:
    mov BYTE [rax], 1
    jmp alloc_exit
    
alloc_top:

    mov r8, QWORD [cur_brk]         ; saving current brk (register starting point)
    mov r10, rdi                    ; save data size

    mov r9, r8                      ; saving into r9 for new brk
    add r9, 9                       ; extends to hold use and size data
    add r9, r10                     ; rdi = current brk +9 + size
    mov rdi, r9

    mov rax, 12
    syscall

    mov QWORD [cur_brk], rax
    mov BYTE [r8], 1
    mov QWORD [r8 + 1],  r10        ; old brk is new register staring point
    mov rax, r8                     ; return old brk

    jmp alloc_exit

alloc_exit:
    add rax, 9                      ; returns data portion of block exclusively
    pop rbp 
    ret

memory_free:
    push rbp
    mov rbp, rsp

    cmp rdi, 0                      ; tests invalid free
    je free_error

    sub rdi, 9

    cmp BYTE [rdi], 0
    je free_exit

    mov BYTE [rdi], 0

free_exit:
    pop rbp
    ret

free_error:
    mov rax, -1

    pop rbp
    ret