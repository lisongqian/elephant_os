nasm -I boot/include/ -o build/loader.bin  boot/loader.S
dd if=build/loader.bin of=hd60M.img bs=512 count=4 seek=2 conv=notrunc