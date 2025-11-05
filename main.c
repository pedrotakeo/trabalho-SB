#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

extern void setup_brk();
extern void dismiss_brk();
extern long get_brk();
extern void* memory_alloc();
extern void memory_free();

extern unsigned long int ini_brk;
extern unsigned long int cur_brk;

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

    
    printf("Teste SETUP_BRK() e DISMISS_BRK()\n\n");

    printf("setup_brk()");
    setup_brk();

    printf("valor de ini_brk: %p\n", (void*)ini_brk);
    printf("valor de get_brk(): %p \n", (void*)get_brk());
    printf("\n");

    printf("Teste MEMORY_ALLOC()\n\n");

    print_heap();

    void *bloco1 = memory_alloc(23);

    if (bloco1)
        printf("[ Bloco 1 alocado ]\n");

    printf("\n");

    print_heap();

    memory_free(bloco1);

    printf("[ Bloco 1 desalocado ]\n\n");

    print_heap();

    void* bloco2 = memory_alloc(3);

    if (bloco2)
        printf("[ Bloco 2 alocado ]\n");
    printf("\n");

    void* bloco3 = memory_alloc(1);

    if (bloco3)
        printf("[ Bloco 3 alocado ]\n");
    printf("\n");

    print_heap();

    memory_free(bloco2);
    memory_free(bloco3);

    printf("[ blocos 2 e 3 desalocados ]\n");

    print_heap();

    memory_free(bloco2);

    printf("[ double free bloco 2 ]\n");

    print_heap();

    void* bloco4 = memory_alloc(1); 

    if (bloco4)
        printf("[ bloco 4 alocado ]\n"); 

    print_heap();

    printf("[ free inválido do bloco 2 ]\n"); // !!!!!!!!!!!!!!!!!
    memory_free(bloco2);

    print_heap();

    printf("[ memory_free(0) e memory_free(NULL) ]\n");

    memory_free(0);
    memory_free(NULL);

    printf("Teste WORST-FIT\n\n");

    void* bloco5 = memory_alloc(3);
    void* bloco6 = memory_alloc(5);
    void* bloco7 = memory_alloc(4);

    print_heap();

    memory_free(bloco5);
    memory_free(bloco6);
    memory_free(bloco7);

    print_heap();

    printf("[ alocando bloco8 (2 bytes) ]\n");
    void* bloco8 = memory_alloc(2);

    print_heap();

    memory_free(bloco8);

    printf("dismiss_brk()\n");
    dismiss_brk();
    printf("valor de ini_brk: %p\n", (void*)ini_brk);
    printf("valor de get_brk(): %p \n", (void*)get_brk()); 
    
    return 0;
}