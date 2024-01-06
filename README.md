
## 操作系统真象还原练习

### 

```text
.
├── boot                # 系统引导目录
│   ├── include
│   │   └── boot.inc
│   ├── loader.S
│   └── mbr.S
├── build               # 编译目录
├── device              # 设备管理目录
│   ├── timer.c
│   └── timer.h
├── kernel              # 内核实现目录
│   ├── debug.c
│   ├── debug.h
│   ├── global.h
│   ├── init.c
│   ├── init.h
│   ├── interrupt.c
│   ├── interrupt.h
│   ├── kernel.S
│   ├── main.c
│   ├── memory.c
│   ├── memory.h
├── lib                 # 通用类目录
│   ├── kernel
│   │   ├── bitmap.c
│   │   ├── bitmap.h
│   │   ├── io.h
│   │   ├── print.h
│   │   └── print.S
│   ├── stdint.h
│   ├── string.c
│   ├── string.h
│   └── user
├── tmp                 # 测试练习文件
├── bochsrc.disk        # bochs参数文件
├── hd60M.img           # 虚拟硬盘文件
├── log_bochs.txt       # bochs日志文件
├── makefile
├── README.md
├── start_bochs.sh      # 启动bochs脚本文件
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