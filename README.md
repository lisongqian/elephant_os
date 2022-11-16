
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

### 参考资料

1. [《操作系统真象还原》](https://github.com/yifengyou/os-elephant)
2. [操作系统真象还原 读书笔记 by 我可是会飞的啊](https://www.kn0sky.com/tags/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E7%9C%9F%E8%B1%A1%E8%BF%98%E5%8E%9F)
3. [https://github.com/seaswalker/tiny-os](https://github.com/seaswalker/tiny-os)
4. [os-elephant/《操作系统真象还原》勘误.md](https://github.com/yifengyou/os-elephant/blob/master/doc/%E3%80%8A%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E7%9C%9F%E8%B1%A1%E8%BF%98%E5%8E%9F%E3%80%8B%E5%8B%98%E8%AF%AF.md)