#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/gpio.h> 
#include <linux/types.h>


/**************** 	DEFINES *************/

#define  PIN_PULL_OFF               0
#define  PIN_PULL_DOWN              1
#define  PIN_PULL_UP                2

#define GPIO_INPUT     0b000
#define GPIO_OUTPUT    0b001
#define GPIO_ALT_FUNC0 0b100
#define GPIO_ALT_FUNC1 0b101
#define GPIO_ALT_FUNC2 0b110
#define GPIO_ALT_FUNC3 0b111
#define GPIO_ALT_FUNC4 0b011
#define GPIO_ALT_FUNC5 0b010

#define PERIPH_BASE 0xFE000000
#define BLOCK_SIZE 		(4*1024)

#define GPIO_BASE (PERIPH_BASE + 0x200000)
#define PWM_BASE (PERIPH_BASE + 0x20C000)
#define PWM_CLK_BASE (PERIPH_BASE + 0x101000)
#define PWMCLK_CNTL	40
#define PWMCLK_DIV	41

//The Raspberry Pi PWM clock has a base frequency of 19.2 MHz
#define CLK_RATE 19200000 //Hz


#define DEVICE_NAME "ledRGB0"
#define CLASS_NAME "ledRGBClass"

#define LED_RED_PWM0 12
#define LED_GREEN_PWM1 13
#define LED_BLUE 22
/**************** 	DEFINES *************/


/**************** 	STRUCTS *************/
struct GpioRegisters
{
      uint32_t GPFSEL[6];
      uint32_t Reserved0; // Reserved byte
      uint32_t GPSET[2];
      uint32_t Reserved1;
      uint32_t GPCLR[2];
      uint32_t Reserved2;
      uint32_t GPLEV[2];
      uint32_t Reserved3;
      uint32_t GPEDS[2];
      uint32_t Reserved4;
      uint32_t GPREN[2];
      uint32_t Reserved5;
      uint32_t GPFEN[2];
      uint32_t Reserved6;
      uint32_t GPHEN[2];
      uint32_t Reserved7;
      uint32_t GPLEN[2];
      uint32_t Reserved8;
      uint32_t GPAREN[2];
      uint32_t Reserved9;
      uint32_t GPAFEN[2];
      uint32_t Reserved10;
      uint32_t GPPUD;
      uint32_t GPPUDCLK[2];
      uint32_t Reserved11[4];
};

volatile struct pwmRegisters
{
	uint32_t CTL;
	uint32_t STA;
	uint32_t DMAC;
	uint32_t reserved0;
	uint32_t RNG1;
	uint32_t DAT1;
	uint32_t FIF1;
	uint32_t reserved1;
	uint32_t RNG2;
	uint32_t DAT2;
} *s_pPwmRegisters;

struct S_PWM_CTL {
	unsigned PWEN1 : 1;
	unsigned MODE1 : 1;
	unsigned RPTL1 : 1;
	unsigned SBIT1 : 1;
	unsigned POLA1 : 1;
	unsigned USEF1 : 1;
	unsigned CLRF1 : 1;
	unsigned MSEN1 : 1;
	unsigned PWEN2 : 1;
	unsigned MODE2 : 1;
	unsigned RPTL2 : 1;
	unsigned SBIT2 : 1;
	unsigned POLA2 : 1;
	unsigned USEF2 : 1;
	unsigned Reserved1 : 1;
	unsigned MSEN2 : 1;
	unsigned Reserved2 : 16;
} *pwm_ctl;

struct S_PWM_STA {
	unsigned FULL1 : 1;
	unsigned EMPT1 : 1;
	unsigned WERR1 : 1;
	unsigned RERR1 : 1;
	unsigned GAPO1 : 1;
	unsigned GAPO2 : 1;
	unsigned Reserved1 : 2;
	unsigned BERR : 1;
	unsigned STA1 : 1;
	unsigned STA2 : 1;
	unsigned Reserved2 : 21;
} volatile *pwm_sta;
/**************** 	STRUCTS *************/


/**************** VARIABLES *************/
static struct class* ledRGBClass = NULL;
static struct device* ledRGBDevice = NULL;
static dev_t ledRGBMajorMinor;
static struct cdev c_dev;  // Character device structure

static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;
struct GpioRegisters *s_pGpioRegisters;
volatile unsigned int *s_pPwmClkRegisters;
/**************** VARIABLES *************/


/**************** FUNTIONS *************/
void SetGPIOFunction(struct GpioRegisters *s_pGpioRegisters, int GPIO, int functionCode);
/*
 * Establish the PWM frequency :
 */
static void pwm_frequency(uint32_t freq);
/*
 * Establish the PWM duty cicle :
 */
static void pwm_dutyCicle(unsigned n,unsigned m);
/**************** FUNTIONS *************/



static struct file_operations ledRGBDevice_fops = {
	.owner = THIS_MODULE,
};
 
static int __init  ledRGBModule_init(void)
{
   int ret;
	struct device *dev_ret;

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&ledRGBMajorMinor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(ledRGBClass = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(ledRGBMajorMinor, 1);
		return PTR_ERR(ledRGBClass);
	}
	if (IS_ERR(dev_ret = device_create(ledRGBClass, NULL, ledRGBMajorMinor, NULL, DEVICE_NAME))) {
		class_destroy(ledRGBClass);
		unregister_chrdev_region(ledRGBMajorMinor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &ledRGBDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, ledRGBMajorMinor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(ledRGBClass, ledRGBMajorMinor);
		class_destroy(ledRGBClass);
		unregister_chrdev_region(ledRGBMajorMinor, 1);
		return ret;
	}

	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	SetGPIOFunction(s_pGpioRegisters,LED_BLUE, GPIO_OUTPUT);
	
   //Active the blue led
   s_pGpioRegisters->GPSET[LED_BLUE / 32] = (1 << (LED_BLUE % 32));
	

	s_pPwmRegisters = (volatile struct pwmRegisters *)ioremap(PWM_BASE, sizeof(struct pwmRegisters));
	pwm_ctl = (struct S_PWM_CTL *) &s_pPwmRegisters->CTL;
	pwm_sta = (struct S_PWM_STA *) &s_pPwmRegisters->STA;
	s_pPwmClkRegisters = ioremap(PWM_CLK_BASE, BLOCK_SIZE);

   //frequency in kHz
	pwm_frequency(1000);
	pwm_dutyCicle(50, 1024);

	return 0;
}

static void __exit ledRGBModule_exit(void)
{
   pr_alert("%s: called\n",__FUNCTION__);

	s_pGpioRegisters->GPCLR[LED_BLUE/32] = (1 << (LED_BLUE % 32));

	SetGPIOFunction(s_pGpioRegisters,LED_BLUE, GPIO_INPUT);
	SetGPIOFunction(s_pGpioRegisters,LED_RED_PWM0, GPIO_INPUT);

	iounmap(s_pGpioRegisters);
	iounmap(s_pPwmRegisters);
	iounmap(s_pPwmClkRegisters);
	cdev_del(&c_dev);
	device_destroy(ledRGBClass, ledRGBMajorMinor);
	class_destroy(ledRGBClass);
	unregister_chrdev_region(ledRGBMajorMinor, 1);
}

/**************** FUNTIONS *************/
void SetGPIOFunction(struct GpioRegisters *s_pGpioRegisters, int GPIO, int functionCode) {
	int registerIndex = GPIO / 10;
	int bit = (GPIO % 10) * 3;

	unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
	unsigned mask = 0b111 << bit;

	pr_alert("%s: register index is %d\n",__FUNCTION__,registerIndex);
	pr_alert("%s: mask is 0x%x\n",__FUNCTION__,mask);
	pr_alert("%s: update value is 0x%x\n",__FUNCTION__,((functionCode << bit) & mask));
	
	s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

static void pwm_frequency(uint32_t freq) {

	long idiv;
	int rc = 0;

	/*
	 * Kill the clock:
	 */
	*(s_pPwmClkRegisters+PWMCLK_CNTL) = 0x5A000020; /* Kill clock */
	pwm_ctl->PWEN1 = 0; /* Disable PWM */
	udelay(10);

	/*
	 * Compute and set the divisor :
	 */
	idiv = (long) ( CLK_RATE / (double) freq );
	if ( idiv < 1 ) {
		idiv = 1;			/* Lowest Limit */
		rc = -1;
	} else if ( idiv >= 0x1000 ) {
		idiv = 0xFFF;			/* Highest Limit */
		rc = +1;
	}

	*(s_pPwmClkRegisters+PWMCLK_DIV) = 0x5A000000 | ( idiv << 12 );

	/*
	* Set source to oscillator and enable clock :
	*/
	*(s_pPwmClkRegisters+PWMCLK_CNTL) = 0x5A000011;
	udelay(10);
	printk("Busy clk == %d\n", (*(s_pPwmClkRegisters+PWMCLK_CNTL) & 0x80) );

   /*
      GPIO 12 (PWM0)
      Alt0	   Alt1	    Alt2	      Alt3	        Alt4	     Alt5
      PWM0	  SMI SD4	DPI D8	AVEOUT VID8	   AVEIN VID8	JTAG TMS
   */
	SetGPIOFunction(s_pGpioRegisters,LED_RED_PWM0, GPIO_ALT_FUNC0);
	printk("Sbit == %d\n", (pwm_ctl->SBIT1 & 0xFF) );

	pwm_ctl->MODE1 = 0; /* PWM mode */
	pwm_ctl->RPTL1 = 0;
	pwm_ctl->SBIT1 = 0;
	pwm_ctl->POLA1 = 0;
	pwm_ctl->USEF1 = 0;
	pwm_ctl->MSEN1 = 0;
	pwm_ctl->CLRF1 = 1;
}

static void pwm_dutyCicle(unsigned n,unsigned m) {

   /* Disable PWM */
	pwm_ctl->PWEN1 = 0;

   /*Define the ratio for duty cicle*/
	s_pPwmRegisters->RNG1 = m;
	s_pPwmRegisters->DAT1 = n;

	if ( !pwm_sta->STA1 ) {
		if ( pwm_sta->RERR1 ) pwm_sta->RERR1 = 1;
		if ( pwm_sta->WERR1 ) pwm_sta->WERR1 = 1;
		if ( pwm_sta->BERR ) pwm_sta->BERR = 1;
	}

	printk("Err == %d\n", s_pPwmRegisters -> STA );

	udelay(10); /* Pause */

   /* Enable PWM*/
	pwm_ctl->PWEN1 = 1; 
}

/**************** FUNTIONS *************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PIgate"); 
MODULE_DESCRIPTION("Led RGB PWM driver");
module_init(ledRGBModule_init);
module_exit(ledRGBModule_exit);