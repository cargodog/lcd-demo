#include "board.h"
#include "timer_utils.h"
#include "debug_console.h"
#include "led_blink.h"


int main(void)
{
    static const int TICK_PERIOD_MS = 10;
    board_init();
    debug_console_init();

    while(1) {
        // Tick each sub-process
        debug_console_tick();
        blink_tick(TICK_PERIOD_MS);

        // Delay
        delay_ms(TICK_PERIOD_MS);
    }
}