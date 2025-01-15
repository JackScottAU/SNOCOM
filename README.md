# SNOCOM Emulator

Designed to emulate the SNOCOM computer: https://www.pearcey.org.au/blog/2020/snocom-and-cirrus/

8KiB (2048 32-bit words)

Plus accumulator, counter, and instruction pointer.

## Intended Usage

snocom -b biosfile.rom < intape.txt > outtype.txt

biosfile is generally the 3-word autoload sequence

Starts at IP 0.
stdio is used instead of typewriter and paper tape, redirection can be used for file.
