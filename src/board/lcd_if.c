#include "lcd_if.h"
#include "board.h"
#include "pinmux.h"
#include "timer_utils.h"
#include "fsl_port.h"
#include "fsl_gpio.h"


typedef struct {
    uint8_t csx :1;
    uint8_t dcx :1;
    uint8_t wrx :1;
    uint8_t rdx :1;
} LCDProtoSignals_t;

typedef struct {
    LCDProtoSignals_t toggle;
    uint32_t          msDelay;
} LCDProtoStep_t;


static uint8_t         command;
static const uint16_t  *write;
static uint32_t        writeCnt;
static uint16_t        *read;
static uint32_t        readCnt;

// All signals start high. Each protocol step defines a set
// of signals to toggle, and a delay until the next step.

const LCDProtoStep_t lcdProtoStart[] = {
    {.msDelay = 1,  .toggle = {.csx = 1, .dcx = 1,}},
    {.msDelay = 1,  .toggle = {.wrx = 1,}},
    {.msDelay = 1,  .toggle = {.wrx = 1,}},
    {.msDelay = 1,  .toggle = {.dcx = 1,}},
};
#define PROTO_START()\
    lcd_proto_run(lcdProtoStart, sizeof(lcdProtoStart)/sizeof(lcdProtoStart[0]))

const LCDProtoStep_t lcdProtoWrite[] = {
    {.msDelay = 1,  .toggle = {.wrx = 1,}},
    {.msDelay = 1,  .toggle = {.wrx = 1,}},
};
#define PROTO_WRITE()\
    lcd_proto_run(lcdProtoWrite, sizeof(lcdProtoWrite)/sizeof(lcdProtoWrite[0]))

const LCDProtoStep_t lcdProtoRead[] = {
    {.msDelay = 1,  .toggle = {.rdx = 1,}},
    {.msDelay = 1,  .toggle = {.rdx = 1,}},
};
#define PROTO_READ()\
    lcd_proto_run(lcdProtoRead, sizeof(lcdProtoRead)/sizeof(lcdProtoRead[0]))

const LCDProtoStep_t lcdProtoStop[] = {
    {.msDelay = 1,  .toggle = {.csx = 1,}},
};
#define PROTO_STOP()\
    lcd_proto_run(lcdProtoStop, sizeof(lcdProtoStop)/sizeof(lcdProtoStop[0]))


static inline void lcd_proto_reset(void)
{
    board_gpio_set_high(PIN_LCD_CSX);
    board_gpio_set_high(PIN_LCD_RESET);
    board_gpio_set_high(PIN_LCD_DCX);
    board_gpio_set_high(PIN_LCD_WRX);
    board_gpio_set_high(PIN_LCD_RDX);
}   

static void load_cmd(void)
{
    for(int i = 0; i < 8; i++) {
        if(command & (1 << i)) {
            board_gpio_set_high(PIN_LCD_DB0 + i);
        } else {
            board_gpio_set_low(PIN_LCD_DB0 + i);
        }
    }
}

static void load_data(void)
{
    if(!write)
        return;

    for(int i = 0; i < sizeof(*write); i++) {
        if(*write & (1 << i)) {
            board_gpio_set_high(PIN_LCD_DB0 + i);
        } else {
            board_gpio_set_low(PIN_LCD_DB0 + i);
        }
    }
}

static void save_data(void)
{
    if(!read)
        return;

    *read = 0;
    for(int i = 0; i < 8*sizeof(*read); i++)
        if(board_gpio_read(PIN_LCD_DB0 + i))
            *read |= (1 << i);
}

static void lcd_proto_run(const LCDProtoStep_t * step, uint32_t n)
{
    if(!step)
        return; // noop

    for(int i = 0; i < n; i++) {
        if(step[i].toggle.csx)
            board_gpio_toggle(PIN_LCD_CSX);
        if(step[i].toggle.dcx)
            board_gpio_toggle(PIN_LCD_DCX);

        // Only allow WRX to change if RDX is high
        if(step[i].toggle.wrx && board_gpio_read(PIN_LCD_RDX)) {
            if(board_gpio_read(PIN_LCD_WRX)) {
                if(!board_gpio_read(PIN_LCD_DCX))
                    load_cmd();
                else
                    load_data();
            }
            board_gpio_toggle(PIN_LCD_WRX);
        }

        // Only allow RDX to change if WRX is high
        if(step[i].toggle.rdx && board_gpio_read(PIN_LCD_WRX)) {
            if(!board_gpio_read(PIN_LCD_RDX)) 
                save_data();
            board_gpio_toggle(PIN_LCD_RDX);
        }

        // Delay
        delay_ms(step[i].msDelay);
    }
}

void lcd_reset(void)
{
    board_gpio_set_low(PIN_LCD_RESET);
    delay_ms(20);
    board_gpio_set_high(PIN_LCD_RESET);
    delay_ms(300);
}

void lcd_write(uint8_t cmd, const uint16_t * data, uint32_t count)
{
    // Set pin mode
    lcd_proto_reset();
    board_set_lcd_pins_output();

    // Configure transfer
    readCnt = 0;
    writeCnt = count;
    command = cmd;
    write = data;
    read = 0;

    // Execute protocol
    PROTO_START();
    while(writeCnt--) {
        PROTO_WRITE();
        write++;
    }
    PROTO_STOP();

    // Set pins back to read mode, to prevent conflict
    board_set_lcd_pins_input();
}

void lcd_read(uint8_t cmd, uint16_t * data, uint32_t count)
{
    // Set pin mode
    lcd_proto_reset();
    board_set_lcd_pins_input();

    // Configure transfer
    readCnt = count;
    writeCnt = 0;
    command = cmd;
    write = 0;
    read = data;

    // Execute protocol
    PROTO_START();
    while(readCnt--) {
        PROTO_READ();
        read++;
    }
    PROTO_STOP();
}

