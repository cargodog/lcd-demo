#include "board.h"
#include "timer_utils.h"
#include "debug_console.h"
#include "led_blink.h"
#include <ft6x36.h>


extern uint8_t serial_read_single(uint32_t reg);
int main(void)
{
    static const int TICK_PERIOD_MS = 10;
    board_init();
    debug_console_init();

    while(1) {
        // Tick each sub-process
        debug_console_tick();
        blink_tick(TICK_PERIOD_MS);

        // Get touches
        FT6X36Touchpoint_t tp[FT6X36_MAX_TOUCH_POINTS];
        if(ft6x36_get_touches(tp))
            DBG_LOG("x: %d,\r\t\ty: %d\n\r", tp[0].x, tp[0].y);

        // Delay
        delay_ms(TICK_PERIOD_MS);
    }
}