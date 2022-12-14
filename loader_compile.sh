nasm -I boot/include/ -o boot/loader.bin  boot/loader.S
dd if=boot/loader.bin of=hd60M.img bs=512 count=4 seek=2 conv=notrunc