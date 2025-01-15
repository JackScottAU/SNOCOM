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
                
            case 12: // store
                int address = decodeAddress();
                memory[address] = accumulator;
                printf("STORE: MEMORY[%d] = %#010x\n", address, accumulator);
                break;

            case 1: // bring
            case 2: // store address
            case 3: // return address
            case 5: // divide
            case 6: // multiply integers
            case 7: // multiply fractions
            case 8: // output
            case 9: // extract
            case 10: // unconditional transfer
            case 11: // conditional transfer
            case 13: // store and clear
            case 14: // add
            case 15: // subtract

            default:
                printf("Instruction: %d\n", instruction);
                break;
        }

        instructionpointer++;
    }

    return 0;
}
