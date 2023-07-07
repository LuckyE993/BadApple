#include "bsp_dma.h"

void DMA_Config(void)
{
    rcu_periph_clock_enable(RCU_DMA1);

    dma_deinit(DMA1,DMA_CH2);

    dma_single_data_parameter_struct dma_init_structure;

    dma_init_structure.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_structure.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_structure.memory0_addr = (uint32_t)usart0_receive_buffer;
    dma_init_structure.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_structure.number = USART_RECEIVE_LENGTH;
    dma_init_structure.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_structure.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_structure.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_structure.priority = DMA_PRIORITY_ULTRA_HIGH;

    dma_single_data_mode_init(DMA1,DMA_CH2,&dma_init_structure);

    dma_channel_subperipheral_select(DMA1,DMA_CH2,DMA_SUBPERI4);

    dma_channel_enable(DMA1,DMA_CH2);

    
    dma_interrupt_enable(DMA1,DMA_CH2,DMA_CHXCTL_FTFIE);
    nvic_irq_enable(DMA1_Channel2_IRQn,2,1);
    
    usart_dma_receive_config(USART0,USART_DENR_ENABLE);
    
}
