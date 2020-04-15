/*
  Demonstrates writing to and reading from the USB serial interface (USB CDC).
  Connect Thunderpack to your computer and then connect a serial console to it.
  Then:
    + Press the thunderpack user button and you should see the console output the button value.
    + Type a number 1 - 4 into the console and you should see that LED toggle either on or off.
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
  uint8_t lastBtnState = 0;
  while (1) {
    // Print button state
    btnState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
    if (btnState != lastBtnState) {
      // Convert to character and output to serial console
      buf[0] = btnState + '0';
      CDC_Transmit_FS(buf, 3);
    }
    lastBtnState = btnState;

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

  // LED 1: PA0
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * This function is executed in case of errors
  * @retval None
  */
void Error_Handler(void) {
}