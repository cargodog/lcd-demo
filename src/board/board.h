#ifndef _BOARD_H_
#define _BOARD_H_

#include "pinmux.h"
#include <stdbool.h>


#define DBG_UART_MODULE         LPUART0
#define DBG_UART_BAUDRATE       (115200U)
#define DBG_UART_CLK_FREQ       CLOCK_GetFreq(kCLOCK_Osc0ErClk)

#define CTP_I2C_MODULE          I2C3
#define CTP_I2C_BAUDRATE        (200000U)
#define CTP_I2C_CLK_FREQ        CLOCK_GetFreq(I2C3_CLK_SRC)


// initialization
void board_init(void);

// Generic GPIO control
bool board_gpio_read(PinID_t pin);
void board_gpio_set_high(PinID_t pin);
void board_gpio_set_low(PinID_t pin);
void board_gpio_toggle(PinID_t pin);

// Status LED control
static inline void board_status_led_on(void) {board_gpio_set_high(PIN_STATUS_LED);}
static inline void board_status_led_off(void) {board_gpio_set_low(PIN_STATUS_LED);}
static inline void board_status_led_toggle(void) {board_gpio_toggle(PIN_STATUS_LED);}

// Debug UART Interface
int board_dbg_uart_getchar(void);
int board_dbg_uart_getchar_non_blocking(void);
void board_dbg_uart_putchar(char character);


#endif /* _BOARD_H_ */