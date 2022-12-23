mkdir -p build/lib/kernel/
mkdir -p build/kernel/
nasm -f elf -o build/lib/kernel/print.o lib/kernel/print.S
nasm -f elf -o build/kernel/kernel.o kernel/kernel.S
gcc -m32 -I lib/kernel/ -fno-stack-protector -c -o build/kernel/main.o kernel/main.c
gcc -m32 -I lib/kernel/ -I kernel/ -c -fno-builtin -fno-stack-protector -o build/kernel/interrupt.o kernel/interrupt.c
gcc -m32 -I lib/kernel/ -I kernel/ -c -fno-builtin -fno-stack-protector -o build/kernel/init.o kernel/init.c
ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin \
build/kernel/main.o build/kernel/init.o build/kernel/interrupt.o build/lib/kernel/print.o build/kernel/kernel.o
dd if=build/kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc