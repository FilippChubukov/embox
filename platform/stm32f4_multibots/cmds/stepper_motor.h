/**
 * @file
 * @brief
 *
 * @date 11.01.2017
 * @author Alex Kalmuk
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_

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

extern void motor_init(struct stepper_motor *m);
extern void motor_do_steps(struct stepper_motor *m, size_t nsteps, int direction);

static inline void motor_set_speed(struct stepper_motor *m, size_t speed) {
	m->speed = speed;
}

#endif /* STEPPER_MOTOR_H_ */
