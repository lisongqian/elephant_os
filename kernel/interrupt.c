#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"
#include "print.h"

#define PIC_M_CTRL 0x20           // 这里用的可编程中断控制器是8259A,主片的控制端口是0x20
#define PIC_M_DATA 0x21           // 主片的数据端口是0x21
#define PIC_S_CTRL 0xa0           // 从片的控制端口是0xa0
#define PIC_S_DATA 0xa1           // 从片的数据端口是0xa1

#define IDT_DESC_CNT 0x30     // 目前总共支持的中断数

#define EFLAGS_IF   0x00000200       // eflags寄存器中的if位为1
#define GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g" (EFLAG_VAR))
/**
 * 中断门描述符结构体
 */
struct gate_desc {
    uint16_t func_offset_low_word;
    uint16_t selector;
    uint8_t dcount;   //此项为双字计数字段，是门描述符中的第4字节。此项固定值，不用考虑
    uint8_t attribute;
    uint16_t func_offset_high_word;
};

static struct gate_desc idt[IDT_DESC_CNT];   // idt是中断描述符表,本质上就是个中断门描述符数组

char* intr_name[IDT_DESC_CNT];             // 用于保存异常的名字
intr_handler idt_table[IDT_DESC_CNT];         // 定义中断处理程序数组.在kernel.S中定义的intrXXentry只是中断处理程序的入口,最终调用的是ide_table中的处理程序
extern intr_handler intr_entry_table[IDT_DESC_CNT];        // 声明引用定义在kernel.S中的中断处理函数入口数组

/**
 * @description 初始化可编程中断控制器8259A
 */
static void pic_init(void) {
    //初始化主片
    outb(PIC_M_CTRL, 0x11);         //ICW1: 0001 0001 ,边沿触发，级联 8259，需要ICW4
    outb(PIC_M_DATA, 0x20);         //ICW2: 0010 0000 ,起始中断向量号为 0x20(0x20-0x27)
    outb(PIC_M_DATA, 0x04);         //ICW3: 0000 0100 ,IR2 接从片
    outb(PIC_M_DATA, 0x01);         //ICW4: 0000 0001 ,8086 模式，正常EOI

    //初始化从片
    outb(PIC_S_CTRL, 0x11);         //ICW1: 0001 0001 ,边沿触发，级联 8259，需要ICW4
    outb(PIC_S_DATA, 0x28);         //ICW2: 0010 1000 ,起始中断向量号为 0x28(0x28-0x2f)
    outb(PIC_S_DATA, 0x02);         //ICW3: 0000 0010 ,设置连接到主片的 IR2 引脚
    outb(PIC_S_DATA, 0x01);         //ICW4: 0000 0001 ,8086 模式，正常EOI

    //eflags 里的 IF 位对所有外部中断有效，但不能屏蔽某个外设的中断了
    //测试键盘，只打开中断，其他全部关闭
    outb(PIC_M_DATA, 0xfd);        //OCW1: 1111 1101
    outb(PIC_S_DATA, 0xff);        //OCW1: 1111 1111

    put_str("    pic init done\n");
}

/**
 * @description 创建中断门描述符, 向中断描述符填充属性和地址
 * @param p_gdesc 中断描述符
 * @param attr 属性
 * @param function 中断处理函数地址
 */
static void make_idt_desc(struct gate_desc* p_gdesc, uint8_t attr, intr_handler function) {
    p_gdesc->func_offset_low_word = (uint32_t) function & 0x0000FFFF;
    p_gdesc->selector = SELECTOR_K_CODE;
    p_gdesc->dcount = 0;
    p_gdesc->attribute = attr;
    p_gdesc->func_offset_high_word = ((uint32_t) function & 0xFFFF0000) >> 16;
}

/**
 * @description 初始化中断描述符表
 */
static void idt_desc_init(void) {
    int i;
    for (i = 0; i < IDT_DESC_CNT; i++) {
        make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
    }
    put_str("   idt_desc_init done\n");
}

/**
 * @description 通用的中断处理函数,一般用在异常出现时的处理
 * @param vec_nr 中断向量号
 */
