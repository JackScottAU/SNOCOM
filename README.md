# SNOCOM Emulator

Designed to emulate the SNOCOM computer built in 1960 by David Wong from Sydney University for the Snowy Hydro.

8KiB (2048 32-bit words)

Plus accumulator and instruction pointer.

## Goals

1.  Write a fully functional SNOCOM emulator.
2.  Write an assembler for SNOCOM.
3.  Get remaining programs from the NMA into ROM and tape files.
4.  Build a physical SNOCOM.

## Current Status

Runs a hello world program from rom.

## Differences between the emulator and the prototype.

Input instruction: need a way to differentiate between nibble reads and word reads.
Perhaps add an operand: if 0 then read 1 byte, discard top nibble and store bottom nibble
if 1 then read whole word. Use z-mode flag to determine how much to write.

Output = writes bytes. Bottom 8 of address are what to output. Uses Z-mode flag to determine which output device to use (Z set = typewriter, stdout)

These two changes mean that programs restored from punch tape will need some changes in order to work... but they needed changes to work anyway.

Also has the ability to load the machine's memory with a rom file, to avoid having to load via 'paper tape'.

The conditional transfer doesn't use the Z physical switch (because there isn't one), it asks via stdin what to do.

## Usage

    snocom [-r biosfile.rom] -i intape.txt -o outtype.txt -d

-r overwrites the tape autoloader with a custom memory image. Without a rom image specified, it will use Wong's tape autoloader as specified in his thesis.

-i is the input tape file (required)

-o is the output tape file (required)

-d specifies debugging mode, where a STOP instruction will enter the debugger rather than quitting.

-v shows version info

-h shows help info

typewriter goes to stdout

### Samples

helloworld.bin prints "Hello world" to output tape.

helloworld_typewriter.bin does the same thing to the stdout typewriter.

## Compilation

You'll need a little endian computer, make and gcc.

    make

## License, Contributing, etc

ISC License

Pull requests welcome.

## Resources

https://www.pearcey.org.au/blog/2020/snocom-and-cirrus/

https://ses.library.usyd.edu.au/handle/2123/16005

### Sexadecimal

Not as fun as it sounds.

0 to 9 as they are

| Binary | Normal Hexadecimal | Wong's Sexadecimal |
| ------ | ------------------ | ------------------ |
| 1010   | A                  | +                  |
| 1011   | B                  | -                  |
| 1100   | C                  | N                  |
| 1101   | D                  | J                  |
| 1110   | E                  | F                  |
| 1111   | F                  | L                  |
