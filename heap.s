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

; aumenta brk conforme necessário ou de 4096
; +9 bytes da erro no alinhamento
; retorno (?)
memory_alloc:
    push rbp
    mov rbp, rsp

    mov rax, QWORD [initial_brk]    ; rax = largest available of block (if any)
    mov rbx, QWORD [current_brk]    ; current brk position (end of heap)
    mov r10, QWORD [initial_brk]    ; address iterator

    xor rcx, rcx                  ; size of max block found (initialized as 0)

    ;NEW LOGIC pseudocode


    ;;MODULARIZAR WORST FIT LOOP
    ;While r10 is smaller than top of heap (BRK)
        ;if (in use)
            ;go to next block

        ;else if (block size smaller than required size)
            ;go to next block

        ;else if (block size fits)
            ;rax = current address 
            ;rcx = current block size


    ;if(rcx != 0)
        ;faz setup dos registros de uso e tamanho
        ;se tamanho sobrando for maior ou igual a 10 bytes entao deica como um bloco livre, se não adiciona no tamanho total do bloco
    ;else
        ;aumenta tamanho da heap


block_search:
    cmp r10, rbx
    jge search_finished

    ; iterate
    cmp rcx, 0                      ; not found yet
    jne found

    cmp [r10], 0                    ; is used
    jne next_block

    cmp [r10 + 1], rdi              ; fits
    jl next_block

    jmp found
    
previously_found:                   ; already found
    cmp [r10], 0                    ; is used
    jne next_block

    cmp [r10 + 1], [rax + 1]        ; greater than found
    jl next_block

found:
    mov rcx, 1  
    mov rax, r10                    ; update greater

next_block:
    add r10, [r10 + 1]              ; next block
    add r10, 9
    jmp block_search

search_finished:
    cmp rcx, 1                      ; found == true
    je finalize

no_fitting_block:                   ; brk increase
    mov r12, rdi

    mov rdx, rbx
    add rdx, 9
    add rdx, rdi

    mov rax, 12
    mov rdi, rdx
    syscall

    mov rax, r12                    

finalize:                           ; block update
    mov BYTE [rax], 1
    mov QWORD [rax + 1], rcx

    pop rbp
    ret