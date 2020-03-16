/*
  A simple program which demonstrates outputting serial data over USB.
  In this case it simply outputs the state (0/1) of the button, repeatedly,
  to the serial console.
*/

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "thunderpack.h"

void SystemClock_Config(void);
void init_usb_clock(void);
static void MX_GPIO_Init(void);

int main(void) {
  // Init the system
  HAL_Init();
  SystemClock_Config();

  // Setup GPIO & Serial/USB
  MX_GPIO_Init();
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

void SystemClock_Config(void) {
  init_system_clock();
  init_usb_clock();
}

/**
  * System Clock Configuration
  */
void init_usb_clock(void) {
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 72;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 12;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 3;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLI2SQ;
  PeriphClkInitStruct.PLLI2SSelection = RCC_PLLI2SCLKSOURCE_PLLSRC;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * GPIO Initialization Function
  */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Button - PB4
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * This function is executed in case of error occurrence.
  */
void Error_Handler(void) {
}
