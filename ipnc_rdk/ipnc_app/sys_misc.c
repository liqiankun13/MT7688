#include <stdio.h>
#include <sys_misc.h>
#include <sys_ctrl.h>
#include <ipnc_gio_util.h>


static pthread_t gLedMngThr,gBeepMngThr;

static  int frq = 5;
static int times = 0;

void start_led(int frq1, int times1)
{		
	if(frq1 > 0 && frq1 < 1000)
	{
		frq = frq1;
		times = times1;
	}
}
void LedMngThr(void *args)
{
	
	while(1)
	{
		if(times != 0)
		{
			//ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_LOW);
			usleep(1000000/frq);
			//ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_HIGH);
			usleep(1000000/frq);
			//ipnc_gio_write(GPIO_BREATH_LIGTHT_EN, GPIO_LOW);
			if(times > 0)
				times --;
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






