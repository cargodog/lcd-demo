#include "board.h"
#include "ft6x36.h"
#include "qspi_flash.h"
#include "timer_utils.h"
#include "pinmux.h"
#include "pinmux_cfg.h"
#include "gpio_cfg.h"
#include "clock_config.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "fsl_i2c.h"
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
    config.baudRate_Bps = DBG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPUART_Init(DBG_UART_MODULE, &config, DBG_UART_CLK_FREQ);
}

static inline bool ft6x36_read_int_pin(void)
{
    return board_gpio_read(PIN_CTP_INT);
}

static void ft6x36_reset(void)
{
    board_gpio_set_low(PIN_CTP_RESET);
    delay_ms(20); // Trst >= 5ms
    board_gpio_set_high(PIN_CTP_RESET);
    delay_ms(300);
}

static void ft6x36_i2c_transfer(bool write,
                                uint32_t addr,
                                uint32_t reg,
                                void * data,
                                int len)
{
    assert(len >= 0);
    if(len == 0)
        return;
    
    i2c_master_transfer_t masterXfer;
    memset(&masterXfer, 0, sizeof(masterXfer));

    masterXfer.slaveAddress = addr;
    masterXfer.direction = (write) ? kI2C_Write : kI2C_Read;
    masterXfer.subaddress = reg;
    masterXfer.subaddressSize = 1;
    masterXfer.data = (uint8_t *)data;
    masterXfer.dataSize = len;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    assert(kStatus_Success == I2C_MasterTransferBlocking(CTP_I2C_MODULE, &masterXfer));
}

static inline void ft6x36_i2c_read( uint32_t addr,
                                    uint32_t reg,
                                    void * data,
                                    int len)
{
    ft6x36_i2c_transfer(false, addr, reg, data, len);
}

static inline void ft6x36_i2c_write(uint32_t addr,
                                    uint32_t reg,
                                    void * data,
                                    int len)
{
    ft6x36_i2c_transfer(true, addr, reg, data, len);
}

static void initialize_touch_panel(void)
{
    // Setup I2C module
    i2c_master_config_t i2cConfig;
    I2C_MasterGetDefaultConfig(&i2cConfig);
    i2cConfig.baudRate_Bps = CTP_I2C_BAUDRATE;
    I2C_MasterInit(CTP_I2C_MODULE, &i2cConfig, CTP_I2C_CLK_FREQ);

    // Setup touch panel driver
    const FT6X36Config_t ft6x36Config = {
        .fnReadIntPin = ft6x36_read_int_pin,
        .fnResetChip = ft6x36_reset,
        .fnSerialRead = ft6x36_i2c_read,
        .fnSerialWrite = ft6x36_i2c_write,
    };
    ft6326_init(&ft6x36Config);
}

void board_init(void)
{
    clkcfg_run_default();
    initialize_pinmux();
    initialize_gpios();
    initialize_dbg_uart();
    qspi_flash_init();
    initialize_touch_panel();
}

bool board_gpio_read(PinID_t pin)
{
    assert(pin >= 0 && pin < (MAX_NUM_PINS));
    return 0 != GPIO_PinRead(get_gpio_base(pin), defaultPinCfgs[pin].pin);
}

void board_gpio_set_high(PinID_t pin)
{
    assert(pin >= 0 && pin < (MAX_NUM_PINS));
    GPIO_PortSet(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

void board_gpio_set_low(PinID_t pin)
{
    assert(pin >= 0 && pin < (MAX_NUM_PINS));
    GPIO_PortClear(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

void board_gpio_toggle(PinID_t pin)
{
    assert(pin >= 0 && pin < (MAX_NUM_PINS));
    GPIO_PortToggle(get_gpio_base(pin), (1U << defaultPinCfgs[pin].pin));
}

int board_dbg_uart_getchar(void)
{
    char ch;
    LPUART_ReadBlocking(DBG_UART_MODULE, (unsigned char *)&ch, 1);
    return ch;
}

int board_dbg_uart_getchar_non_blocking(void)
{
    if(is_lpuart_data_waiting(DBG_UART_MODULE)) {
        return LPUART_ReadByte(DBG_UART_MODULE);
    } else {
        return 0; // NUL: Nothing to read
    }
}

void board_dbg_uart_putchar(char character)
{
    LPUART_WriteBlocking(DBG_UART_MODULE, (unsigned char *)&character, 1);
}