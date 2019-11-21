#include "pinmux.h"
#include "pinmux_cfg.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include <limits.h>


static void enable_port_clocks(void)
{
    // By default, all port clocks are enabled.
    // To save power, disable any unused port clocks.
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);
}

void initialize_pinmux(void)
{
    enable_port_clocks();
    pinmux_apply(defaultPinCfgs, sizeof(defaultPinCfgs)/sizeof(defaultPinCfgs[0]));
}

void pinmux_apply(const PinConfig_t * const pinmux, size_t count)
{
    enable_port_clocks();
    for(int i = 0; i < count; i++) {
        PORT_SetPinConfig(pinmux[i].port, pinmux[i].pin, &pinmux[i].cfg);
    }
}