static void general_intr_handler(uint8_t vec_nr) {
    if (vec_nr == 0x27 || vec_nr == 0x2f) {    // 0x2f是从片8259A上的最后一个irq引脚，保留
        return;        //IRQ7和IRQ15会产生伪中断(spurious interrupt),无须处理。
    }
    /* 将光标置为0,从屏幕左上角清出一片打印异常信息的区域,方便阅读 */
    set_cursor(0);
    int cursor_pos = 0;
    while (cursor_pos < 320) {
        put_char(' ');
        cursor_pos++;
    }

    set_cursor(0);     // 重置光标为屏幕左上角
    put_str("!!!!!!!      excetion message begin  !!!!!!!!\n");
    set_cursor(88);    // 从第2行第8个字符开始打印
    put_str(intr_name[vec_nr]);
    if (vec_nr == 14) {      // 若为Pagefault,将缺失的地址打印出来并悬停
        int page_fault_vaddr = 0;
        asm ("movl %%cr2, %0" : "=r" (page_fault_vaddr));      // cr2是存放造成page_fault的地址
        put_str("\npage fault addr is ");
        put_int(page_fault_vaddr);
    }
    put_str("\n!!!!!!!      excetion message end    !!!!!!!!\n");
    // 能进入中断处理程序就表示已经处在关中断情况下,
    // 不会出现调度进程的情况。故下面的死循环不会再被中断。
    while (1);
}

/**
 * @description 完成一般中断处理函数注册及异常名称注册
 */
static void exception_init(void) {
    int i;
    for (i = 0; i < IDT_DESC_CNT; i++) {

/* idt_table数组中的函数是在进入中断后根据中断向量号调用的,
 * 见kernel/kernel.S的call [idt_table + %1*4] */
        idt_table[i] = general_intr_handler;            // 默认为general_intr_handler。
        // 以后会由register_handler来注册具体处理函数。
        intr_name[i] = "unknown";                    // 先统一赋值为unknown
    }
    intr_name[0] = "#DE Divide Error";
    intr_name[1] = "#DB Debug Exception";
    intr_name[2] = "NMI Interrupt";
    intr_name[3] = "#BP Breakpoint Exception";
    intr_name[4] = "#OF Overflow Exception";
    intr_name[5] = "#BR BOUND Range Exceeded Exception";
    intr_name[6] = "#UD Invalid Opcode Exception";
    intr_name[7] = "#NM Device Not Available Exception";
    intr_name[8] = "#DF Double Fault Exception";
    intr_name[9] = "Coprocessor Segment Overrun";
    intr_name[10] = "#TS Invalid TSS Exception";
    intr_name[11] = "#NP Segment Not Present";
    intr_name[12] = "#SS Stack Fault Exception";
    intr_name[13] = "#GP General Protection Exception";
    intr_name[14] = "#PF Page-Fault Exception";
    // intr_name[15] 第15项是intel保留项，未使用
    intr_name[16] = "#MF x87 FPU Floating-Point Error";
    intr_name[17] = "#AC Alignment Check Exception";
    intr_name[18] = "#MC Machine-Check Exception";
    intr_name[19] = "#XF SIMD Floating-Point Exception";
}

/**
 * @description 开中断
 * @return 中断前状态
 */
enum intr_status intr_enable() {
    if (INTR_ON == intr_get_status()) {
        return INTR_ON;
    }
    else {
        asm volatile("sti");     // 开中断,sti指令将IF位置1
        return INTR_OFF;
    }
}

/**
 * @description 关中断
 * @return 中断前的状态
 */
enum intr_status intr_disable() {
    if (INTR_ON == intr_get_status()) {
        asm volatile("cli" : : : "memory"); // 关中断,cli指令将IF位置0
        return INTR_ON;
    }
    else {
        return INTR_OFF;
    }
}

/**
 * 将中断状态设置为status
 * @param status 中断状态
 * @return 中断前状态
 */
enum intr_status intr_set_status(enum intr_status status) {
    return status & INTR_ON ? intr_enable() : intr_disable();
}

/**
 * @description 获取当前中断状态
 * @return 当前中断状态
 */
enum intr_status intr_get_status() {
    uint32_t eflags = 0;
    GET_EFLAGS(eflags);
    return (EFLAGS_IF & eflags) ? INTR_ON : INTR_OFF;
}

/**
 * 为中断注册中断处理程序
 * @param vector_no 中断向量号
 * @param function 中断处理程序
 */
void register_handler(uint8_t vector_no, intr_handler function) {
/* idt_table数组中的函数是在进入中断后根据中断向量号调用的,
 * 见kernel/kernel.S的call [idt_table + %1*4] */
    idt_table[vector_no] = function;
}

/**
 * @description 完成有关中断的所有初始化工作
 */
void idt_init() {
    put_str("idt_init start\n");
    idt_desc_init();       // 初始化中断描述符表
    exception_init();       // 异常名初始化并注册通常的中断处理函数
    pic_init();           // 初始化8259A
    /* 加载idt */
    uint64_t
    idt_operand = ((sizeof(idt) - 1) | ((uint64_t)(uint32_t)
    idt << 16));
    asm volatile("lidt %0" : : "m" (idt_operand));
    put_str("idt_init done\n");
}
