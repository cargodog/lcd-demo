#include <assert.h>
#include "printf.h"

void __assert_func (const char *file, int line, const char * func, const char * expr)
{
    printf("Assert in %s():%d (%d)\n\r", func, line, expr);
    __asm__ __volatile__ ("bkpt #0");
    while(1);
}