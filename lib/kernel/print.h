#ifndef __LIB_KERNEL_PRINT_H
#define __LIB_KERNEL_PRINT_H
#include "stdint.h"
void put_char(unsigned char char_asci);
void put_str(char* message);
void put_int(unsigned int num);
void set_cursor(uint32_t cursor_pos);
#endif //__LIB_KERNEL_PRINT_H
