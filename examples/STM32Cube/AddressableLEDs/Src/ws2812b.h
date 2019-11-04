#include "ws2812b_conf.h"

#ifndef WS2812B_H
#define WS2812B_H

void ws2812b(void);
void ws2812b_init(void);
void ws2812b_interrupt_handler(void);
void led_set_color_rgb(size_t index, uint32_t rgb);
void led_set_color_all_rgb(uint32_t rgbw);

#if USE_RGBW
void led_set_color(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_set_color_all(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
#else /* USE_RGBW */
void led_set_color(size_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_color_all(uint8_t r, uint8_t g, uint8_t b);
#endif /* !USE_RGBW */

#endif