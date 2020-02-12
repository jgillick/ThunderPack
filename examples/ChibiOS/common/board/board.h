/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32 Nucleo144-F412ZG board.
 */

/*
 * Board identifier.
 */
#define BOARD_THUNDERPACK
#define BOARD_NAME                  "ThunderPack-STM32F412Cx"

/*
 * Board oscillators-related settings.
 * NOTE: LSE not fitted.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0U
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

#define STM32_HSE_BYPASS

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F412Cx

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0U
#define GPIOA_LED1                  0U
#define GPIOA_PIN1                  1U
#define GPIOA_LED2                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_LED3                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_LED4                  4U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_PIN6                  6U
#define GPIOA_SPI1_MISO             6U
#define GPIOA_PIN7                  7U
#define GPIOA_SPI1_MOSI             7U
#define GPIOA_PIN8                  8U
#define GPIOA_USB_SOF               8U
#define GPIOA_PIN9                  9U
#define GPIOA_USB_VBUS              9U
#define GPIOA_PIN10                 10U
#define GPIOA_USB_ID                10U
#define GPIOA_PIN11                 11U
#define GPIOA_USB_DM                11U
#define GPIOA_PIN12                 12U
#define GPIOA_USB_DP                12U
#define GPIOA_PIN13                 13U
#define GPIOA_SWDIO                 13U
#define GPIOA_PIN14                 14U
#define GPIOA_SWCLK                 14U
#define GPIOA_PIN15                 15U
#define GPIOA_I2S3_WS               15U

#define GPIOB_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_PIN2                  2U
#define GPIOB_PIN3                  3U
#define GPIOB_PIN4                  4U
#define GPIOB_BUTTON                4U
#define GPIOB_PIN5                  5U
#define GPIOB_I2S3_SD               5U
#define GPIOB_PIN6                  6U
#define GPIOB_QUADSPI_BK1_NCS       6U
#define GPIOB_PIN7                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_I2C1_SCL              8U
#define GPIOB_PIN9                  9U
#define GPIOB_I2C1_SDA              9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_I2S2_WS               12U
#define GPIOB_PIN13                 13U
#define GPIOB_I2S2_CK               13U
#define GPIOB_PIN14                 14U
#define GPIOB_I2S2_SD               15U

#define LED_PAD0                    0u
#define LED_PAD1                    1u
#define LED_PAD2                    2u
#define LED_PAD3                    3u

/*
 * IO lines assignments.
 */
#define LINE_TIM2_CH1               PAL_LINE(GPIOA, 0U)
#define LINE_TIM2_CH2               PAL_LINE(GPIOA, 1U)
#define LINE_TIM2_CH3               PAL_LINE(GPIOA, 2U)
#define LINE_TIM2_CH4               PAL_LINE(GPIOA, 3U)
#define LINE_SPI3_NSS               PAL_LINE(GPIOA, 4U)
#define LINE_SPI1_SCK               PAL_LINE(GPIOA, 5U)
#define LINE_SPI1_MISO              PAL_LINE(GPIOA, 6U)
#define LINE_SPI1_MOSI              PAL_LINE(GPIOA, 7U)
#define LINE_USB_SOF                PAL_LINE(GPIOA, 8U)
#define LINE_USB_VBUS               PAL_LINE(GPIOA, 9U)
#define LINE_USB_ID                 PAL_LINE(GPIOA, 10U)
#define LINE_USB_DM                 PAL_LINE(GPIOA, 11U)
#define LINE_USB_DP                 PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_I2S3_WS                PAL_LINE(GPIOA, 15U)

#define LINE_BUTTON                 PAL_LINE(GPIOB, 4U)
#define LINE_I2S3_SD                PAL_LINE(GPIOB, 5U)
#define LINE_QUADSPI_BK1_NCS        PAL_LINE(GPIOB, 6U)

