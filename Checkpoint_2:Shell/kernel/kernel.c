#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

// #include "process/process.h"
// define list of comand
#define CMD_SOTI  "SOTI"
#define CMD_ED "ED"
#define CMD_EFASE "EFASE"
#define CMD_PS "PS"
// end define list of comand

void main() {
    isr_install();
    irq_install();
    kprint("              ********  Byenvini nan OS nou an  ********\n");
    kprint("-----------------------------------------------------------------\n");
    kprint("\n");
    kprint("> Ekri yon komand. Ekri 'ED' pou'w jwenn lis komand yo.\n");
    kprint("\n");

}
//This handles user input
void user_input(char *input)
 {  
    if (strcmp(input, CMD_SOTI) == 0)
    {
        clear_screen();
        kprint("Ou kanpe CPU a. Orevwa\n ");
        asm volatile("hlt");
    }

        else if((strcmp(input, CMD_ED) == 0))
    { 
        kprint("> Men lis komand yo:\n");
        kprint("> SOTI : \nDeskripsyon : li pemet femen odinate a (kanpe CPU an).\n");
        kprint("> ED : \nDeskripsyon : li afiche ed la.\n");
        kprint("> EFASE : \nDeskripsyon : li pemet ou efase tout sa ki ekri sou ekran an.\n");
        kprint("> PS : \nDeskripsyon : li pemet ou afiche lis posesis kap travay yo.\n");
        kprint("> LANSE 'non pwogram' : \nDeskripsyon : li pemet ou efase tout sa ki ekri sou ekran an.\n");
    }
     else if((strcmp(input, CMD_EFASE) == 0))
    {        
       clear_screen();
    }
        else if((strcmp(input, CMD_PS) == 0))
    {        
       process_list();
    }
    else
    {
        kprint("ADMIN> Ou ekri '");
        kprint(input);
        kprint("'. Li pa on komand.\n");
    }
   
}

void process_list(){
    clear_screen();
    int i=1;
    kprint("Men lis posesis yo :\n");
    do{//placeholder for all running process
       char str[16]="";
       int_to_ascii(i, str);
       print_a_process("1","p1",str);
       i++;
    }while(i <20);
}

void print_a_process(char* process_id, char* process_name, char* process_address)
{
    kprint(process_id);
    kprint("   ");
    kprint(process_name);
    kprint("   ");
    kprint(process_address);
    kprint("\n");
}