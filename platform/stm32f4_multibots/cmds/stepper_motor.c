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

#include "stm32f4_discovery.h"
#include "stepper_motor.h"

static void stm32_delay(uint32_t delay) {
	while (--delay > 0)
		;
}

void motor_init(struct stepper_motor *m) {
	GPIO_InitTypeDef  GPIO_InitStruct;

	memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));

	MOTOR_CLK_ENABLE();

	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	
	GPIO_InitStruct.Pin = MOTOR_IN1 | MOTOR_IN2 | MOTOR_IN3 | MOTOR_IN4;
	HAL_GPIO_Init(MOTOR_GPIO, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN4, GPIO_PIN_RESET);

	m->speed = MOTOR_DEFAULT_SPEED;
	m->step_size = MOTOR_STEP_SIZE;
	m->steps_cnt = 0;

	printf("Motor inited\n");
}

static void motor_do_one_step(struct stepper_motor *m, int direction) {
	size_t delay = m->speed * MOTOR_MIN_DELAY;

	assert(direction == MOTOR_RUN_FORWARD || direction == MOTOR_RUN_BACKWARD);

	if (direction == MOTOR_RUN_FORWARD) {
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN2, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN3, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN2, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN4, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN3, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN4, GPIO_PIN_RESET);
	} else {
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN4, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN3, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN4, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN2, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN3, GPIO_PIN_RESET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN1, GPIO_PIN_SET);
		stm32_delay(delay);
		HAL_GPIO_WritePin(MOTOR_GPIO, MOTOR_IN2, GPIO_PIN_RESET);
	}
}

void motor_do_steps(struct stepper_motor *m, size_t nsteps, int direction) {
	int i = nsteps;

	while (i-- > 0) {
		motor_do_one_step(m, direction);
	}
}
