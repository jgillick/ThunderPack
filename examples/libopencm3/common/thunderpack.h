/**
 * Setup functions for the thunderpack board.
 */

#ifndef __THUNDERPACK_H
#define __THUNDERPACK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Setup the thunderpack clock
void thunderpack_clock_init(void);

// Delay execution by a number of milliseconds.
void delay(uint64_t duration);

// Get the number of milliseconds which have elapsed since the microcontroller
// was turned on.
uint64_t millis(void);

#ifdef __cplusplus
} /* extern "C" { */
#endif 

#endif