//  CITS2002 Project 1 2021
//  Name(s):             Sean Peralta Garcia, Cameron Locke 
//  Student number(s):   23088091, 21137282

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


//  EACH WORD OF CACHE MEMORY MUST BE TAGGED WITH A LOCATION, AND AS CLEAN OR DIRTY
struct CACHE_WORD {
    AWORD address;          // Location
    IWORD contents;         //Stored word
    bool clean;             //0 - Dirty, 1 - Clean (default 0/false)
} cache_memory[N_CACHE_WORDS];


//  see:  https://teaching.csse.uwa.edu.au/units/CITS2002/projects/coolinstructions.php
enum INSTRUCTION {
    I_HALT       = 0,
    I_NOP,
    I_ADD,
    I_SUB,
    I_MULT,
    I_DIV,
    I_CALL,
    I_RETURN,
    I_JMP,
    I_JEQ,
    I_PRINTI,
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
int n_main_memory_reads         = 0;
int n_main_memory_writes        = 0;
int n_cache_memory_hits         = 0;
int n_cache_memory_misses       = 0;
int n_number_of_instructions    = 0;

void report_statistics(void)
{
    printf("@number-of-main-memory-reads-(fast-jeq)\t%i\n",    n_main_memory_reads);
    printf("@number-of-main-memory-writes-(fast-jeq)\t%i\n",   n_main_memory_writes);
    printf("@number-of-cache-memory-hits\t%i\n",    n_cache_memory_hits);
    printf("@number-of-cache-memory-misses\t%i\n",  n_cache_memory_misses);
    
    printf("\n");
    printf("@number-of-instructions\t%i\n",  n_number_of_instructions);
}

//  -------------------------------------------------------------------

//  EVEN THOUGH main_memory[] IS AN ARRAY OF WORDS, IT SHOULD NOT BE ACCESSED DIRECTLY.
//  INSTEAD, USE THESE FUNCTIONS read_memory() and write_memory()
//
//  THIS WILL MAKE THINGS EASIER WHEN WHEN EXTENDING THE CODE TO
//  SUPPORT CACHE MEMORY

AWORD read_memory(int address)
{
    int cache_address = address % N_CACHE_WORDS;

    if(cache_memory[cache_address].address == address) {
        n_cache_memory_hits++;
    } else {
        n_cache_memory_misses++;
        n_main_memory_reads++;

        // if byte is dirty then write back to main memory to
        if (cache_memory[cache_address].clean == false) { 
            n_main_memory_writes++;
            int old_address = cache_memory[cache_address].address;
            main_memory[old_address] = cache_memory[cache_address].contents;
        }  

        // update address and read contents of main memory into cache
        cache_memory[cache_address].address  = address;
        cache_memory[cache_address].contents = main_memory[address];

        // byte is now clean
        cache_memory[cache_address].clean = true;
    }

    return cache_memory[cache_address].contents;
}

void write_memory(AWORD address, AWORD value)
{
    int cache_address = address % N_CACHE_WORDS;

    if(cache_memory[cache_address].address == address) {
        cache_memory[cache_address].contents = value;
    } else {

        // if byte is dirty then write back to main memory
        if(cache_memory[cache_address].clean == false) {
            int old_address = cache_memory[cache_address].address;
            main_memory[old_address] = cache_memory[cache_address].contents;
        }

        // update address and contents of cache memory
        cache_memory[cache_address].address  = address;
        cache_memory[cache_address].contents = value;
    }

    // byte is now dirty
    cache_memory[cache_address].clean = false;
}
 
//  -------------------------------------------------------------------

//  EXECUTE THE INSTRUCTIONS IN main_memory[]
int execute_stackmachine(void)
{
//  THE 3 ON-CPU CONTROL REGISTERS:
    int PC      = 0;                    // 1st instruction is at address=0
    int SP      = N_MAIN_MEMORY_WORDS;  // initialised to top-of-stack
    int FP      = 0;                    // frame pointer

    while(true) {
        // incrememnt count number of instructions count
        ++n_number_of_instructions;

        int value1, value2;

        // holds the signed value of an FP-offset
        IWORD signed_pc;


//  FETCH THE NEXT INSTRUCTION TO BE EXECUTED
        IWORD instruction   = read_memory(PC);
        ++PC;

        if(instruction == I_HALT) {
            break;
        }

//  SUPPORT OTHER INSTRUCTIONS HERE 
//      ....
        switch (instruction) {
        case I_NOP :
                            break;
        case I_ADD :
                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value1 + value2);
                            break;
        case I_SUB :
                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value2 - value1);
                            break;
        case I_MULT :
                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value1 * value2);
                            break;
        case I_DIV :
                            value1 = read_memory(SP);
                            ++SP;
                            value2 = read_memory(SP);

                            // result is stored in same memory location as value2 (current value SP)
                            write_memory(SP, value2 / value1);
                            break;
        case I_CALL :
                            // PC curently on callee function-addr
                            // write return address to memory (PC + 1)
                            --SP;
                            write_memory(SP, PC + 1);

                            // write FP of calling procecdure to TOS
                            --SP;
                            write_memory(SP, FP);
                            // assign new FP value (current value of SP)
                            FP = SP;

                            // update PC to new function-addr
                            PC = read_memory(PC);
                            break;
        case I_RETURN :
                            // PC at FP-offset
                            // store new PC
                            value1 = read_memory(FP + 1);

                            // store new SP
                            value2 = FP + read_memory(PC);

                            // write return value from TOS (SP) to Caller's TOS
                            write_memory(value2, read_memory(SP));

                            // set new TOS (SP)
                            SP = value2;

                            // set PC to return address (FP + 1)
                            PC = value1;

                            // reset FP to FP of calling procedure
                            FP = read_memory(FP);
                            break;
        case I_JMP :
                            PC = read_memory(PC);
                            break;
        case I_JEQ :
                            if (read_memory(SP) == 0) {
                                PC = read_memory(PC);
                            } else {
                                ++PC;
                            }
                            ++SP;
                            break;
        case I_PRINTI : ;
                            // convert to signed
                            value1 = read_memory(SP);

                            printf("%i", value1);

                            ++SP;
                            break;
        case I_PRINTS : ;
                            // temp address PC
                            int print_addr = read_memory(PC);

                            // init a char array that holds 2 bytes of chars
                            char string_sections[2];

                            while(true) {
                                int raw = read_memory(print_addr);

                                // read each byte separately (fread??)
                                string_sections[0]=raw%256;
                                string_sections[1]=raw/256;

                                for (int chr = 0; chr < 2; ++chr) {
                                    // if char is not a null byte then print so stdout
                                    putchar(string_sections[chr]);
                                }
     
                                // check if char is a null byte
                                if (string_sections[0] == '\0' || string_sections[1] == '\0') {
                                    break;
                                }

                                ++print_addr;
                            }


                            // increment to next instruction (after start of str addr)
                            ++PC;
                            break;
        case I_PUSHC :
                            --SP;
                            write_memory(SP, read_memory(PC));

                            ++PC;
                            break;
        case I_PUSHA :
                            // hold the address of the integer value to be pushed onto stack
                            value1 = read_memory(PC);
                            --SP;
                            write_memory(SP, read_memory(value1));
                            
                            ++PC;
                            break;
        case I_PUSHR : 
                            // hold signed value if negative
                            signed_pc = read_memory(PC);
                            
                            // holds value to be stored on TOS
                            value1 = read_memory(FP + signed_pc);
                            
                            --SP;
                            write_memory(SP, value1);

                            ++PC;


                            break;
        case I_POPA :
                            // holds value to be popped
                            value1 = read_memory(SP);
                            write_memory(read_memory(PC), value1);
                            ++SP;
                            break;
        case I_POPR : 
                            // hold signed value if negative
                            signed_pc = read_memory(PC);

                            // holds value to be popped
                            value1 = read_memory(SP);
    
                            write_memory(FP + signed_pc, value1);
                            ++SP;
                            ++PC;
                            break;
        }
    }

//  THE RESULT OF EXECUTING THE INSTRUCTIONS IS FOUND ON THE TOP-OF-STACK
    // convert to sigmed
    IWORD result = read_memory(SP);

    return result;
}

//  -------------------------------------------------------------------

//  READ THE PROVIDED coolexe FILE INTO main_memory[]
void read_coolexe_file(char filename[])
{
    memset(main_memory, 0, sizeof main_memory);   //  clear all memory

//  READ CONTENTS OF coolexe
    FILE *fp = fopen(filename, "rb");

    if(fp == NULL) {
        printf("cannot open '%s'\n", filename);
        exit(EXIT_FAILURE);
    } else {
        fread(main_memory, 2, N_MAIN_MEMORY_WORDS, fp);
    }
    fclose(fp);
}

void initialise_cache(void)
{
    for (int memloc = 0; memloc < N_CACHE_WORDS; ++memloc) {
        // initialise all addresses to from bottom of stack - can assume that program data does not exist here
        cache_memory[memloc].address  = N_MAIN_MEMORY_WORDS - 1;
        cache_memory[memloc].clean    = false;
    }
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

// INITIALISE cache
    initialise_cache();

//  EXECUTE THE INSTRUCTIONS FOUND IN main_memory[]
    int result = execute_stackmachine();

    report_statistics();

    printf("\n@exit(%i)\n", result);

    return result;          // or  exit(result);
}
