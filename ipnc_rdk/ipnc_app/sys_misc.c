#include <stdio.h>
#include <sys_misc.h>
#include <sys_ctrl.h>
#include <ipnc_gio_util.h>
#include <pwm.h>

static pthread_t gLedMngThr,gBeepMngThr;

static  int frq = 5;
static int times = 0;
static int stopSta = GPIO_HIGH;

void start_led(int frq1, int times1, int isHighstop)
{		
	if(frq1 > 0 && frq1 < 1000)
	{
		frq = frq1;
		times = times1;
	}
	stopSta = isHighstop;
}
void LedMngThr(void *args)
{
	
	while(1)
	{
		if(times != 0)
		{
			ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_LOW);
			usleep(1000000/frq);
			ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_HIGH);
			usleep(1000000/frq);
			ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_LOW);
			if(times > 0)
				times --;
			if(times == 0)
			{
				ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, stopSta);
			}
		}
		
		usleep(1000000/frq);

	}

}



void BeepMngThr(void *args)
{
	while(1)
	{
		usleep(20000);
	}
}



int BeepMngInit()
{
	if(pthread_create(&gBeepMngThr,NULL,BeepMngThr,NULL)){
		printf("create thread fail \n");
		return -1;
	}
	return 0;
}

int BeepMngExit()
{
	int ret = 0;

	pthread_join(gBeepMngThr,(void **)&ret);
	pthread_detach(gBeepMngThr);
	return ret;
}

int LedMngInit()
{
	if(pthread_create(&gLedMngThr,NULL,LedMngThr,NULL)){
		printf("create thread fail \n");
		return -1;
	}
	return 0;
}

int LedMngExit()
{
	int ret = 0;
	
	pthread_join(gLedMngThr,(void **)&ret);
	pthread_detach(gLedMngThr);
	
	return ret;
}



int MiscMngInit()
{
	int ret = 0;

	ret |= LedMngInit();

	return ret;
}

int MiscMngExit()
{
	int ret = 0;
	
	ret |= LedMngExit();
	
	return ret;
}


void start_bp(int ms, int times)
{
	PWM_enable(MTK7XXX_PWM_0); //ÊµÀý 20 KHZ %50 
	PWM_SetPeriod(MTK7XXX_PWM_0,"500000",  6);//1000 000 000/x Hz 
	PWM_SetDuty(MTK7XXX_PWM_0,"250000",6);
	usleep(1000*ms);
	PWM_disable(MTK7XXX_PWM_0);

}



