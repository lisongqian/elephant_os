#include "print.h"

int main() {
    put_char('\n');
    put_char('k');
    put_char('e');
    put_char('r');
    put_char('n');
    put_char('e');
    put_char('l');
    put_char('\n');
    put_char('1');
    put_char('2');
    put_char('\b');
    put_char('3');
    put_str("\nhello kernel\n");
    put_int(0x00000000);
    put_str("\n");
    put_int(0x0000023f);
    while (1);
    return 0;
}