#define LINE_LED1                   PAL_LINE(GPIOA, 0U)
#define LINE_LED2                   PAL_LINE(GPIOA, 1U)
#define LINE_LED3                   PAL_LINE(GPIOA, 2U)
#define LINE_LED4                   PAL_LINE(GPIOA, 3U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - TIM2_CH1          (alternate 2).
 * PA1  - TIM2_CH2          (alternate 2).
 * PA2  - TIM2_CH3          (alternate 2).
 * PA3  - TIM2_CH4          (alternate 2).
 * PA4  - SPI3_NSS          (input pullup).
 * PA5  - SPI1_SCK          (input pullup).
 * PA6  - SPI1_MISO         (input pullup).
 * PA7  - SPI1_MOSI         (input pullup).
 * PA8  - USB_SOF           (alternate 10).
 * PA9  - USB_VBUS          (analog).
 * PA10 - USB_ID            (alternate 10).
 * PA11 - USB_DM            (alternate 10).
 * PA12 - USB_DP            (alternate 10).
 * PA13 - SWDIO             (alternate 0).
 * PA14 - SWCLK             (alternate 0).
 * PA15 - I2S3_WS           (not connected).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ALTERNATE(GPIOA_LED1) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_LED2) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_LED3) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_LED4) |   \
                                     PIN_MODE_INPUT(GPIOA_PIN4) |       \
                                     PIN_MODE_INPUT(GPIOA_PIN5) |       \
                                     PIN_MODE_INPUT(GPIOA_PIN6) |       \
                                     PIN_MODE_INPUT(GPIOA_PIN7) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN8) |   \
                                     PIN_MODE_ANALOG(GPIOA_PIN9) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN10) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN11) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN12) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN13) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_PIN14) |  \
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_LED1) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED3) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED4) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN7) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN9) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN10) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN11) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN12) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN13) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN14) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_LED1) |       \
                                     PIN_OSPEED_HIGH(GPIOA_LED2) |    \
                                     PIN_OSPEED_HIGH(GPIOA_LED3) |    \
                                     PIN_OSPEED_HIGH(GPIOA_LED4) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN4) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN5) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN6) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN7) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN8) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN9) |       \
                                     PIN_OSPEED_HIGH(GPIOA_PIN10) |      \
                                     PIN_OSPEED_HIGH(GPIOA_PIN11) |      \
                                     PIN_OSPEED_HIGH(GPIOA_PIN12) |      \
                                     PIN_OSPEED_HIGH(GPIOA_PIN13) |      \
                                     PIN_OSPEED_HIGH(GPIOA_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_LED1) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_LED2) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_LED3) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_LED4) |    \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN4) |      \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN5) |      \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN6) |      \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN7) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN8) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN9) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN10) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN11) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN12) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN13) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN14) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_LED1) |          \
                                     PIN_ODR_HIGH(GPIOA_LED2) |          \
                                     PIN_ODR_HIGH(GPIOA_LED3) |          \
                                     PIN_ODR_HIGH(GPIOA_LED4) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN5) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN6) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN7) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN8) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN9) |          \
                                     PIN_ODR_HIGH(GPIOA_PIN10) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN11) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN12) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN13) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN14) |         \
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_LED1, 2U) |       \
                                     PIN_AFIO_AF(GPIOA_LED2, 2U) |       \
                                     PIN_AFIO_AF(GPIOA_LED3, 2U) |       \
                                     PIN_AFIO_AF(GPIOA_LED4, 2U) |       \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |       \
                                     PIN_AFIO_AF(GPIOA_PIN5, 0U) |       \
                                     PIN_AFIO_AF(GPIOA_PIN6, 0U) |       \
                                     PIN_AFIO_AF(GPIOA_PIN7, 0U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 10U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN9, 0U) |       \
                                     PIN_AFIO_AF(GPIOA_PIN10, 10U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN11, 10U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN12, 10U) |     \
                                     PIN_AFIO_AF(GPIOA_PIN13, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN14, 0U) |      \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - GPIO              (not connected).
 * PB1  - ADC1_IN9          (not connected).
 * PB2  - QUADSPI_CLK       (not connected).
 * PB3  - I2S3_CK           (not connected).
 * PB4  - Button            (input pulldown).
 * PB5  - I2S3_SD           (input pullup).
 * PB6  - QUADSPI_BK1_NCS   (input pullup).
 * PB7  - GPIO              (input floating).
 * PB8  - I2C1_SCL          (not connected).
 * PB9  - I2C1_SDA          (not connected).
 * PB10 - GPIO              (not connected).
 * PB11 - GPIO              (not connected).
 * PB12 - I2S2_WS           (not connected).
 * PB13 - I2S2_CK           (not connected).
 * PB14 - GPIO              (not connected).
 * PB15 - I2S2_SD           (not connected).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0) |       \
                                     PIN_MODE_INPUT(GPIOB_PIN1) |         \
                                     PIN_MODE_INPUT(GPIOB_PIN2) |        \
                                     PIN_MODE_INPUT(GPIOB_PIN3) |        \
                                     PIN_MODE_INPUT(GPIOB_BUTTON) |        \
                                     PIN_MODE_INPUT(GPIOB_I2S3_SD) |        \
                                     PIN_MODE_INPUT(GPIOB_QUADSPI_BK1_NCS) |        \
                                     PIN_MODE_INPUT(GPIOB_PIN7) |          \
                                     PIN_MODE_INPUT(GPIOB_I2C1_SCL) |        \
                                     PIN_MODE_INPUT(GPIOB_I2C1_SDA) |        \
                                     PIN_MODE_INPUT(GPIOB_PIN10) |        \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |        \
                                     PIN_MODE_INPUT(GPIOB_I2S2_WS) |        \
                                     PIN_MODE_INPUT(GPIOB_I2S2_CK) |        \
                                     PIN_MODE_INPUT(GPIOB_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOB_I2S2_SD))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN3) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_BUTTON) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2S3_SD) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_QUADSPI_BK1_NCS) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SCL) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SDA) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2S2_WS) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2S2_CK) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2S2_SD))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_LOW(GPIOB_PIN0) |       \
                                     PIN_OSPEED_LOW(GPIOB_PIN1) |        \
                                     PIN_OSPEED_LOW(GPIOB_PIN2) |       \
                                     PIN_OSPEED_LOW(GPIOB_PIN3) |       \
                                     PIN_OSPEED_HIGH(GPIOB_BUTTON) |       \
                                     PIN_OSPEED_HIGH(GPIOB_I2S3_SD) |       \
                                     PIN_OSPEED_HIGH(GPIOB_QUADSPI_BK1_NCS) |       \
                                     PIN_OSPEED_HIGH(GPIOB_PIN7) |          \
                                     PIN_OSPEED_LOW(GPIOB_I2C1_SCL) |       \
                                     PIN_OSPEED_LOW(GPIOB_I2C1_SDA) |       \
                                     PIN_OSPEED_LOW(GPIOB_PIN10) |       \
                                     PIN_OSPEED_LOW(GPIOB_PIN11) |       \
                                     PIN_OSPEED_LOW(GPIOB_I2S2_WS) |       \
                                     PIN_OSPEED_LOW(GPIOB_I2S2_CK) |       \
                                     PIN_OSPEED_LOW(GPIOB_PIN14) |          \
                                     PIN_OSPEED_LOW(GPIOB_I2S2_SD))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_PIN0) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN3) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_BUTTON) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_I2S3_SD) |      \
                                     PIN_PUPDR_PULLUP(GPIOB_QUADSPI_BK1_NCS) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_I2S2_WS) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_I2S2_CK) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN14) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_I2S2_SD))
