
## 操作系统真象还原练习

### 操作

1. 创建硬盘
```bash
bximage
```
2. 编译MBR
```bash
nasm -I include/ -o mbr.bin mbr.S
```
3. 将MBR写入硬盘
```bash
dd if=./mbr.bin of=hd60M.img bs=512 count=1 conv=notrunc
```