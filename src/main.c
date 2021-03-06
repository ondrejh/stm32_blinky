#include <stdio.h>
#include <stdlib.h>

#include <timer.h>

#include "stm32f0xx_conf.h"

#define LED_PORT (GPIO_TypeDef *)GPIOC_BASE
#define LED_RED_PIN (GPIO_Pin_6)
#define LED_BLUE_PIN (GPIO_Pin_7)
#define LED_ORANGE_PIN (GPIO_Pin_8)
#define LED_GREEN_PIN (GPIO_Pin_9)

#define BUTTON_PORT (GPIO_TypeDef *)GPIOA_BASE
#define BUTTON_PIN (GPIO_Pin_0)

#define LED_RED_ON() GPIO_SetBits(LED_PORT, LED_RED_PIN);
#define LED_RED_OFF() GPIO_ResetBits(LED_PORT, LED_RED_PIN);
#define LED_ORANGE_ON() GPIO_SetBits(LED_PORT, LED_ORANGE_PIN);
#define LED_ORANGE_OFF() GPIO_ResetBits(LED_PORT, LED_ORANGE_PIN);
#define LED_BLUE_ON() GPIO_SetBits(LED_PORT, LED_BLUE_PIN);
#define LED_BLUE_OFF() GPIO_ResetBits(LED_PORT, LED_BLUE_PIN);
#define LED_GREEN_ON() GPIO_SetBits(LED_PORT, LED_GREEN_PIN);
#define LED_GREEN_OFF() GPIO_ResetBits(LED_PORT, LED_GREEN_PIN);

#define BUTTON GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN)


void init_led_output() {
  // Enable GPIO Peripheral clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin = LED_RED_PIN | LED_ORANGE_PIN | LED_BLUE_PIN | LED_GREEN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);

  // Start with led turned off
  GPIO_ResetBits(LED_PORT, LED_RED_PIN | LED_ORANGE_PIN | LED_BLUE_PIN | LED_GREEN_PIN);
}

void init_button_input() {
	// Enable GPIO Peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin input pull-down mode
	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(BUTTON_PORT, &GPIO_InitStructure);
}

void init_uart() {
	// serial comms
	// use PA2 (TX) and PA3 (RX)
	// USART2
	// These are connected to the ST-Link circuit on the Nucleo board
	// so the serial output will be via USB, on your computer

	// enable clock for USART2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Enable GPIO Peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// setup Tx/RX pins:
	GPIO_InitTypeDef GPIO_InitStructure;

	// init TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// init RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// setup USART2:

	// initialize struct
	USART_InitTypeDef usartInitStruct;
	USART_StructInit(&usartInitStruct);
	// set parameters
	usartInitStruct.USART_BaudRate = 115200;
	usartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &usartInitStruct);
	USART_Cmd(USART2, ENABLE);
}

#define uart_putc(x) USART_SendData(USART2, x)

int main()
{
  timer_init();

  init_led_output();
  init_button_input();
  init_uart();
  
  uart_putc('A');
  uart_putc('h');
  uart_putc('o');
  uart_putc('j');
  uart_putc('!');
  uart_putc('\n');

  // Infinite loop
  while (1)
    {
      LED_RED_ON();
      sleep(200);

      LED_RED_OFF();
      sleep(800);

      if (BUTTON) LED_BLUE_ON() else LED_BLUE_OFF();
    }
  // Infinite loop, never return.
}
