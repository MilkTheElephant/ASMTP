nasm -g src/asmtp.asm -f elf32 -o bin/asmtp.o -i src/&& ld -g bin/asmtp.o -melf_i386 -o bin/asmtp
