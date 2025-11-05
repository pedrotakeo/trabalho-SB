#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "heap.h"

void print_heap() {
    uint64_t ini = ini_brk;
    uint64_t cur = cur_brk;
    
    printf("ini_brk: %p\n", (void*)ini);
    printf("cur_brk: %p\n", (void*)cur);

    if (ini == cur) {
        printf("  A heap está vazia\n");
        printf(" \n");
        return;
    }

    printf("\n%-5s   %-18s   %-7s   %s\n", 
           "Bloco", "Endereço", "Status", "Tamanho");
    printf("------------------------------------------------------\n");

    uint8_t* iter = (uint8_t*)ini;
    int i = 0;

    while ((uint64_t)iter < cur) {
        
        uint8_t   is_used = *iter;
        uint64_t  size = *((uint64_t*)(iter + 1)); 

        const char* status_str = is_used ? "USADO" : "LIVRE";

        printf("%-5d   %-18p  %-7s   %lu bytes\n", 
               i, (void*)iter, status_str, size);

        iter += (9 + size);
        i++;
    }

    printf("------------------------------------------------------\n\n");
}

int main() {

    printf("***** TESTE DE ALOCAÇÃO DINÂMICA DE MEMÓRIA *****\n\n");

    setup_brk();

    void *bloco1 = memory_alloc(100);

    if (bloco1) {
        printf("[ BLOCO 1 ALOCADO ]\n");
    }

    print_heap();

    printf("[ BLOCO 1 DESALOCADO ]\n");
    memory_free(bloco1);

    print_heap();

    void *bloco2 = memory_alloc(50);

    if (bloco2) {
        printf("[ BLOCO 2 ALOCADO ]\n");
    }

    void *bloco3 = memory_alloc(100);

    if (bloco3) {
        printf("[ BLOCO 3 ALOCADO ]\n");
    }

    print_heap();

    void *bloco4 = memory_alloc(50);
    if (bloco4) {
        printf("[ BLOCO 4 ALOCADO ]\n");
    }

    print_heap();

    memory_free(bloco4);

    printf("[ BLOCO 4 DESALOCADO ]\n");

    void *bloco5 = memory_alloc(41);
    if (bloco5) {
        printf("[ BLOCO 4 ALOCADO ]\n");
    }

    print_heap();

    dismiss_brk();

    printf("[ DISMISS_BRK() ]\n");
    printf("BRK FINAL: %p\n", sbrk(0));

    return 0;
}