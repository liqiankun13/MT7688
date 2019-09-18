#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pwm.h>


int ipnc_pwm_write( int pwm_id , const char* node, const char* valueStr, int valueStrLen)
{

	int 	fd_pwm = 0;
	int		result = 0;
	char 	dev_name[48];

	/* Don't do anything on EVM */
	if( pwm_id < 0 || pwm_id > MTK7XXX_PWM_MAX )
	{
		printf("gio id = %d over range!!\n", pwm_id);
		return -1;
	}

	snprintf( dev_name, sizeof(dev_name),PWM_DEV_PATH"pwm%d/%s", pwm_id, node);

	fd_pwm = open(dev_name, O_RDWR);

	if( fd_pwm < 0)
	{
		printf("open device %s\r\n", dev_name);
		goto PWM_QUIT;
	}



	result = write(fd_pwm,  valueStr, valueStrLen);
	if( result <= 0 )
	{
		printf("write device error !! gio = %d\n", pwm_id);
		close(fd_pwm);
		goto PWM_QUIT;
	}
	

PWM_QUIT:
	if( fd_pwm )
		close(fd_pwm);

	return 0;
}



# if 0
int main()
{
	PWM_enable(MTK7XXX_PWM_0); //ÊµÀý 20 KHZ %50 
	PWM_SetDuty(MTK7XXX_PWM_0,"25000",5);
	PWM_SetPeriod(MTK7XXX_PWM_0,"50000",  5);
	
	
	return 0;
}
#endif