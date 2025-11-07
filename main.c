#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "heap.h"

// Cores ANSI
#define RESET        "\033[0m"
#define BOLD         "\033[1m"
#define DIM          "\033[2m"
#define GREEN        "\033[32m"
#define RED          "\033[31m"
#define YELLOW       "\033[33m"
#define CYAN         "\033[36m"
#define MAGENTA      "\033[35m"
#define BLUE         "\033[34m"
#define GRAY         "\033[90m"

void print_heap() {
    uint64_t ini = ini_brk;
    uint64_t cur = cur_brk;
    
    printf("\n%s========== ESTADO ATUAL DA HEAP ==========%s\n", CYAN, RESET);
    printf("ini_brk: %p\n", (void*)ini);
    printf("cur_brk: %p\n", (void*)cur);

    if (ini == cur) {
        printf("%s  A heap está vazia%s\n\n", YELLOW, RESET);
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
        const char* color = is_used ? RED : GREEN;

        printf("%-5d   %-18p  %s%-7s%s   %lu bytes\n", 
               i, (void*)iter, color, status_str, RESET, size);

        iter += (9 + size);
        i++;
    }

    printf("------------------------------------------------------\n\n");
}

int main() {

    printf("%s***** TESTE DE ALOCAÇÃO DINÂMICA DE MEMÓRIA *****%s\n\n", BOLD, RESET);

    printf("BRK INICIAL: %p\n\n", sbrk(0));

    setup_brk();

    void *bloco1 = memory_alloc(50);
    if (bloco1)
        printf("%s[ BLOCO 1 ALOCADO ]%s\n", GREEN, RESET);

    print_heap();

    printf("%s[ BLOCO 1 DESALOCADO ]%s\n", YELLOW, RESET);
    memory_free(bloco1);
    print_heap();

    void *bloco2 = memory_alloc(25);
    if (bloco2)
        printf("%s[ BLOCO 2 ALOCADO ]%s\n", GREEN, RESET);

    void *bloco3 = memory_alloc(50);
    if (bloco3)
        printf("%s[ BLOCO 3 ALOCADO ]%s\n", GREEN, RESET);

    print_heap();

    void *bloco4 = memory_alloc(25);
    if (bloco4)
        printf("%s[ BLOCO 4 ALOCADO ]%s\n", GREEN, RESET);

    print_heap();

    memory_free(bloco4);
    printf("%s[ BLOCO 4 DESALOCADO ]%s\n", YELLOW, RESET);
    print_heap();

    void *bloco5 = memory_alloc(16);
    if (bloco5)
        printf("%s[ BLOCO 5 ALOCADO ]%s\n", GREEN, RESET);

    print_heap();

    printf("%s[ TESTES DE ERRO: MEMORY_FREE(NULL) E MEMORY_ALLOC(0) ]%s\n", MAGENTA, RESET);
    memory_alloc(0);
    memory_free(NULL);
    print_heap();

    printf("%s[ TESTE DE ENDEREÇOS INVÁLIDOS NO FREE ]%s\n", MAGENTA, RESET);
    printf("%d (esperado = -1)\n", memory_free((void *)ini_brk - 10));
    printf("%d (esperado = -1)\n", memory_free((void *)cur_brk + 10));
    printf("\n");
    print_heap();

    dismiss_brk();

    printf("%s[ DISMISS_BRK() ]%s\n", CYAN, RESET);
    printf("BRK FINAL: %p\n\n", sbrk(0));

    printf("%s***** FIM DOS TESTES *****%s\n", BOLD, RESET);

    return 0;
}
