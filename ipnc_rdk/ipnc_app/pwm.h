#ifndef _PWM_H
#define _PWM_H


#define PWM_DEV_PATH "/sys/class/pwm/pwmchip0/"

enum {
	MTK7XXX_PWM_0 = 0,
	MTK7XXX_PWM_1 = 1,
	MTK7XXX_PWM_MAX,
};

#define PWM_enable(id) 	ipnc_pwm_write(id,"enable","1", 1)
#define PWM_disable(id) ipnc_pwm_write(id,"enable","0", 1)

#define PWM_SetDuty(id, value, valueLen) 	ipnc_pwm_write(id,"duty_cycle",value, valueLen)
#define PWM_SetPeriod(id, value, valueLen) ipnc_pwm_write(id,"period",value, valueLen)

#endif 
