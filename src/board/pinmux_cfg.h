#ifndef _PINMUX_CFG_H_
#define _PINMUX_CFG_H_

#include "pinmux.h"
#include "fsl_port.h"

/*
 * Default pinmux configuration
 */
const PinConfig_t defaultPinCfgs[] = {
    // LED pins
    [PIN_STATUS_LED] = { 
        .port = PORTC,
        .pin = 12U,
        .cfg = {
            .slewRate               = kPORT_SlowSlewRate,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },

    // Debug UART pins
    [PIN_DBG_UART_RX] = { 
        .port = PORTB,
        .pin = 16U,
        .cfg = {
            .pullSelect             = kPORT_PullDown,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt3,
        }
    },
    [PIN_DBG_UART_TX] = { 
        .port = PORTB,
        .pin = 17U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt3,
        }
    },

    // QSPI Flash pins
    [PIN_QSPI_FLASH_A_CLK] = { 
        .port = PORTE,
        .pin = 1U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
    [PIN_QSPI_FLASH_A_CS] = { 
        .port = PORTE,
        .pin = 5U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
    [PIN_QSPI_FLASH_A_D0] = { 
        .port = PORTE,
        .pin = 2U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
    [PIN_QSPI_FLASH_A_D1] = { 
        .port = PORTE,
        .pin = 4U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
    [PIN_QSPI_FLASH_A_D2] = { 
        .port = PORTE,
        .pin = 3U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
    [PIN_QSPI_FLASH_A_D3] = { 
        .port = PORTE,
        .pin = 0U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt5,
        }
    },
};
_Static_assert(sizeof(defaultPinCfgs) / sizeof(defaultPinCfgs[0]) >= MAX_NUM_PINS,
                "Missing pin config for one or more pin IDs!");
_Static_assert(sizeof(defaultPinCfgs) / sizeof(defaultPinCfgs[0]) <= MAX_NUM_PINS,
                "Number of pin configs exceeds number of pin IDs!");


#endif /* _PINMUX_CFG_H_ */
