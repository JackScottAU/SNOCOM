#include <stdio.h>


unsigned int memory[2048];
unsigned int accumulator;
unsigned int counter;
unsigned int instructionpointer = 0;

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

int main(int argc, char** argv) {

    FILE *bios;
    FILE *infile;
    FILE *outfile;

    bios = fopen("bios.rom", "rb");
    infile = fopen("infile.bin", "rb");
    outfile = fopen("outfile.bin", "wb");

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

    printf("Read %d words of ROM.\n", i);

    // main computer loop
    int run = 1;
    while(run) {
        printf("ip: %#010x, instruction: %#010x\n", instructionpointer, memory[instructionpointer]);

        int instruction = decodeInstruction();
        int address = decodeAddress();

        switch(instruction) {
            case 0: // stop
                printf("STOP\n");
                run = 0;
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
                putchar((char)address); // for now assume it's less than 8 bits.
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

            case 2: // store address
            case 3: // return address
            case 5: // divide
            case 6: // multiply integers
            case 7: // multiply fractions
            case 9: // extract
            case 10: // unconditional transfer
            case 11: // conditional transfer - note this has a sub instruction where we will need to wait for user input

            default:
                printf("Instruction: %d\n", instruction);
                break;
        }

        instructionpointer++;
    }

    return 0;
}
