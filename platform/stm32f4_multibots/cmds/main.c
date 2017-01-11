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

static void init_leds() {
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
}

int main(int argc, char *argv[]) {
	struct stepper_motor motor;

	printf("Robot test start!\n");

	//HAL_Init();

	init_leds();
	BSP_LED_Toggle(LED6);

	motor_init(&motor);

	motor_set_speed(&motor, MOTOR_MAX_SPEED);
	motor_do_steps(&motor, 3 * MOTOR_STEPS_PER_REVOLUTION, MOTOR_RUN_FORWARD);

	return 0;
}