#define VAL_GPIOB_ODR               (PIN_ODR_LOW(GPIOB_PIN0) |           \
                                     PIN_ODR_LOW(GPIOB_PIN1) |           \
                                     PIN_ODR_LOW(GPIOB_PIN2) |          \
                                     PIN_ODR_LOW(GPIOB_PIN3) |          \
                                     PIN_ODR_HIGH(GPIOB_BUTTON) |          \
                                     PIN_ODR_HIGH(GPIOB_I2S3_SD) |          \
                                     PIN_ODR_HIGH(GPIOB_QUADSPI_BK1_NCS) |          \
                                     PIN_ODR_LOW(GPIOB_PIN7) |              \
                                     PIN_ODR_LOW(GPIOB_I2C1_SCL) |          \
                                     PIN_ODR_LOW(GPIOB_I2C1_SDA) |          \
                                     PIN_ODR_LOW(GPIOB_PIN10) |          \
                                     PIN_ODR_LOW(GPIOB_PIN11) |          \
                                     PIN_ODR_LOW(GPIOB_I2S2_WS) |          \
                                     PIN_ODR_LOW(GPIOB_I2S2_CK) |          \
                                     PIN_ODR_LOW(GPIOB_PIN14) |              \
                                     PIN_ODR_LOW(GPIOB_I2S2_SD))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |        \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN3, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_BUTTON, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_I2S3_SD, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_QUADSPI_BK1_NCS, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C1_SCL, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_I2S2_WS, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_I2S2_CK, 0U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_I2S2_SD, 0U))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
