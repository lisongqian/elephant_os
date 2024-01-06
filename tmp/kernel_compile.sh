# 创建生成中间文件目录
mkdir -p build/lib/kernel/
mkdir -p build/kernel/
mkdir -p build/device/
# 编译汇编
nasm -f elf -o build/lib/kernel/print.o lib/kernel/print.S
nasm -f elf -o build/kernel/kernel.o kernel/kernel.S
# 编译C程序
gcc -m32 -I lib/kernel/ -fno-stack-protector -c -o build/device/timer.o device/timer.c
gcc -m32 -I lib/kernel/ -fno-stack-protector -c -o build/kernel/main.o kernel/main.c
gcc -m32 -I lib/kernel/ -I kernel/ -c -fno-builtin -fno-stack-protector -o build/kernel/interrupt.o kernel/interrupt.c
gcc -m32 -I lib/kernel/ -I kernel/ -c -fno-builtin -fno-stack-protector -o build/kernel/init.o kernel/init.c
# 链接目标文件
ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin \
build/kernel/main.o build/kernel/init.o build/kernel/interrupt.o build/lib/kernel/print.o build/kernel/kernel.o build/device/timer.o
# 写入虚拟机磁盘
dd if=build/kernel.bin of=hd60M.img bs=512 count=200 seek=9 conv=notrunc