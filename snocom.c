#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int memory[2048];
unsigned int accumulator = 0;
unsigned int instructionpointer = 0; // called the instruction counter in documentation.

int decodeInstruction() {
    int word = memory[instructionpointer];

    int instruction = (word & 0x00F00000) >> 20;

    return instruction;
}

int decodeAddress() {
    int word = memory[instructionpointer];

    int address = (word & 0x00007FF0) >> 4;

    return address;
}

int decodeAddressOfAccumulator() {
    int word = accumulator;

    int address = (word & 0x00007FF0) >> 4;

    return address;
}

int main(int argc, char** argv) {

    int debuggingMode = 0;

    char* romFileName = NULL;
    char* inTapeName = NULL;
    char* outTapeName = NULL;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-v") == 0) {
            printf("SNOCOM Emulator v0.0.1\n");
            exit(0);
        }

        if(strcmp(argv[i], "-h") == 0) {
            printf("SNOCOM Emulator v0.0.1\n");
            printf("Usage: snocom -i INPUTTAPE -o OUTPUTTAPE [-r ROMFILE]\n");
            exit(0);
        }

        if(strcmp(argv[i], "-d") == 0) {
            debuggingMode = 1;
        }

        if(strcmp(argv[i], "-r") == 0) {
            romFileName = argv[i+1];
        }

        if(strcmp(argv[i], "-i") == 0) {
            inTapeName = argv[i+1];
        }

        if(strcmp(argv[i], "-o") == 0) {
            outTapeName = argv[i+1];
        }
    }


    FILE *infile = fopen(inTapeName, "rb");
    FILE *outfile = fopen(outTapeName, "wb");


    if(infile == NULL) {
        printf("No input tape found.\n");
        return -1;
    }

    if(outfile == NULL) {
        printf("No output tape found.\n");
        return -1;
    }

    
    if(romFileName != NULL) 
    {
        FILE *bios = fopen(romFileName, "rb");

        if(bios == NULL) {
            printf("No BIOS found.\n");
            return -1;
        }
        // load bios into rom.
        int i = 0;
        while(1)
        {

            int readwords = fread(&memory[i], sizeof(unsigned int), 1, bios); 

            if(!readwords) {
                break;
            }

            i++;
        }
        fclose(bios); 

        if(debuggingMode) {
            printf("Read %d words of ROM.\n", i);
        }
    } 
    else 
    {
        // load tape autoloader into memory 
        memory[0] = 0x00400000;
        memory[1] = 0x00C00030;
        memory[2] = 0x00400000;

    }

    

    // main computer loop
    int run = 1;
    int breakpoint = 0;
    while(run) {
      //  printf("ip: %#010x, instruction: %#010x\n", instructionpointer, memory[instructionpointer]);

        int instruction = decodeInstruction();
        int address = decodeAddress();

        if(breakpoint) {
            printf("BREAKPOINT. IP: %#010x. ACC: %#010x\n", instructionpointer, accumulator);
            printf("[S]ingle Step, [C]ontinue, or [Q]uit? ");

            char a;
            scanf("%c", &a);
            printf("\n");

            switch(a) {
                case 'Q':
                case 'q':
                    run = 0;
                    break;

                case 'C':
                case 'c':
                    breakpoint = 0;
                    break;

                case 'S':
                case 's':
                default:
                    // do nothing here.
                    break;
            }
        }

    // to give us the option of exiting after a breakpoint.
        if(run) {
            switch(instruction) {
            case 0: // stop - TODO: this is actually supposed to be a breakpoint.
                
                if(debuggingMode) {
                    breakpoint = 1;
                } else {
                    run = 0;
                }

                break;

            
            case 4: // input
                int temp, success;
                success = fread(&temp, sizeof(unsigned int), 1, infile);
                if(!success) {
                    printf("INPUT FAILURE\n");
                    run = 0;
                } else {
                    accumulator = temp;
                    printf("INPUT: ACCUMULATOR = %#010x\n", accumulator);
                }
                break;

            case 8: // output
                char output = (char)(address & 0xFF);

                if(memory[instructionpointer] & 0x80000000) {
                    // z-mode = typewriter
                    putchar(output); // for now assume it's less than 8 bits.
                } else {
                    fwrite(&output, 1, 1, outfile);
                }
              //  printf("%d\n", (address >> 4) & 0xFF);
                
                break;
                
            case 12: // store
                memory[address] = accumulator;
                printf("STORE: MEMORY[%d] = %#010x\n", address, accumulator);
                break;

            case 13: // store and clear
                memory[address] = accumulator;
                accumulator = 0;
                printf("STORE AND CLEAR: MEMORY[%d] = %#010x\n", address, accumulator);
                break;

            case 1: // bring (load)
                accumulator = memory[address];
                printf("BRING: ACCUMULATOR = %#010x\n", accumulator);
                break;
                
            case 14: // add - TODO: fix this to use the actual machine arithmetic
                accumulator = memory[address] + accumulator;
                printf("ADD: ACCUMULATOR = %#010x\n", accumulator);
                break;
                
            case 15: // subtract - TODO: fix this to use the actual machine arithmetic
                accumulator = accumulator - memory[address];
                printf("SUB: ACCUMULATOR = %#010x\n", accumulator);
                break;

                
            case 10: // unconditional transfer
                instructionpointer = address;
                printf("JUMP\n");
                break;
                
            case 11: // conditional transfer - note this has a sub instruction where we will need to wait for user input
                if(instruction & 0x80000000) {
                    // Z mode
                    printf("Do Z-mode jump (y/n)? ");
                    char y; scanf("%c", &y);
                    if(y == 'y' || y == 'Y') {
                        instructionpointer = address;
                    }
                }

                if(accumulator & 0x80000000) {
                    instructionpointer = address;
                }
                printf("Jump if negative\n");
                break;

            case 9: // extract
                accumulator = accumulator & memory[address];
                printf("extract product (and)\n");
                break;

            case 2: // store address Store Address. Replace the address digits of the 
//word in location ABC by the address digits of the word 
//in the accumulator, leaving the accumulator and the 
//remaining digits of ABC unchanged.
                int mem = memory[address] & 0xFFFF800F;
                memory[address] = mem | (address << 4);
                printf("updated address of memory with address from accumulator\n");
                break;


            case 3: // return addressReturn Address. Replace the address digits of the 
//word in location ABC by the address digits of the 
//instruction counter augmented by “ one ”, leaving the 
//instruction counter and the remaining digits of ABC 
//unchanged. (During the execution of this instruction, 
//the instruction counter holds the address of the next 
//instruction to be executed.)
                memory[address] = mem | ((instructionpointer + 2) << 4);
                printf("set return address\n");

            case 5: // divide
                accumulator = memory[address] / accumulator;
                printf("DIV: ACCUMULATOR = %#010x\n", accumulator);
                break;

            case 6: // multiply integers
                accumulator = memory[address] * accumulator;
                printf("MUL: ACCUMULATOR = %#010x\n", accumulator);
                break;

            case 7: // multiply fractions
                printf("INCOMPLETE INSTRUCTION (multiple fraction)");
                break;

            default:
                printf("Instruction: %d\n", instruction);
                break;
        }
        }
        

        instructionpointer++;
    }

    return 0;
}
