nasm -I include/ -o boot/mbr.bin mbr.S
nasm -I include/ -o boot/loader.bin  loader.S
dd if=boot/mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc
dd if=boot/loader.bin of=hd60M.img bs=512 count=4 seek=2 conv=notrunc
