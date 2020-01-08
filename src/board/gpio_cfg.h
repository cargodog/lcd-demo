#ifndef _GPIO_CFG_H_
#define _GPIO_CFG_H_

#include "pinmux.h"
#include "fsl_gpio.h"

typedef struct {
    PinID_t pin;
    gpio_pin_direction_t direction;
    uint8_t defaultOutput;
} GPIOConfig_t;


/*
 * Default gpio pin configurations
 */
const GPIOConfig_t defaultGPIOCfgs[] = {
    {PIN_STATUS_LED,    kGPIO_DigitalOutput,    0U},
    {PIN_CTP_INT,       kGPIO_DigitalInput,     1U},
    {PIN_CTP_RESET,     kGPIO_DigitalOutput,    0U},
    {PIN_LCD_RDX,       kGPIO_DigitalOutput,    0U},
    {PIN_LCD_WRX,       kGPIO_DigitalOutput,    0U},
    {PIN_LCD_DCX,       kGPIO_DigitalOutput,    0U},
    {PIN_LCD_CSX,       kGPIO_DigitalOutput,    0U},
    {PIN_LCD_RESET,     kGPIO_DigitalOutput,    0U},
};


#endif /* _GPIO_CFG_H_ */
