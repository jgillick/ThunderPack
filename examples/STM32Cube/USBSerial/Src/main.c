/*
  A simple program which demonstrates outputting serial data over USB.
  In this case it simply outputs the state (0/1) of the button, repeatedly,
  to the serial console.
*/

#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#include "main.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"
#include "thunderpack.h"

static void gpio_init(void);

int main(void) {
  HAL_Init();
  thunderpack_clock_init();

  // Setup GPIO & Serial/USB
  gpio_init();
  MX_USB_DEVICE_Init();
  HAL_Delay(1000);

  uint8_t buf[] = "0\r\n";
  uint8_t btnState = 0;
  while (1) {
    // Get button state
    btnState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

    // Convert to character and output to serial console
    buf[0] = btnState + '0';
    CDC_Transmit_FS(buf, 3);

    HAL_Delay(200);
  }
}

/**
  * Setup GPIO pins
  */
static void gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE(); // usb
  __HAL_RCC_GPIOB_CLK_ENABLE(); // button

  // Button - PB4
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * This function is executed in case of errors
  * @retval None
  */
void Error_Handler(void) {
}