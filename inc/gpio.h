#ifndef GPIO
#define GPIO


#define PWM_MAX 100
#define PWM_MIN 0

#define FAN_MIN 40


#define RESISTOR_GPIO_PIN 4
#define FAN_GPIO_PIN 5

void initialize_gpio();
void adjust_tempeture(float);

#endif