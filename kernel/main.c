#include "print.h"
#include "init.h"

void main() {
    put_str("I am kernel\n");
    init_all();
    asm volatile("sti");    //开启中断
    while (1);
}