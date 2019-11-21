#include "printf.h"
#include "board.h"


void _putchar(char character)
{
    board_dbg_uart_putchar(character);
}
