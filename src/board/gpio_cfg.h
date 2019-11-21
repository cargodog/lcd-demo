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
    {PIN_STATUS_LED, kGPIO_DigitalOutput, 0U},
};


#endif /* _GPIO_CFG_H_ */