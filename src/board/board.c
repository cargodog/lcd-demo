#include "board.h"
#include "qspi_flash.h"
#include "pinmux.h"
#include "pinmux_cfg.h"
#include "gpio_cfg.h"
#include "clock_config.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include <stdbool.h>


#define INITIALIZE_GPIO_PIN(id, direction, polarity) \
{\
    assert((id) >= 0 && (id) < (MAX_NUM_PINS));\
    GPIO_PinInit(   get_gpio_base(id),\
                    defaultPinCfgs[(id)].pin,\
                    &(const gpio_pin_config_t) {\
                        .pinDirection   = (direction),\
                        .outputLogic    = (polarity)\
                    } );\
}

static inline GPIO_Type * get_gpio_base(PinID_t pin)
{
    if (defaultPinCfgs[pin].port == PORTA)
        return GPIOA;
    if (defaultPinCfgs[pin].port == PORTB)
        return GPIOB;
    if (defaultPinCfgs[pin].port == PORTC)
        return GPIOC;
    if (defaultPinCfgs[pin].port == PORTD)
        return GPIOD;
    if (defaultPinCfgs[pin].port == PORTE)
        return GPIOE;
    assert(0);
}

static inline bool is_lpuart_data_waiting(LPUART_Type *base)
{
    #if defined(FSL_FEATURE_LPUART_HAS_FIFO) && FSL_FEATURE_LPUART_HAS_FIFO
        return !(0U == ((base->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT));
    #else
        return !(0U == (base->STAT & LPUART_STAT_RDRF_MASK));
    #endif
}

static void initialize_gpios(void)
{
    for(int i = 0; i < sizeof(defaultGPIOCfgs)/sizeof(defaultGPIOCfgs[0]); i++) {
        const GPIOConfig_t * const cfg = &defaultGPIOCfgs[i];
        INITIALIZE_GPIO_PIN(cfg->pin, cfg->direction, cfg->defaultOutput);
    }
}

static void initialize_dbg_uart(void)
{
    lpuart_config_t config;

    CLOCK_SetLpuartClock(2U);

    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = DBG_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DBG_LPUART, &config, DBG_LPUART_CLK_FREQ);
}

void board_init(void)
{
    clkcfg_run_default();
    initialize_pinmux();
    initialize_gpios();
    initialize_dbg_uart();
    qspi_flash_init();
}

void board_gpio_set(PinID_t pin)
{
    assert((pin) >= 0 && (pin) < (MAX_NUM_PINS));
    GPIO_PortSet(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

void board_gpio_clear(PinID_t pin)
{
    assert((pin) >= 0 && (pin) < (MAX_NUM_PINS));
    GPIO_PortClear(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

void board_gpio_toggle(PinID_t pin)
{
    assert((pin) >= 0 && (pin) < (MAX_NUM_PINS));
    GPIO_PortToggle(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

int board_dbg_uart_getchar(void)
{
    char ch;
    LPUART_ReadBlocking(DBG_LPUART, (unsigned char *)&ch, 1);
    return ch;
}

int board_dbg_uart_getchar_non_blocking(void)
{
    if(is_lpuart_data_waiting(DBG_LPUART)) {
        return LPUART_ReadByte(DBG_LPUART);
    } else {
        return 0; // NUL: Nothing to read
    }
}

void board_dbg_uart_putchar(char character)
{
    LPUART_WriteBlocking(DBG_LPUART, (unsigned char *)&character, 1);
}