/*
 * lcd_buffer.h
 *
 *  Created on: 17/feb/2016
 *      Author: Enrico Gambini
 */

#ifndef SRC_DUCTED_DRIVERS_LCD_BUFFER_H_
#define SRC_DUCTED_DRIVERS_LCD_BUFFER_H_

#include <machine.h>
#include <platform.h>
#include <stdio.h>

static uint8_t lcd_buffer[8][12];

void lcd_buffer_print(uint8_t line, const uint8_t* format, ...) {
	_Va_list args;
	va_start(args, format);

	if (line % 8 == 0 && line < 56) {
		for (int i = 0; i < 12; i++)
			lcd_buffer[line / 8][i] = ' ';
		vsprintf(lcd_buffer[line / 8], format, args);
	}

	va_end(args);
}

void lcd_buffer_clear() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 12; j++)
			lcd_buffer[i][j] = (uint8_t) (' ');
}

void lcd_buffer_flush() {
	for (int i = 0; i < 8; i++)
		lcd_display(i * 8, lcd_buffer[i]);
}

#endif /* SRC_DUCTED_DRIVERS_LCD_BUFFER_H_ */
