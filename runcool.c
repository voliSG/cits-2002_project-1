//  CITS2002 Project 1 2021
//  Name(s):             Sean Peralta Garcia
//  Student number(s):   23088091

//  compile with:  cc -std=c11 -Wall -Werror -o runcool runcool.c

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//  THE STACK-BASED MACHINE HAS 2^16 (= 65,536) WORDS OF MAIN MEMORY
#define N_MAIN_MEMORY_WORDS (1<<16)

//  EACH WORD OF MEMORY CAN STORE A 16-bit UNSIGNED ADDRESS (0 to 65535)
#define AWORD               uint16_t
//  OR STORE A 16-bit SIGNED INTEGER (-32,768 to 32,767)
#define IWORD               int16_t

//  THE ARRAY OF 65,536 WORDS OF MAIN MEMORY
AWORD                       main_memory[N_MAIN_MEMORY_WORDS];

//  THE SMALL-BUT-FAST CACHE HAS 32 WORDS OF MEMORY
#define N_CACHE_WORDS       32


//  see:  https://teaching.csse.uwa.edu.au/units/CITS2002/projects/coolinstructions.php
enum INSTRUCTION {
    I_HALT       = 0,
    I_NOP        = 1,
    I_ADD        = 2,
    I_SUB        = 3,
    I_MULT       = 4,
    I_DIV        = 5,
    I_CALL       = 6,
    I_RETURN     = 7,
    I_JMP        = 8,
    I_JEQ        = 9,
    I_PRINTI     = 10,
    I_PRINTS,
    I_PUSHC,
    I_PUSHA,
    I_PUSHR,
    I_POPA,
    I_POPR
};

//  USE VALUES OF enum INSTRUCTION TO INDEX THE INSTRUCTION_name[] ARRAY
const char *INSTRUCTION_name[] = {
    "halt",
    "nop",
    "add",
    "sub",
    "mult",
    "div",
    "call",
    "return",
    "jmp",
    "jeq",
    "printi",
    "prints",
    "pushc",
    "pusha",
    "pushr",
    "popa",
    "popr"
};

//  ----  IT IS SAFE TO MODIFY ANYTHING BELOW THIS LINE  --------------


//  THE STATISTICS TO BE ACCUMULATED AND REPORTED
int n_main_memory_reads     = 0;
int n_main_memory_writes    = 0;
int n_cache_memory_hits     = 0;
int n_cache_memory_misses   = 0;

void report_statistics(void)
{
    printf("@number-of-main-memory-reads\t%i\n",    n_main_memory_reads);
    printf("@number-of-main-memory-writes\t%i\n",   n_main_memory_writes);
    printf("@number-of-cache-memory-hits\t%i\n",    n_cache_memory_hits);
    printf("@number-of-cache-memory-misses\t%i\n",  n_cache_memory_misses);
}

//  -------------------------------------------------------------------

//  EVEN THOUGH main_memory[] IS AN ARRAY OF WORDS, IT SHOULD NOT BE ACCESSED DIRECTLY.
//  INSTEAD, USE THESE FUNCTIONS read_memory() and write_memory()
//
//  THIS WILL MAKE THINGS EASIER WHEN WHEN EXTENDING THE CODE TO
//  SUPPORT CACHE MEMORY

AWORD read_memory(int address)
{
    ++n_main_memory_reads;
    return main_memory[address];
}

void write_memory(AWORD address, AWORD value)
{
    ++n_main_memory_writes;
    main_memory[address] = value;
}

//  -------------------------------------------------------------------

