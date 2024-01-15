/*
 * interrupt_manager.c
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#include "interrupt_manager.h"
#include "misc.h"

void interrupt_manager_init_mcu_external_interrupts(void)
{
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = INTERRUPT_MANAGER_ACTIVITY_PIN|INTERRUPT_MANAGER_FIFO_WATERMARK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = ENABLE;
	GPIO_Init(&GPIO_InitStructure);
	NVIC_InitType NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GPIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LOW_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	GPIO_EXTIConfigType GPIO_EXTIStructure;
	GPIO_EXTIStructure.GPIO_Pin =INTERRUPT_MANAGER_ACTIVITY_PIN|INTERRUPT_MANAGER_FIFO_WATERMARK_PIN;
	GPIO_EXTIStructure.GPIO_IrqSense = GPIO_IrqSense_Level;
	GPIO_EXTIStructure.GPIO_Event = GPIO_Event_High;
	GPIO_EXTIConfig(&GPIO_EXTIStructure);
}

void interrupt_manager_set_mcu_interrupt_pin_state(uint32_t gpio_pin,FunctionalState state)
{
	GPIO_ClearITPendingBit(gpio_pin);
	GPIO_EXTICmd(gpio_pin, state);
}

ITStatus interrupt_manager_get_interrupt_pin_value(uint32_t gpio_pin)
{
	return GPIO_GetITStatusBit(gpio_pin);
}
