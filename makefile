BUILD_DIR = ./build
DISK_IMG = hd60M.img
ENTRY_POINT = 0xc0001500
AS = nasm
CC = gcc
LD = ld
LIB = -I lib/ -I lib/kernel/ -I lib/user/ -I kernel/ -I device/ -I thread/
ASFLAGS = -f elf
ASBINLIB = -I boot/include/
CFLAGS = -m32 -Wall $(LIB) -c -fno-builtin -W -Wstrict-prototypes \
         -Wmissing-prototypes -fno-stack-protector
LDFLAGS = -m elf_i386 -Ttext $(ENTRY_POINT) -e main -Map $(BUILD_DIR)/kernel.map
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/init.o $(BUILD_DIR)/interrupt.o \
      $(BUILD_DIR)/timer.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/print.o \
      $(BUILD_DIR)/debug.o $(BUILD_DIR)/string.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/bitmap.o \
      $(BUILD_DIR)/thread.o $(BUILD_DIR)/list.o $(BUILD_DIR)/switch.o

##############     MBR代码编译     ###############
$(BUILD_DIR)/mbr.bin: boot/mbr.S
	$(AS) $(ASBINLIB) $< -o $@

##############     bootloader代码编译     ###############
$(BUILD_DIR)/loader.bin: boot/loader.S
	$(AS) $(ASBINLIB) $< -o $@

##############     c代码编译     ###############
$(BUILD_DIR)/main.o: kernel/main.c lib/kernel/print.h \
        lib/stdint.h kernel/init.h thread/thread.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c kernel/init.h lib/kernel/print.h \
        lib/stdint.h kernel/interrupt.h device/timer.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.o: kernel/interrupt.c kernel/interrupt.h \
        lib/stdint.h kernel/global.h lib/kernel/io.h lib/kernel/print.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/timer.o: device/timer.c device/timer.h lib/stdint.h\
         lib/kernel/io.h lib/kernel/print.h kernel/debug.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/debug.o: kernel/debug.c kernel/debug.h \
        lib/kernel/print.h lib/stdint.h kernel/interrupt.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/string.o: lib/string.c lib/string.h \
        lib/stdint.h  kernel/debug.h  kernel/global.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/bitmap.o: lib/kernel/bitmap.c lib/kernel/bitmap.h \
    	kernel/global.h lib/stdint.h lib/string.h lib/stdint.h \
     	lib/kernel/print.h kernel/interrupt.h kernel/debug.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/memory.o: kernel/memory.c kernel/memory.h lib/stdint.h lib/kernel/bitmap.h \
   	kernel/global.h kernel/debug.h lib/kernel/print.h \
	lib/kernel/io.h kernel/interrupt.h lib/string.h lib/stdint.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/thread.o: thread/thread.c thread/thread.h lib/stdint.h \
    kernel/global.h lib/string.h kernel/memory.h lib/kernel/list.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/list.o: lib/kernel/list.c lib/kernel/list.h \
	kernel/interrupt.h kernel/global.h
	$(CC) $(CFLAGS) $< -o $@

##############    汇编代码编译    ###############
$(BUILD_DIR)/kernel.o: kernel/kernel.S
	$(AS) $(ASFLAGS) $< -o $@
$(BUILD_DIR)/print.o: lib/kernel/print.S
	$(AS) $(ASFLAGS) $< -o $@
$(BUILD_DIR)/switch.o: thread/switch.S
		$(AS) $(ASFLAGS) $< -o $@

##############    链接所有目标文件    #############
$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY : mk_dir mk_img mbr2hd loader2hd kernel2hd clean all

mk_dir:
	if [ ! -d $(BUILD_DIR) ];then mkdir $(BUILD_DIR);fi

mk_img:
	if [ ! -e $(DISK_IMG) ];then /usr/bin/bximage -hd -mode="flat" -size=60 -q $(DISK_IMG);fi

mbr2hd:
	$(AS) $(ASBINLIB) -o $(BUILD_DIR)/mbr.bin boot/mbr.S
	dd if=$(BUILD_DIR)/mbr.bin of=$(DISK_IMG) bs=512 count=1 conv=notrunc

loader2hd:
	$(AS) $(ASBINLIB) -o $(BUILD_DIR)/loader.bin  boot/loader.S
	dd if=$(BUILD_DIR)/loader.bin of=$(DISK_IMG) bs=512 count=4 seek=2 conv=notrunc
kernel2hd:
	dd if=$(BUILD_DIR)/kernel.bin \
           of=$(DISK_IMG) \
           bs=512 count=200 seek=9 conv=notrunc

clean:
	cd $(BUILD_DIR) && rm -f ./*

build: $(BUILD_DIR)/kernel.bin

run:
	bochs -f bochsrc.disk

all: mk_dir build kernel2hd