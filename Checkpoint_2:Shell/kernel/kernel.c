#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void main() {
    isr_install();
    irq_install();
    kprint("              ********  Byenvini nan OS nou anmmmm  ********\n");
    kprint("\n");
    kprint("> Ekri on komand. Ekri 'ED' pou'w jwenn lis komand yo.\n");
    kprint("\n");
}
//This handles user input
void user_input(char *input) {
    if (strcmp(input, "SOTI") == 0)
    {
        clear_screen();
        kprint("Ou kanpe CPU a. Orevwa\n ");
        asm volatile("hlt");
    }
    else if((strcmp(input, "ED") == 0))
    {
        
        kprint("> CLEAR :Retire tout sak sou ekran an.\n");
        kprint("> SOTI  :Pouw femen odinate a (kanpe CPU an).\n");

    }
     else if((strcmp(input, "CLEAR") == 0))
    {
        
       clear_screen();

    }
    else
    {
        kprint("ADMIN> Ou ekri '");
        kprint(input);
        kprint("'. Li pa on komand.\n");
    }
   
}
