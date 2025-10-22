section .bss 
initial_brk:    resq 1 ;declara variavel que salva inicio da heap 


section .text
global _start

_setup_brk:
    push rbp
    mov rbp, rsp  ;setup da função

    ;syscall arguments (brk())
    mov   rax, 12         
    mov   rdi, 0          
    syscall 

    ;RAX com endereço de brk
    ret