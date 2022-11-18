gcc -c -o kernel/main.o kernel/main.c
ld kernel/main.o -Ttext 0xc0001500 -e main -o kernel/kernel.bin