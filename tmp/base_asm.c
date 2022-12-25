#include <stdio.h>

int in_a = 1, in_b = 2, out_sum;

void base_asm() {
    asm(" pusha;            \
          movl in_a, %eax;  \
          movl in_b, %ebx;  \
          addl %ebx, %eax;  \
          movl %eax, out_sum;  \
          popa");
    printf("sum is %d\n", out_sum);
}

void extent_asm() {
    int in_a = 1, in_b = 2, out_sum;
    asm("addl %%ebx, %%eax":"=a"(out_sum):"a"(in_a), "b"(in_b));
    printf("sum is %d\n", out_sum);
}

void mem() {
    int in_a = 1, in_b = 2;
    printf("in_b is %d\n", in_b);
    asm("movb %b0, %1;"::"a"(in_a), "m"(in_b));
    printf("in_b now is %d\n", in_b);
}

void reg4() {
    int in_a = 0x12345678, in_b = 0;
    asm("movw %1,%0;":"=m"(in_b):"a"(in_a));
    printf("word in_b is 0x%x\n", in_b);
    in_b = 0;
    asm("movb %b1,%0;":"=m"(in_b):"a"(in_a));
    printf("low byte in_b is 0x%x\n", in_b);
    in_b = 0;
    asm("movb %h1,%0;":"=m"(in_b):"a"(in_a));
    printf("high byte in_b is 0x%x\n", in_b);
}

void reg5() {
    int in_a = 18, in_b = 3, out = 0;
    asm("divb %[divisor];movb %%al,%[result]" \
            :[result]"=m"(out)                \
            :"a"(in_a),[divisor]"m"(in_b)     \
    );
    printf("result is %d\n", out);
}

void main() {
//    base_asm();
//    extent_asm();
//    mem();
//    reg4();
    reg5();
}