#ifndef __DEBUG_CONSOLE_H__
#define __DEBUG_CONSOLE_H__

#include <printf.h>
#include <microcli.h>

MicroCLI_t dbgConsoleCtx;

#define DBG_ERROR(fmt, ...) microcli_error(&dbgConsoleCtx, fmt, ##__VA_ARGS__)
#define DBG_WARN(fmt, ...) microcli_warn(&dbgConsoleCtx, fmt, ##__VA_ARGS__)
#define DBG_LOG(fmt, ...) microcli_log(&dbgConsoleCtx, fmt, ##__VA_ARGS__)
#define DBG_DEBUG(fmt, ...) microcli_debug(&dbgConsoleCtx, fmt, ##__VA_ARGS__)

void debug_console_init(void);
void debug_console_tick(void);

#endif /* __DEBUG_CONSOLE_H__ */
