#include "bsp_led.h"
void LED_Init(void)
{
    rcu_periph_clock_enable(LED1_RCU_CLK);
    rcu_periph_clock_enable(LED2_RCU_CLK);
    rcu_periph_clock_enable(LED3_RCU_CLK);
    rcu_periph_clock_enable(LED4_RCU_CLK);

    gpio_mode_set(LED1_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED1_PIN);
    gpio_mode_set(LED2_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED2_PIN);
    gpio_mode_set(LED3_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED3_PIN);
    gpio_mode_set(LED4_GPIO,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED4_PIN);

    gpio_output_options_set(LED1_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED1_PIN);
    gpio_output_options_set(LED2_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED2_PIN);
    gpio_output_options_set(LED3_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED3_PIN);
    gpio_output_options_set(LED4_GPIO,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,LED4_PIN);

}