//  EXECUTE THE INSTRUCTIONS IN main_memory[]
int execute_stackmachine(void)
{
//  THE 3 ON-CPU CONTROL REGISTERS:
    int PC      = 0;                    // 1st instruction is at address=0
    int SP      = N_MAIN_MEMORY_WORDS;  // initialised to top-of-stack
    int FP      = 0;                    // frame pointer

//  REMOVE THE FOLLOWING LINE ONCE YOU ACTUALLY NEED TO USE FP
    FP = FP+0;

    while(true) {
        IWORD value1, value2;

//  FETCH THE NEXT INSTRUCTION TO BE EXECUTED
        IWORD instruction   = read_memory(PC);
        ++PC;

//      printf("%s\n", INSTRUCTION_name[instruction]);

        if(instruction == I_HALT) {
            break;
        }

//  SUPPORT OTHER INSTRUCTIONS HERE 
//      ....
        switch (instruction) {
        case I_NOP :
                            printf(">>> I_NOP\n");
                            break;
        case I_ADD :
                            printf(">>> I_ADD\n");

                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value1 + value2);
                            break;
        case I_SUB :
                            printf(">>> I_SUB\n");

                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value2 - value1);
                            break;
        case I_MULT :
                            printf(">>> I_MULT\n");

                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value1 * value2);
                            break;
        case I_DIV :
                            printf(">>> I_DIV\n");

                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value2 / value1);
                            break;
        case I_CALL :
                            printf(">>> I_CALL\n");

                            break;
        case I_RETURN :
                            printf(">>> I_RETURN\n");

                            break;
        case I_JMP :
                            printf(">>> I_JMP\n");

                            PC = read_memory(PC);
                            break;
        case I_JEQ :
                            printf(">>> I_JEQ\n");

                            if (read_memory(SP) == 0) {
                                PC = read_memory(PC);
                            } else {
                                ++PC;
                            }
                            break;
        case I_PRINTI :
                            printf(">>> I_PRINTI\n");

                            printf("...%i\n", read_memory(SP));
                            break;
        case I_PRINTS :
                            printf(">>> I_PRINTS\n");


                            break;
        case I_PUSHC :
                            printf(">>> I_PUSHC\n");

                            value1 = read_memory(PC);
                            --SP;
                            write_memory(SP, value1);

                            ++PC;
                            break;
        case I_PUSHA :
                            printf(">>> I_PUSHA\n");

                            // hold the address of the integer value to be pushed onto stack
                            value1 = read_memory(PC);
                            --SP;
                            write_memory(SP, read_memory(value1));
                            
                            ++PC;
                            break;
        case I_PUSHR :
                            printf(">>> I_PUSHR\n");

                            break;
        case I_POPA :
                            printf(">>> I_POPA\n");

                            break;
        case I_POPR :
                            printf(">>> I_POPR\n");

                            break;  
        }              
    }

//  THE RESULT OF EXECUTING THE INSTRUCTIONS IS FOUND ON THE TOP-OF-STACK
    return read_memory(SP);
}

//  -------------------------------------------------------------------

//  READ THE PROVIDED coolexe FILE INTO main_memory[]
void read_coolexe_file(char filename[])
{
    memset(main_memory, 0, sizeof main_memory);   //  clear all memory

//  READ CONTENTS OF coolexe
    int file_contents[BUFSIZ];

    FILE *fp = fopen(filename, "rb");

    fread(file_contents, sizeof(IWORD), BUFSIZ, fp);

    fclose(fp);

    int PC = 0;
    for(int m = 0; m <= BUFSIZ; m += 2) {
        //enum INSTRUCTION instr = file_contents[m];
        write_memory(PC, file_contents[m]);
        printf("%i\n", file_contents[m]);
        //printf("%i\n", main_memory[m]);
    }

   /*
   int m = 0;

   main_memory[m] = I_PUSHC;        ++m;
   main_memory[m] = 3;           ++m;

   main_memory[m] = I_PUSHC;        ++m;
   main_memory[m] = 5;           ++m;

   main_memory[m] = I_ADD;          ++m;
   main_memory[m] = I_PRINTI;       ++m;
   main_memory[m] = I_HALT;         ++m;
   */
}

//  -------------------------------------------------------------------

int main(int argc, char *argv[])
{
//  CHECK THE NUMBER OF ARGUMENTS
    if(argc != 2) {
        fprintf(stderr, "Usage: %s program.coolexe\n", argv[0]);
        exit(EXIT_FAILURE);
    }

//  READ THE PROVIDED coolexe FILE INTO THE EMULATED MEMORY
    read_coolexe_file(argv[1]);

//  EXECUTE THE INSTRUCTIONS FOUND IN main_memory[]
    int result = execute_stackmachine();

    report_statistics();

    return result;          // or  exit(result);
}
