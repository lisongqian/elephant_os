nasm -I include/ -o boot/mbr.bin boot/mbr.S
dd if=boot/mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc