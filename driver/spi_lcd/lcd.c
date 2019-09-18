#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

#include <linux/gpio.h>
#include <linux/delay.h>

#include <linux/fb.h>

#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/wait.h>

//#include <asm/mach/map.h>
#define USE_HORIZONTAL
#define __DEBUG__ 1

#ifdef __DEBUG__
#define DEBUG(format,...) \
        printk("DEBUG::"format,  ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif


#define LCD_X_SIZE         128
#define LCD_Y_SIZE         64

static int tft_lcdfb_setcolreg(unsigned int regno, unsigned int red,
			     unsigned int green, unsigned int blue,
			     unsigned int transp, struct fb_info *info);

struct tft_lcd{
    struct gpio_desc *reset_gpio;   
	struct gpio_desc *rs_gpio;

};

struct tft_lcd_fb{
	struct spi_device *spi; //记录fb_info对象对应的spi设备对象
	struct task_struct *thread; //记录线程对象的地址，此线程专用于把显存数据发送到屏的驱动ic
};
static struct fb_ops fops = {
	.owner		= THIS_MODULE,
	.fb_setcolreg	= tft_lcdfb_setcolreg,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
};



struct regdata_t{
        u8 cmd;
        int delay_ms;
}regdatas[] = {	
	{0xe2, 0},
	{0x2c, 0},
	{0x2e, 0},
	{0x2f, 0},
	{0x23, 0},
	{0x81, 0},
	{0xa2, 0},
	{0xc8, 0},
	{0xa0, 0},
	{0x40, 0},
	{0xaf, 0}
}; 

static inline void LCD_WriteCommand(struct spi_device *spi, u8 cmd)
{
	struct tft_lcd *pdata = spi_get_drvdata(spi);
	u8 command = cmd;
	gpio_set_value(pdata->rs_gpio, 0); //高电平
	spi_write(spi, &command, 1);   
	gpio_set_value(pdata->rs_gpio, 1); //高电平
}
static inline void LCD_WriteData(struct spi_device *spi, u8 data)
{
	struct tft_lcd *pdata = spi_get_drvdata(spi);
	u8 tmp_data = data;
	spi_write(spi, &tmp_data, 1);   
}


#if 0
static void Lcd_SetRegion(struct spi_device *spi, u8 xStar, u8 yStar,u8 xEnd,u8 yEnd)
{
#ifdef USE_HORIZONTAL//如果定义了横屏 
		LCD_WriteReg(spi,0x38,xEnd);
		LCD_WriteReg(spi,0x39,xStar);
		LCD_WriteReg(spi,0x36,yEnd);
		LCD_WriteReg(spi,0x37,yStar);
		LCD_WriteReg(spi,0x21,xStar);
		LCD_WriteReg(spi,0x20,yStar);
#else//竖屏   
		LCD_WriteReg(spi,0x36,xEnd);
		LCD_WriteReg(spi,0x37,xStar);
		LCD_WriteReg(spi,0x38,yEnd);
		LCD_WriteReg(spi,0x39,yStar);
		LCD_WriteReg(spi,0x20,xStar);
		LCD_WriteReg(spi,0x21,yStar);
#endif
		Lcd_WriteIndex(spi,0x22);	

}
#endif 
static int lcd_dt_parse(struct spi_device *spi, struct tft_lcd *lcd_data)
{
	int err;
	struct device_node *np = spi->dev.of_node;
	of_property_read_u32(np, "reset-gpio",
			     (u32 *)&lcd_data->reset_gpio);
	of_property_read_u32(np, "rs-gpio",
			     (u32 *)&lcd_data->rs_gpio);
	err = gpio_request(lcd_data->reset_gpio, "reset-gpio");
	if (unlikely(err)) {
		pr_err(" gpio %ld request failed ", lcd_data->reset_gpio);
		goto err0;
	}
	/* Configure reset_gpio GPIO as output=0 */
	err = gpio_direction_output(lcd_data->reset_gpio, 1);
	if (unlikely(err)) {
		pr_err(" unable to configure gpio %ld", lcd_data->reset_gpio);
		goto err0;
	}		
	err = gpio_request(lcd_data->rs_gpio, "rs-gpio");
	if (unlikely(err)) {
		pr_err(" gpio %ld request failed ", lcd_data->rs_gpio);
		goto err0;
	}
	/* Configure rs_gpio GPIO as output=0 */
	err = gpio_direction_output(lcd_data->rs_gpio, 1);
	if (unlikely(err)) {
		pr_err(" unable to configure gpio %ld", lcd_data->rs_gpio);
		goto err1;
	}	
	
	DEBUG("[GPIO] reset = %d  rs = %d \r\n", lcd_data->reset_gpio, lcd_data->rs_gpio);
	return 0;

err1:
	gpio_free(lcd_data->reset_gpio);
err0:
	gpio_free(lcd_data->rs_gpio);
	DEBUG("[%s]:failed\n", __FUNCTION__);
	return err;

}

static inline void lcd_init(struct spi_device *spi, struct tft_lcd *pdata)
{
	int i =0;
	gpio_set_value(pdata->reset_gpio, 0); //设低电平
    msleep(10);
    gpio_set_value(pdata->reset_gpio, 1); //设高电平

	for (i = 0; i < ARRAY_SIZE(regdatas); i++)
    {
        LCD_WriteCommand(spi, regdatas[i].cmd);
        if (regdatas[i].delay_ms)
            msleep(regdatas[i].delay_ms);
    }
}
static void JLX12864G_SetPageAdd(struct spi_device *spi, u8 PageAdd)
{
	LCD_WriteCommand(spi, 0xb0 + PageAdd);
}

static void JLX12864G_SetLineAdd(struct spi_device *spi, u8 LineAdd)
{
	LineAdd += 0;
	LCD_WriteCommand(spi, 0x10 + (LineAdd >> 4));		//列地址高4位
	LCD_WriteCommand(spi, 0x00 + (LineAdd & 0x0f)); 	//列地址低4位
}


static inline void show_fb(struct fb_info *fbi, struct spi_device *spi)
{
    u8 *p = (u8 *)fbi->screen_base;
	u8 i,j;
	u8 w = fbi->var.xres;
	u8 h = fbi->var.yres;
	u8 x = 0, y = 0;
	if(w > LCD_X_SIZE) 
		w = LCD_X_SIZE;
	h /= 8;
	y /= 8; 
	
	for(i = 0;i < h;i ++)
	{
		JLX12864G_SetPageAdd(spi, y + i);	//写入页地址
		JLX12864G_SetLineAdd(spi, x);  		//写入行地址
		for(j = 0;j < w;j ++)
		{
			//p[(i+y)*w + x + j] = 0x0F;
			//printk("%d ",(i+y)*w + x + j);
			//msleep(200);
			
			LCD_WriteData(spi, p[(i+y)*w + x + j]);
		}
	
	}
	//DEBUG("refresh LCD\r\n");
}

int thread_func_fb(void *data)
{
    struct fb_info *fbi = (struct fb_info *)data;
    struct tft_lcd_fb *lcd_fb = fbi->par;

    while (1)
    {
		if (kthread_should_stop())
			break;
        show_fb(fbi, lcd_fb->spi);
        msleep(500);
    }

    return 0;
}
static u32 pseudo_palette[16];
static inline unsigned int chan_to_field(unsigned int chan, struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;
	return chan << bf->offset;
}

static int tft_lcdfb_setcolreg(unsigned int regno, unsigned int red,
			     unsigned int green, unsigned int blue,
			     unsigned int transp, struct fb_info *info)
{
	unsigned int val;
	
	if (regno > 16)
	{
		DEBUG("[%S] the regno is %d !!\n",__FUNCTION__, regno);
		return 1;
	}
		

	/* 用red,green,blue三原色构造出val  */
	val  = chan_to_field(red,	&info->var.red);
	val |= chan_to_field(green, &info->var.green);
	val |= chan_to_field(blue,	&info->var.blue);
	
	//((u32 *)(info->pseudo_palette))[regno] = val;
	pseudo_palette[regno] = val;
	return 0;
}

int tft_lcd_fb_register(struct spi_device *spi) //此函数在probe函数里被调用
{
	struct fb_info *fbi;
    u8 *v_addr;
    u32 p_addr;
	struct tft_lcd_fb *lcd_fb;

	v_addr = dma_alloc_coherent(&spi->dev, LCD_X_SIZE*LCD_Y_SIZE/8, &p_addr, GFP_KERNEL);
	if(!v_addr){
		DEBUG("[%s]:dma_alloc_coherent failed\n", __FUNCTION__);
		return -ENOMEM;
	}
	//额外分配lcd_data_t类型空间
    fbi = framebuffer_alloc(sizeof(struct tft_lcd_fb), &spi->dev);
	if(fbi == NULL)
	{
		dma_free_coherent(&spi->dev, LCD_X_SIZE*LCD_Y_SIZE/8, v_addr, &p_addr);
		DEBUG("[%s]:framebuffer_alloc failed\n", __FUNCTION__);
		return -ENOMEM;
	}
    //lcd_fb = &fbi[1]; //data指针指向额外分配的空间
    lcd_fb = fbi->par; //data指针指向额外分配的空间

	lcd_fb->spi = spi;

	fbi->pseudo_palette = pseudo_palette;
	fbi->var.activate       = FB_ACTIVATE_NOW;

    fbi->var.xres = LCD_X_SIZE;
    fbi->var.yres = LCD_Y_SIZE;
    fbi->var.xres_virtual = LCD_X_SIZE;
    fbi->var.yres_virtual = LCD_Y_SIZE;
    fbi->var.bits_per_pixel = 1; // 屏是rgb565, 但QT程序只能支持32位.还需要在刷图时把32位的像素数据转换成rgb565
    fbi->var.red.offset = 0;
    fbi->var.red.length = 0;
    fbi->var.green.offset = 0;
    fbi->var.green.length = 0;
    fbi->var.blue.offset = 0;
    fbi->var.blue.length = 0;

	strcpy(fbi->fix.id, "JLX12864G");
    fbi->fix.smem_start = p_addr; //显存的物理地址
    fbi->fix.smem_len = LCD_X_SIZE*LCD_Y_SIZE/8; 
    fbi->fix.type = FB_TYPE_PACKED_PIXELS;
    fbi->fix.visual = FB_VISUAL_MONO01; //双色 
    fbi->fix.line_length = LCD_X_SIZE/8;

	fbi->fbops = &fops;
    fbi->screen_base = v_addr; //显存虚拟地址
    //fbi->screen_base = dma_alloc_writecombine(NULL, fbi->fix.smem_len, &fbi->fix.smem_start, GFP_KERNEL);
    fbi->screen_size = LCD_X_SIZE*LCD_Y_SIZE/8;//计算显存大小

	//spi_set_drvdata(spi, fbi);
    register_framebuffer(fbi);
    lcd_fb->thread = kthread_run(thread_func_fb, fbi, spi->modalias);
    return 0; 

}
#if 0
static void tft_fb_test(struct spi_device *spi)
{

	int i,j;
	u16 color = 0x001f; /* rgb565,  蓝色 */
	
	Lcd_SetRegion(spi, 0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1); //设置从屏哪个坐标开始显示，到哪个坐标结束

	#define rgb(r, g, b)  ((((r>>3)&0x1f)<<11) | (((g>>2)&0x3f)<<5) | ((b>>3)&0x1f))
    for(i=0 ; i<Y_MAX_PIXEL/2 ; i++)
    {
		
        color = rgb(255, 0, 255); 
        for(j=0; j<X_MAX_PIXEL/2; j++)
            Lcd_WriteData_16Bit(spi, color);//(u8 *)&
        color = rgb(255, 255, 0);
        for(j=X_MAX_PIXEL/2; j<X_MAX_PIXEL; j++)
            Lcd_WriteData_16Bit(spi, color);
    }
    for(i=Y_MAX_PIXEL/2 ; i<Y_MAX_PIXEL; i++)
    {
        color = rgb(0, 255, 255);
        for(j=0; j<X_MAX_PIXEL/2; j++)
            Lcd_WriteData_16Bit(spi, color);

        color = rgb(255, 0,0);
        for(j=X_MAX_PIXEL/2; j<X_MAX_PIXEL; j++)
            Lcd_WriteData_16Bit(spi, color);
    }
}
#endif 

static int tft_lcd_probe(struct spi_device *spi)
{
	int ret;
	
	struct tft_lcd *lcd_data = devm_kzalloc(&spi->dev, sizeof(struct tft_lcd), GFP_KERNEL);

	ret = lcd_dt_parse(spi, lcd_data);
	if(ret !=0)
		goto err0;
	
	
	spi->mode = SPI_MODE_3; 
	spi->bits_per_word = 8;
	spi->max_speed_hz = 10000000;
	spi_set_drvdata(spi, lcd_data);
	lcd_init(spi, lcd_data);
	
	ret = tft_lcd_fb_register(spi); //fb设备初始化
	if(ret < 0)
	{
		goto err0;
	}
	DEBUG("[%s]:success\n",  spi->modalias);
    return 0;

err0:
	devm_kfree(&spi->dev, lcd_data);
	DEBUG("[%s]:failed ret = %d\n", __FUNCTION__, ret);
	return ret;
	
}

int tft_lcd_remove(struct spi_device *spi)
{
    struct tft_lcd *pdata = spi_get_drvdata(spi);

    DEBUG("[%s]:success\n", __FUNCTION__);
	gpio_free(pdata->rs_gpio);
    gpio_free(pdata->reset_gpio);
    return 0;
}


struct of_device_id tft_lcd_ids[] = {
    {.compatible = "mediatek,tft_lcd_spi"},
    {},
};

struct spi_driver JLX12864_drv = {
        .probe	= tft_lcd_probe,
        .remove = tft_lcd_remove,

        .driver = {
            .owner = THIS_MODULE,
            .name = "JLX12864",
            .of_match_table = tft_lcd_ids,
        },
};

module_spi_driver(JLX12864_drv);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("JLX12864 LCD SPI driver");
