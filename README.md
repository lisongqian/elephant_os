
## 操作系统真象还原练习

### 操作

1. 创建硬盘
```bash
bximage
```
2. 编译MBR
```bash
nasm -I include/ -o boot/mbr.bin mbr.S
```
3. 将MBR写入硬盘
```bash
dd if=boot/mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc
```

4. 编译loader
```bash
nasm -I include/ -o boot/loader.bin loader.S
```

5. 将loader写入硬盘
```bash
dd if=boot/loader.bin of=hd60M.img bs=512 count=1 seek=2 conv=notrunc
```