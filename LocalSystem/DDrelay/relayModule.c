#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/timer.h>
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

#define GPIO_BASE (PERIPH_BASE + 0x200000) // GPIO controller

struct GpioRegisters
{
      uint32_t GPFSEL[6];
      uint32_t Reserved0;
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


#define DEVICE_NAME "relay0"
#define CLASS_NAME "relayClass"
/**************** 	DEFINES *************/

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


/**************** FUNTIONS *************/

/* Device variables */
static struct class* relayClass = NULL;
static struct device* relayDevice = NULL;
static dev_t relayMajorMinor;
static struct cdev c_dev;  // Character device structure


static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;
struct GpioRegisters *s_pGpioRegisters;


static const int relayInput = 27;
static const int relayOutput = 4;


ssize_t relay_device_write(struct file *pfile, const char __user *pbuff, size_t len, loff_t *off) {
	struct GpioRegisters *pdev; 
	
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);

	
	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	pdev = (struct GpioRegisters *)pfile->private_data;

	pr_alert("%s: register value is 0x%x\n",__FUNCTION__,(1<<(relayOutput %32)));
			 
	if (pbuff[0]=='C')
		s_pGpioRegisters->GPCLR[relayOutput / 32] = (1 << (relayOutput % 32));
	else
		s_pGpioRegisters->GPSET[relayOutput / 32] = (1 << (relayOutput % 32));

	return len;
}

ssize_t relay_device_read(struct file *pfile, char __user *pbuff,size_t len, loff_t *poffset){
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);

	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	char buffer[2];
	int i = gpio_get_value(relayInput);
	sprintf(buffer, "%d", i);

	copy_to_user(pbuff, buffer, 1);
	printk(KERN_INFO "PIN -> %d\n", i);

	return len;

}

int relay_device_close(struct inode *p_inode, struct file * pfile){
	
	pr_alert("%s: called\n",__FUNCTION__);
	pfile->private_data = NULL;
	return 0;
}


int relay_device_open(struct inode* p_indode, struct file *p_file){

	pr_alert("%s: called\n",__FUNCTION__);
	p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters;
	return 0;
	
}


static struct file_operations relayDevice_fops = {
	.owner = THIS_MODULE,
	.write = relay_device_write,
	.read = relay_device_read,
	.release = relay_device_close,
	.open = relay_device_open,
};

static int __init relayModule_init(void) {
	int ret;
	struct device *dev_ret;

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&relayMajorMinor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(relayClass = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(relayMajorMinor, 1);
		return PTR_ERR(relayClass);
	}
	if (IS_ERR(dev_ret = device_create(relayClass, NULL, relayMajorMinor, NULL, DEVICE_NAME))) {
		class_destroy(relayClass);
		unregister_chrdev_region(relayMajorMinor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &relayDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, relayMajorMinor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(relayClass, relayMajorMinor);
		class_destroy(relayClass);
		unregister_chrdev_region(relayMajorMinor, 1);
		return ret;
	}

	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	
	SetGPIOFunction(s_pGpioRegisters, relayInput , GPIO_INPUT); 		
	SetGPIOFunction(s_pGpioRegisters, relayOutput, GPIO_OUTPUT);

	printk("Success: Relay initialization"); 	

	return 0;
}

static void __exit relayModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	
	SetGPIOFunction(s_pGpioRegisters, relayInput , GPIO_INPUT); 
	SetGPIOFunction(s_pGpioRegisters, relayOutput, GPIO_INPUT); 
	iounmap(s_pGpioRegisters);
	cdev_del(&c_dev);
	device_destroy(relayClass, relayMajorMinor);
	class_destroy(relayClass);
	unregister_chrdev_region(relayMajorMinor, 1);

	printk("Success: Relay Exit"); 	
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PIgate"); 
MODULE_DESCRIPTION("Relay driver");
module_init(relayModule_init);
module_exit(relayModule_exit);


