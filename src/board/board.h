#ifndef _BOARD_H_
#define _BOARD_H_

#include "pinmux.h"
#include <stdbool.h>


#define DBG_LPUART              LPUART0
#define DBG_BAUDRATE            115200
#define DBG_LPUART_CLK_FREQ     CLOCK_GetFreq(kCLOCK_Osc0ErClk)


// initialization
void board_init(void);

// Generic GPIO control
void board_gpio_set(PinID_t pin);
void board_gpio_clear(PinID_t pin);
void board_gpio_toggle(PinID_t pin);

// Status LED control
static inline void board_status_led_on(void) {board_gpio_set(PIN_STATUS_LED);}
static inline void board_status_led_off(void) {board_gpio_clear(PIN_STATUS_LED);}
static inline void board_status_led_toggle(void) {board_gpio_toggle(PIN_STATUS_LED);}

// Debug UART Interface
int board_dbg_uart_getchar(void);
int board_dbg_uart_getchar_non_blocking(void);
void board_dbg_uart_putchar(char character);


#endif /* _BOARD_H_ */