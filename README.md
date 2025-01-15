# SNOCOM Emulator

Designed to emulate the SNOCOM computer: https://www.pearcey.org.au/blog/2020/snocom-and-cirrus/

https://ses.library.usyd.edu.au/handle/2123/16005

8KiB (2048 32-bit words)

Plus accumulator, counter, and instruction pointer.

## Intended Usage

snocom -b biosfile.rom < intape.txt > outtype.txt -d 2048

-d dumps the contents of memory after the specified number of clock cycles (to aid with debugging)

registerfile.2048
memorydump.2048

biosfile is generally the 3-word autoload sequence

Starts at IP 0.
stdio is used instead of typewriter and paper tape, redirection can be used for file.

## sexadecimal:

Not as fun as it sounds.

0 to 9 as they are

1010 A = +

1011 B = -

1100 C = N

1101 D = J

1110 E = F

1111 F = L

