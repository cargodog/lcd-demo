#include "timer_utils.h"
#include <fsl_clock.h>


uint32_t ms_to_ticks(uint32_t ms)
{
    uint32_t ticks = CLOCK_GetCoreSysClkFreq();
    return ms * (ticks / 1000);
}

void delay_ms(const uint32_t ms)
{
    static const uint32_t loopOverheadTicks = 19;
    volatile uint32_t sysTickCounter = ms_to_ticks(ms);
    while (sysTickCounter > loopOverheadTicks) {
        sysTickCounter -= loopOverheadTicks;
    }
}
