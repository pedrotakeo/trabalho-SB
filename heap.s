section .bss 
initial_brk:    resq 1 ;declara variavel que salva inicio da heap 


section .text
global _start

_setup_brk: ;retorna endereço atual de brk
    push rbp
    mov rbp, rsp  ;setup da função

    ;syscall arguments (brk())
    mov   rax, 12         
    mov   rdi, 0   ;0 -> end atual     
    syscall 

    ;RAX com endereço de brk
    mov rsp, rbp
    pop rbp
    ret



_dismiss_brk:
    push rbp
    mov rbp, rsp  ;setup da função

    ;syscall arguments (brk())
    mov   rax, 12         
    mov   rdi, qword[initial_brk]        
    syscall 

    ;RAX com endereço de brk
    mov rsp, rbp
    pop rbp
    ret