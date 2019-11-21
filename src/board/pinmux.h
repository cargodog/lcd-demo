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
    
    // NO MORE IDs BELOW HERE
    MAX_NUM_PINS
} PinID_t;


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
