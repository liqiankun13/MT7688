#ifndef _SYS_CTRL_H
#define _SYS_CTRL_H

/* 门打开 */
#define GPIO_GATE_DETECT    24
/* 光敏 */
#define GPIO_LIGHT_DETECT 39
/* 陀机使能 */

/* 加热 */
#define GPIO_HEAT_EN    19
/* 补光灯  */
#define GPIO_FLASH_LIGTHT_EN    11
/* 呼吸灯 */
#define GPIO_BREATH_LIGTHT_EN 25
/* 按键 */
#define GPIO_SYSTEM_KEY 0
/* 电源控制 */
#define GPIO_SYS_KEY_POWEROFF 1

#define GPIO_SYS_KEY_WPS    38





#define start_heat() ipnc_gio_write(GPIO_HEAT_EN, 1)
#define stop_heat()  ipnc_gio_write(GPIO_HEAT_EN, 0)



//#define start_MOTOR() ipnc_gio_write(GPIO_MOTOR_EN, 1)
//#define stop_MOTOR()  ipnc_gio_write(GPIO_MOTOR_EN, 0)

#define getLightState() ipnc_gio_read(GPIO_LIGHT_DETECT)
#define getGateState() ipnc_gio_read(GPIO_GATE_DETECT)

void startFlashTimerEvent(int timeVal);

void enterAirkissConfigNet();
void exitAirkissConfigNet();







void saveJpeg();
void showCamera();
void showWarning(int select);
void showSysTime();





#endif 

