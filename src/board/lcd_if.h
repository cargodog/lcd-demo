#ifndef _LCD_IF_H_
#define _LCD_IF_H_

#include <stdint.h>


void lcd_reset(void);
void lcd_write(uint8_t cmd, const uint16_t * data, uint32_t count);
void lcd_read(uint8_t cmd, uint16_t * data, uint32_t count);

#endif /* _LCD_IF_H_ */
