#ifndef _PINMUX_H_
#define _PINMUX_H_

#include "fsl_port.h"
#include <stdint.h>


typedef struct {
    PORT_Type * const port;
    const uint32_t pin;
    const port_pin_config_t cfg;
} PinConfig_t;

// Define usable pin IDs here, to be used in pinmux.
typedef enum {
    // GPIOs
    PIN_STATUS_LED,

    // Debug UART
    PIN_DBG_UART_RX,
    PIN_DBG_UART_TX,

    // QSPI Flash
    PIN_QSPI_FLASH_A_CLK,
    PIN_QSPI_FLASH_A_CS,
    PIN_QSPI_FLASH_A_D0,
    PIN_QSPI_FLASH_A_D1,
    PIN_QSPI_FLASH_A_D2,
    PIN_QSPI_FLASH_A_D3,

    // Touch panel I2C
    PIN_CTP_INT,
    PIN_CTP_RESET,
    PIN_CTP_SDA,
    PIN_CTP_SCL,
    
    // LCD pins
    PIN_LCD_DB0,
    PIN_LCD_DB1,
    PIN_LCD_DB2,
    PIN_LCD_DB3,
    PIN_LCD_DB4,
    PIN_LCD_DB5,
    PIN_LCD_DB6,
    PIN_LCD_DB7,
    PIN_LCD_DB8,
    PIN_LCD_DB9,
    PIN_LCD_DB10,
    PIN_LCD_DB11,
    PIN_LCD_DB12,
    PIN_LCD_DB13,
    PIN_LCD_DB14,
    PIN_LCD_DB15,
    PIN_LCD_RDX,
    PIN_LCD_WRX,
    PIN_LCD_DCX,
    PIN_LCD_CSX,
    PIN_LCD_RESET,

    // NO MORE IDs BELOW HERE
    MAX_NUM_PINS
} PinID_t;
#define PIN_ID_IS_VALID(i) ((i) >= 0 && (i) < MAX_NUM_PINS)


/*
 * Initialize pinmux to default configuration
 */
void initialize_pinmux(void);

/*
 * Apply pinmux configurations
 * 
 * pinmux   array of pinmux configurations
 * count    number of pins in config array
 */
void pinmux_apply(const PinConfig_t * const pinmux, size_t count);


#endif /* _PINMUX_H_ */
