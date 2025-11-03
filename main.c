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
    
    /*printf("Teste SETUP_BRK() e DISMISS_BRK()\n\n");
    setup_brk();

    printf("valor de ini_brk: %p\n", (void*)ini_brk);
    printf("valor de get_brk(): %p \n", (void*)get_brk());
    printf("\n");

    printf("Adicionando uma pagina ao brk...\n");
    sbrk(4096);
    printf("valor de get_brk(): %p \n", (void*)get_brk());
    printf("\n");

    printf("dismiss_brk()\n");
    dismiss_brk();
    printf("valor de ini_brk: %p\n", (void*)ini_brk);
    printf("valor de get_brk(): %p \n", (void*)get_brk()); */

    printf("Teste MEMORY_ALLOC()\n\n");
    setup_brk();

    print_heap();

    void* bloco1 = memory_alloc(23);

    printf("[ Bloco 1 alocado ]\n");
    printf("\n");

    print_heap();

    memory_free(bloco1);

    printf("[ Bloco 1 desalocado ]\n\n");

    print_heap();

    void* bloco3 = memory_alloc(3);

    printf("[ Bloco 3 alocado ]\n");
    printf("\n");

    void* bloco4 = memory_alloc(1);

    printf("[ Bloco 4 alocado ]\n");
    printf("\n");

    print_heap();

    memory_free(bloco3);
    memory_free(bloco4);

    printf("[ blocos 3 e 4 desalocados ]\n");

    print_heap();

    memory_free(bloco3);

    printf("[ double free bloco 3 ]\n");

    print_heap();

    void* bloco5 = memory_alloc(1); 

    printf("[ bloco 5 alocado ]\n"); 

    print_heap();

    printf("[ free inválido do bloco 3 ]\n"); // !!!!!!!!!!!!!!!!!
    memory_free(bloco3);

    print_heap();

    dismiss_brk(); 

    printf("%p\n", sbrk(0));

    return 0;
}