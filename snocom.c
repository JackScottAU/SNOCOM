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

int main(int argc, char** argv) {

    FILE *bios;

    bios = fopen("bios.rom", "rb");

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
        printf("ip: %#010x\n", memory[instructionpointer]);

        int instruction = decodeInstruction();

        switch(instruction) {
            case 0: // stop
                run = 0;
                printf("Stop\n");
                break;

            default:
                printf("Instruction: %d\n", instruction);
                break;
        }

        instructionpointer++;
    }

    return 0;
}
