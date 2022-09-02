#include <wiringPi.h>
#include <softPwm.h>
#include "gpio.h"

void initialize_gpio(){
  wiringPiSetup();
  softPwmCreate(FAN_GPIO_PIN, PWM_MIN, PWM_MAX);
  softPwmCreate(RESISTOR_GPIO_PIN, PWM_MIN, PWM_MAX);
}


void update_fan_power(float pwm){
    if(pwm < FAN_MIN){
        pwm = FAN_MIN;
    }
    softPwmWrite(FAN_GPIO_PIN, (int) pwm);
}

void update_resistor_power(float pwm){
    softPwmWrite(RESISTOR_GPIO_PIN, (int) pwm);
}

void turn_off_fan(){
    softPwmWrite(FAN_GPIO_PIN, PWM_MIN);
}
void turn_off_resistor(){
    softPwmWrite(RESISTOR_GPIO_PIN, PWM_MIN);
}


void adjust_tempeture(float pwm){
    if(pwm == 0){
        turn_off_fan();
        turn_off_resistor(); 
        return;
    }

    if(pwm > 0){
        update_resistor_power(pwm);
        turn_off_fan();
        return;
    }

    update_fan_power((-1)*pwm);
    turn_off_resistor(); 
}