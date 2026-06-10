//*******************************************************
// Copyright (c) MLRS project
// GPL3
// https://www.gnu.org/licenses/gpl-3.0.de.html
// OlliW @ www.olliw.eu
//*******************************************************
// hal
//********************************************************

//-------------------------------------------------------
// BlueLRS PSA RX Module STM32L432KB (32-pin UFQFPN32)
// SX1280 2.4 GHz transceiver, no external PA
//-------------------------------------------------------
// Schematic: BlueLRS_PSA_module (sch_lrs.pdf)
// Reference: https://github.com/ExpressLRS/ExpressLRS-Hardware/tree/master/PCB/2400MHz/RX_PP
//
// Pin assignments:
//   PA4        SPI1 CS (SX1280 NSS)
//   PA5        SPI1 SCK
//   PA6        SPI1 MISO
//   PA7        SPI1 MOSI
//   PA10       SX DIO1 (EXTI10)
//   PA11       SX BUSY
//   PB4        SX RESET
//   PB5        LED (single)
//   PB6        Serial TX (USART1) — J1 connector, to FC
//   PB7        Serial RX (USART1) — J1 connector, from FC

#define DEVICE_HAS_SINGLE_LED
#define DEVICE_HAS_NO_DEBUG


//-- Timers, Timing, EEPROM, and such stuff

#define DELAY_USE_DWT

#define EE_START_PAGE             60 // 128 kB flash, 2 kB page

#define MICROS_TIMx               TIM15

#define CLOCK_TIMx                TIM2
#define CLOCK_IRQn                TIM2_IRQn
#define CLOCK_IRQHandler          TIM2_IRQHandler
//#define CLOCK_IRQ_PRIORITY        10


//-- UARTS
// UARTB = serial port

#define UARTB_USE_UART1_PB6PB7 // serial
#define UARTB_BAUD                RX_SERIAL_BAUDRATE
#define UARTB_USE_TX
#define UARTB_TXBUFSIZE           RX_SERIAL_TXBUFSIZE
#define UARTB_USE_TX_ISR
#define UARTB_USE_RX
#define UARTB_RXBUFSIZE           RX_SERIAL_RXBUFSIZE


//-- SX1: SX1280 & SPI

#define SPI_USE_SPI1              // PA5 SCK, PA6 MISO, PA7 MOSI
#define SPI_CS_IO                 IO_PA4
#define SPI_USE_CLK_LOW_1EDGE     // datasheet says CPHA = 0  CPOL = 0
#define SPI_USE_CLOCKSPEED_9MHZ

#define SX_RESET                  IO_PB4
#define SX_DIO1                   IO_PA10
#define SX_BUSY                   IO_PA11
#define SX_RX_EN                  //
#define SX_TX_EN                  //

#define SX_DIO1_SYSCFG_EXTI_PORTx     LL_SYSCFG_EXTI_PORTA
#define SX_DIO1_SYSCFG_EXTI_LINEx     LL_SYSCFG_EXTI_LINE10
#define SX_DIO_EXTI_LINE_x            LL_EXTI_LINE_10
#define SX_DIO_EXTI_IRQn              EXTI15_10_IRQn
#define SX_DIO_EXTI_IRQHandler        EXTI15_10_IRQHandler
//#define SX_DIO_EXTI_IRQ_PRIORITY    11

void sx_init_gpio(void)
{
    gpio_init(SX_RESET, IO_MODE_OUTPUT_PP_HIGH, IO_SPEED_VERYFAST);
    gpio_init(SX_DIO1, IO_MODE_INPUT_PD, IO_SPEED_VERYFAST);
    gpio_init(SX_BUSY, IO_MODE_INPUT_PU, IO_SPEED_VERYFAST);
}

bool sx_busy_read(void)
{
    return (gpio_read_activehigh(SX_BUSY)) ? true : false;
}

void sx_amp_transmit(void)
{
}

void sx_amp_receive(void)
{
}

void sx_dio_init_exti_isroff(void)
{
    LL_SYSCFG_SetEXTISource(SX_DIO1_SYSCFG_EXTI_PORTx, SX_DIO1_SYSCFG_EXTI_LINEx);

    LL_EXTI_DisableEvent_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_DisableIT_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_DisableFallingTrig_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_EnableRisingTrig_0_31(SX_DIO_EXTI_LINE_x);

    NVIC_SetPriority(SX_DIO_EXTI_IRQn, SX_DIO_EXTI_IRQ_PRIORITY);
    NVIC_EnableIRQ(SX_DIO_EXTI_IRQn);
}

void sx_dio_enable_exti_isr(void)
{
    LL_EXTI_ClearFlag_0_31(SX_DIO_EXTI_LINE_x);
    LL_EXTI_EnableIT_0_31(SX_DIO_EXTI_LINE_x);
}

void sx_dio_exti_isr_clearflag(void)
{
    LL_EXTI_ClearFlag_0_31(SX_DIO_EXTI_LINE_x);
}


//-- Out port
// No output port on this board.

void out_init_gpio(void)
{
}


//-- Button
// No bind button on this board; stubs return not-pressed.

void button_init(void)
{
}

bool button_pressed(void)
{
    return false;
}


//-- LEDs
// Single LED on PB5, active high.

#define LED_RED                   IO_PB5

void leds_init(void)
{
    gpio_init(LED_RED, IO_MODE_OUTPUT_PP_LOW, IO_SPEED_DEFAULT);
    gpio_low(LED_RED);
}

void led_red_off(void) { gpio_low(LED_RED); }
void led_red_on(void) { gpio_high(LED_RED); }
void led_red_toggle(void) { gpio_toggle(LED_RED); }


//-- POWER

#define POWER_PA_NONE_SX128X
#include "../hal-power-pa.h"


//-- TEST
// Only confirmed-connected pins included.

uint32_t porta[] = {
    LL_GPIO_PIN_4, LL_GPIO_PIN_5, LL_GPIO_PIN_6, LL_GPIO_PIN_7, // SPI
    LL_GPIO_PIN_10, LL_GPIO_PIN_11,                               // DIO1, BUSY
};

uint32_t portb[] = {
    LL_GPIO_PIN_4, LL_GPIO_PIN_5, LL_GPIO_PIN_6, LL_GPIO_PIN_7,  // RESET, LED, UART
};

uint32_t portc[] = {
};
