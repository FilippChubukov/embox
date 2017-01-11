
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <embox/unit.h>

#include "stm32f4_discovery.h"

#define MOTOR_GPIO               GPIOA
#define MOTOR_CLK_ENABLE()       __GPIOA_CLK_ENABLE()

#define MOTOR_IN1                GPIO_PIN_0
#define MOTOR_IN2                GPIO_PIN_2
#define MOTOR_IN3                GPIO_PIN_4
#define MOTOR_IN4                GPIO_PIN_6

#define MOTOR_MIN_DELAY            1000
#define MOTOR_MAX_SPEED            1 // Corresponds to MOTOR_MIN_DELAY
#define MOTOR_DEFAULT_SPEED        10 // Corresponds to MOTOR_MIN_DELAY * 10
#define MOTOR_STEP_SIZE            64
#define MOTOR_STEPS_PER_REVOLUTION 512

#define MOTOR_RUN_FORWARD          0
#define MOTOR_RUN_BACKWARD         1

struct stepper_motor {
	uint8_t speed;
	uint8_t step_size;
	uint8_t steps_cnt;
};

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

static void motor_init(struct stepper_motor *m) {
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

static void motor_set_speed(struct stepper_motor *m, size_t speed) {
	m->speed = speed;
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

static void motor_do_steps(struct stepper_motor *m, size_t nsteps) {
	int i = nsteps;

	while (i-- > 0) {
		motor_do_one_step(m, MOTOR_RUN_BACKWARD);
	}
}

int main(int argc, char *argv[]) {
	struct stepper_motor motor;

	printf("Stepper motor test start!\n");

	//HAL_Init();

	init_leds();
	BSP_LED_Toggle(LED6);

	motor_init(&motor);
	printf("Starting motor loop...\n");

	motor_set_speed(&motor, MOTOR_MAX_SPEED);
	motor_do_steps(&motor, 3 * MOTOR_STEPS_PER_REVOLUTION);

	return 0;
}
