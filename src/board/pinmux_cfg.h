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

    // QSPI flash pins
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

    // Touch panel I2C
    [PIN_CTP_INT] = { 
        .port = PORTE,
        .pin = 6U,
        .cfg = {
            .slewRate               = kPORT_SlowSlewRate,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_CTP_RESET] = { 
        .port = PORTE,
        .pin = 9U,
        .cfg = {
            .slewRate               = kPORT_SlowSlewRate,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_CTP_SDA] = { 
        .port = PORTE,
        .pin = 10U,
        .cfg = {
            .openDrainEnable        = kPORT_OpenDrainEnable,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt2,
        }
    },
    [PIN_CTP_SCL] = { 
        .port = PORTE,
        .pin = 11U,
        .cfg = {
            .openDrainEnable        = kPORT_OpenDrainEnable,
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAlt2,
        }
    },

    // LCD pins
    [PIN_LCD_DB0] = { 
        .port = PORTB,
        .pin = 21U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB1] = { 
        .port = PORTB,
        .pin = 20U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB2] = { 
        .port = PORTB,
        .pin = 19U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB3] = { 
        .port = PORTB,
        .pin = 18U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB4] = { 
        .port = PORTB,
        .pin = 11U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB5] = { 
        .port = PORTB,
        .pin = 10U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB6] = { 
        .port = PORTB,
        .pin = 3U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB7] = { 
        .port = PORTB,
        .pin = 2U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB8] = { 
        .port = PORTB,
        .pin = 1U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB9] = { 
        .port = PORTB,
        .pin = 0U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB10] = { 
        .port = PORTA,
        .pin = 17U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB11] = { 
        .port = PORTA,
        .pin = 16U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB12] = { 
        .port = PORTA,
        .pin = 15U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB13] = { 
        .port = PORTA,
        .pin = 14U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB14] = { 
        .port = PORTA,
        .pin = 13U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DB15] = { 
        .port = PORTA,
        .pin = 12U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_RDX] = { 
        .port = PORTB,
        .pin = 22U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_WRX] = { 
        .port = PORTB,
        .pin = 23U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_DCX] = { 
        .port = PORTC,
        .pin = 0U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_CSX] = { 
        .port = PORTC,
        .pin = 1U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
    [PIN_LCD_RESET] = { 
        .port = PORTB,
        .pin = 9U,
        .cfg = {
            .driveStrength          = kPORT_HighDriveStrength,
            .mux                    = kPORT_MuxAsGpio,
        }
    },
};
_Static_assert(sizeof(defaultPinCfgs) / sizeof(defaultPinCfgs[0]) >= MAX_NUM_PINS,
                "Missing pin config for one or more pin IDs!");
_Static_assert(sizeof(defaultPinCfgs) / sizeof(defaultPinCfgs[0]) <= MAX_NUM_PINS,
                "Number of pin configs exceeds number of pin IDs!");


#endif /* _PINMUX_CFG_H_ */
