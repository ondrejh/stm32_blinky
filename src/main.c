#include <stdio.h>
#include <stdlib.h>

#include <timer.h>

#define LED_PORT (GPIO_TypeDef *)GPIOC_BASE
#define LED_PINMASK (2<<7)

#define LED_ON() GPIO_SetBits(LED_PORT, LED_PINMASK);
#define LED_OFF() GPIO_ResetBits(LED_PORT, LED_PINMASK);

void init_led_output()
{
  // Enable GPIO Peripheral clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  // Configure pin in output push/pull mode
  GPIO_InitStructure.GPIO_Pin = LED_PINMASK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);

  // Start with led turned off
  GPIO_ResetBits(LED_PORT, LED_PINMASK);
}

int main()
{
  timer_init();

  init_led_output();
  
  // Infinite loop
  while (1)
    {
      LED_ON();
      sleep(200);

      LED_OFF();
      sleep(800);
    }
  // Infinite loop, never return.
}
