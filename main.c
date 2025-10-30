#include <stdio.h>
#include <unistd.h>

extern void setup_brk();
extern void dismiss_brk();
extern long get_brk();
extern unsigned long int initial_brk;

int main() {
    setup_brk();

    printf("valor de initial_brk: %p\n", (void*)initial_brk);
    printf("%p \n", (void*)get_brk());

    sbrk(4096);
    printf("brk modificado: %p \n", (void*)get_brk());

    printf("dismiss_brk()\n");
    dismiss_brk();
    printf("valor de initial_brk: %p\n", (void*)initial_brk);

    printf("%p \n", (void*)get_brk());


    return 0;
}