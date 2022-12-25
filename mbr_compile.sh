nasm -I boot/include/ -o build/mbr.bin boot/mbr.S
dd if=build/mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc