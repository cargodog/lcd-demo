#include "led_blink.h"
#include "board.h"
#include <stdbool.h>
#include <assert.h>


bool blinkEnable = true;


void blink_enable(void)
{
    blinkEnable = true;
}

void blink_disable(void)
{
    blinkEnable = false;
}

void blink_tick(int ms)
{
    static const int BLINK_PERIOD_MS = 1000;
    static int msCounter = 0;

    assert(ms > 0);

    msCounter += ms;

    if(msCounter > BLINK_PERIOD_MS && blinkEnable == true) {
        board_status_led_toggle();
        msCounter = 0;
    }
}
