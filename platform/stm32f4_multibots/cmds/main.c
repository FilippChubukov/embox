/**
 * @file
 * @brief
 *
 * @date 11.01.2017
 * @author Alex Kalmuk
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <embox/unit.h>

#include "stm32f4_discovery.h"
#include "stepper_motor.h"
#include "ir.h"

extern void nrf24_enable(void);

static void stm32_delay(uint32_t delay) {
	while (--delay > 0)
		;
}

static void init_leds() {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
}

static void main_loop(void) {
	struct stepper_motor motor1, motor2;

	__GPIOA_CLK_ENABLE(); /* for motors */
	__GPIOE_CLK_ENABLE(); /* for motors */

	nrf24_enable();
	motor_init(&motor1, GPIO_PIN_0, GPIO_PIN_2, GPIO_PIN_4, GPIO_PIN_6, GPIOA);
	motor_init(&motor2, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12, GPIO_PIN_14, GPIOE);
	motor_set_speed(&motor1, MOTOR_MAX_SPEED);
	motor_set_speed(&motor2, MOTOR_MAX_SPEED);
	ir_init();
	ir_enable();

	stm32_delay(1000000);
	while (1) {
		motor_do_steps(&motor1, 3 * MOTOR_STEPS_PER_REVOLUTION, MOTOR_RUN_FORWARD);
		motor_do_steps(&motor2, 3 * MOTOR_STEPS_PER_REVOLUTION, MOTOR_RUN_FORWARD);
		stm32_delay(5000000);
	}
}

int main(int argc, char *argv[]) {
	printf("Robot test start!\n");

	//HAL_Init();

	init_leds();
	BSP_LED_Toggle(LED6);
	HAL_GPIO_WritePin(IR_GPIO, IR_LED_PIN, GPIO_PIN_SET);

	main_loop();

	return 0;
}
