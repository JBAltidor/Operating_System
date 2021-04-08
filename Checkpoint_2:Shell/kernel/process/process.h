
typedef struct 
{ // linked list to keep track of the processes
    int pid;   
    char * name;
    struct process *next;  
} process;

