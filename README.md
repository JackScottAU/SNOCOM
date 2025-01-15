# SNOCOM Emulator

Designed to emulate the SNOCOM computer built in 1960 by David Wong from Sydney University for the Snowy Hydro.

8KiB (2048 32-bit words)

Plus accumulator, counter, and instruction pointer.

## Differences between the emulator and the prototype.

Output = in ASCII, only one output device.

Also has the ability to load the machine's memory with a rom file, to avoid having to load via 'paper tape'.

The conditional transfer doesn't use the Z physical switch (because there isn't one), it asks via stdin what to do.

## Intended Usage

snocom -b biosfile.rom < intape.txt > outtype.txt -d 2048

-b overwrites the autoloader with a custom memory image.

-i is the input tape file

-o is the output tape file

-d dumps the contents of memory after the specified number of clock cycles (to aid with debugging)

registerfile.2048
memorydump.2048

typewriter goes to stdout

z switch is on stdin

biosfile is generally the 3-word autoload sequence

Starts at IP 0.
stdio is used instead of typewriter and paper tape, redirection can be used for file.

## Compilation

You'll need a little endian computer, make and gcc.

    make

## License, Contributing, etc

ISC License

## Resources

https://www.pearcey.org.au/blog/2020/snocom-and-cirrus/

https://ses.library.usyd.edu.au/handle/2123/16005

### sexadecimal:

Not as fun as it sounds.

0 to 9 as they are

1010 A = +

1011 B = -

1100 C = N

1101 D = J

1110 E = F

1111 F = L

