#include "debug_console.h"
#include <printf.h>
#include <microcli.h>
#include "led_blink.h"
#include "board.h"


#define CMD_ENTRY(fn, help) {fn, #fn, help}


// Registered CLI commands
int help(MicroCLI_t * ctx, const char * args) {return microcli_help(&dbgConsoleCtx);}
int ledOff(MicroCLI_t * ctx, const char * args) {board_status_led_off(); return 0;}
int ledOn(MicroCLI_t * ctx, const char * args) {board_status_led_on(); return 0;}
int startBlink(MicroCLI_t * ctx, const char * args) {blink_enable(); return 0;}
int stopBlink(MicroCLI_t * ctx, const char * args) {blink_disable(); return 0;}


const MicroCLICmdEntry_t cmdTable[] = {
    CMD_ENTRY(help,             "Print this help message"),
    CMD_ENTRY(ledOff,           "Turn off the LED"),
    CMD_ENTRY(ledOn,            "Turn on the LED"),
    CMD_ENTRY(startBlink,       "Start blinking the LED"),
    CMD_ENTRY(stopBlink,        "Stop blinking the LED"),
};

const MicroCLICfg_t dbgCfg = {
    .bannerText = "\r\n\n\n\nKinetis MCU Demo\r\n\n",
    .promptText = "> ",
    .cmdTable = cmdTable,
    .cmdCount = sizeof(cmdTable)/sizeof(cmdTable[0]),
    .io.printf = printf,
    .io.getchar = board_dbg_uart_getchar_non_blocking,
};

void debug_console_init(void)
{
    microcli_init(&dbgConsoleCtx, &dbgCfg);
    microcli_banner(&dbgConsoleCtx);
    microcli_interpret_string(&dbgConsoleCtx, "help", false);
}

void debug_console_tick(void)
{
    microcli_interpreter_tick(&dbgConsoleCtx);
}
