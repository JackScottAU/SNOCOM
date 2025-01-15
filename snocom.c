#include <stdio.h>


unsigned int memory[2048];
unsigned int accumulator;
unsigned int counter;
unsigned int instructionpointer;

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

    return 0;
}
