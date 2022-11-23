gcc -m32 -c -o kernel/main.o kernel/main.c
ld -m elf_i386 kernel/main.o -Ttext 0xc0001500 -e main -o kernel/kernel.bin
dd if=kernel/kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc