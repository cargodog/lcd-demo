#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_


#define BOARD_XTAL0_CLK_HZ              12000000U   // xtal0 frequency in Hz
#define CLKCFG_CORE_CLOCK_HIGH_SPEED    150000000U
#define CLKCFG_CORE_CLOCK_LOW_POWER     4000000U
#define CLKCFG_CORE_CLOCK_DEFAULT       120000000U


void clkcfg_run_high_speed(void);
void clkcfg_run_low_power(void);
void clkcfg_run_default(void);


#endif /* _CLOCK_CONFIG_H_ */
