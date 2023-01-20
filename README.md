
## 操作系统真象还原练习

### 

```text
.
├── boot
│   ├── include
│   │   └── boot.inc
│   ├── loader.S
│   └── mbr.S
├── build
│   ├── debug.o
│   ├── init.o
│   ├── interrupt.o
│   ├── kernel.bin
│   ├── kernel.map
│   ├── kernel.o
│   ├── loader.bin
│   ├── main.o
│   ├── mbr.bin
│   ├── print.o
│   └── timer.o
├── device
│   ├── timer.c
│   └── timer.h
├── include
│   └── boot.inc
├── kernel
│   ├── debug.c
│   ├── debug.h
│   ├── global.h
│   ├── init.c
│   ├── init.h
│   ├── interrupt.c
│   ├── interrupt.h
│   ├── kernel.S
│   ├── main.c
├── lib
│   ├── kernel
│   │   ├── io.h
│   │   ├── print.h
│   │   └── print.S
│   ├── stdint.h
│   ├── string.c
│   ├── string.h
│   └── user
├── tmp
│   ├── base_asm.c
│   ├── header.bin
│   ├── header.S
│   ├── kernel_compile.sh
│   ├── loader_compile.sh
│   ├── makefile
│   ├── mbr_compile.sh
│   ├── run_test
│   ├── syscall_write_demo.sh
│   ├── syscall_write.S
│   └── test_compile.sh
├── bochsrc.disk
├── hd60M.img
├── log_bochs.txt
├── makefile
├── README.md
├── start_bochs.sh
└── xxd.sh
```

### 操作说明

1. `make mbr2hd`编译MBR
1. `make kernel2hd`编译LOADER
2. `make all`编译KERNEL
3. `make clean`清理中间文件
4. `./start_bochs.sh`启动bochs调试

### 参考资料

1. [《操作系统真象还原》](https://github.com/yifengyou/os-elephant)
2. [操作系统真象还原 读书笔记 by 我可是会飞的啊](https://www.kn0sky.com/tags/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E7%9C%9F%E8%B1%A1%E8%BF%98%E5%8E%9F)
3. [https://github.com/seaswalker/tiny-os](https://github.com/seaswalker/tiny-os)
4. [os-elephant/《操作系统真象还原》勘误.md](https://github.com/yifengyou/os-elephant/blob/master/doc/%E3%80%8A%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%E7%9C%9F%E8%B1%A1%E8%BF%98%E5%8E%9F%E3%80%8B%E5%8B%98%E8%AF%AF.md)