/*
 * interrupt_manager.h
 *
 *  Created on: Jan 8, 2024
 *      Author: mkadayifci
 */

#ifndef INTERRUPT_MANAGER_H_
#define INTERRUPT_MANAGER_H_

#include "BlueNRG1_gpio.h"

#define INTERRUPT_MANAGER_ACTIVITY_PIN			GPIO_Pin_12
#define INTERRUPT_MANAGER_FIFO_WATERMARK_PIN	GPIO_Pin_5

void interrupt_manager_init_mcu_external_interrupts(void);
void interrupt_manager_set_mcu_interrupt_pin_state(uint32_t gpio_pin,FunctionalState state);
ITStatus interrupt_manager_get_interrupt_pin_value(uint32_t gpio_pin);

#endif /* INTERRUPT_MANAGER_H_ */



