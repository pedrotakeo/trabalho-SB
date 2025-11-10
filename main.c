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

    printf("BRK INICIAL: %p\n\n", get_brk());

    setup_brk();

    void *brk_inicial = (void *)ini_brk;

    void *bloco1 = memory_alloc(50);
    if (bloco1)
        printf("%s[ BLOCO 1 ALOCADO 50B ]%s\n", GREEN, RESET);

    print_heap();

    printf("%s[ BLOCO 1 DESALOCADO ]%s\n", YELLOW, RESET);
    memory_free(bloco1);
    print_heap();

    void *bloco2 = memory_alloc(25);
    if (bloco2)
        printf("%s[ BLOCO 2 ALOCADO 25B ]%s\n", GREEN, RESET);

    print_heap();

    void *bloco3 = memory_alloc(50);
    if (bloco3)
        printf("%s[ BLOCO 3 ALOCADO 50B ]%s\n", GREEN, RESET);

    print_heap();

    void *bloco4 = memory_alloc(25);
    if (bloco4)
        printf("%s[ BLOCO 4 ALOCADO 25B ]%s\n", GREEN, RESET);

    print_heap();

    memory_free(bloco4);
    printf("%s[ BLOCO 4 DESALOCADO ]%s\n", YELLOW, RESET);
    print_heap();

    void *bloco5 = memory_alloc(16);
    if (bloco5)
        printf("%s[ BLOCO 5 ALOCADO 16B ]%s\n", GREEN, RESET);

    print_heap();

    printf("%s[ TESTES DE ERRO: MEMORY_FREE(NULL) E MEMORY_ALLOC(0) ]%s\n", MAGENTA, RESET);
    memory_alloc(0);
    memory_free(NULL);
    print_heap();

    printf("%s[ TESTE DE ENDEREÇOS INVÁLIDOS NO FREE ]%s\n", MAGENTA, RESET);
    printf("memory_free(< ini_brk): %d (esperado = -1)\n", memory_free((void *)ini_brk - 10));
    printf("memory_free(> cur_brk): %d (esperado = -1)\n", memory_free((void *)cur_brk + 10));
    printf("\n");
    print_heap();

    printf("%s********* TESTE DE INTEGRIDADE DE DADOS *********%s\n\n", BOLD, RESET);

    int *valor = (int *) memory_alloc(sizeof(int));
    
    if (valor) {

        printf("%s[ BLOCO 'valor' ALOCADO sizeof(int)B ]%s\n", GREEN, RESET);
    }

    print_heap();
    
    printf("[ VALOR ] = 1234\n");
    *valor = 1234;

    printf("Valor guardado: %d (esperado: 1234)\n\n", *valor);

    printf("%s****************** TESTE VETOR ******************%s\n\n", BOLD, RESET);

    int *vetor = (int *) memory_alloc(3 * sizeof(int));
    
    if (vetor) {

        printf("%s[ BLOCO 'vetor' ALOCADO 3 * sizeof(int)B ]%s\n", GREEN, RESET);
    }

    print_heap();
    
    printf("[ VETOR[0] ] = 123\n");
    printf("[ VETOR[1] ] = 456\n");
    printf("[ VETOR[2] ] = 789\n\n");

    vetor[0] = 123;
    vetor[1] = 456;
    vetor[2] = 789;

    printf("Valor guardado[0]: %d (esperado: 123)\n\n", vetor[0]);
    printf("Valor guardado[1]: %d (esperado: 456)\n\n", vetor[1]);
    printf("Valor guardado[2]: %d (esperado: 789)\n\n", vetor[2]);



    dismiss_brk();

    printf("%s[ DISMISS_BRK() ]%s\n", CYAN, RESET);

    void *brk_final = get_brk();

    if (brk_inicial == brk_final) {
        printf("%sBRK RESTAURADO CORRETAMENTE (%p == %p)%s\n", GREEN, brk_final, brk_inicial, RESET);
    } else {
        printf("%sBRK INCORRETO(brk final:%p ; esperado: %p)%s\n", RED, brk_final, brk_inicial, RESET);
    }

    printf("%s***** FIM DOS TESTES *****%s\n", BOLD, RESET);

    return 0;
}
