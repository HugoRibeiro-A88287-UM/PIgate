#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/gpio.h> 

#include "utils.h"


#define DEVICE_NAME "relay0"
#define CLASS_NAME "relayClass"



MODULE_LICENSE("GPL");

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

	if (pbuff[0]=='0')
		SetGPIOOutputValue(pdev, relayOutput, 0);
	else
		SetGPIOOutputValue(pdev, relayOutput, 1);

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

	return 0;

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
	//s_pGpioRegisters = (struct GpioRegisters *)ioremap_cache(GPIO_BASE, sizeof(struct GpioRegisters));
	
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pGpioRegisters);
	
	SetGPIOFunction(s_pGpioRegisters, relayInput , 0b000); 		//Input
	SetGPIOFunction(s_pGpioRegisters, relayOutput, 0b001); 	//Output

	return 0;
}

static void __exit relayModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	
	SetGPIOFunction(s_pGpioRegisters, relayInput , 0); //Configure the pin as input
	SetGPIOFunction(s_pGpioRegisters, relayOutput, 0); 
	iounmap(s_pGpioRegisters);
	cdev_del(&c_dev);
	device_destroy(relayClass, relayMajorMinor);
	class_destroy(relayClass);
	unregister_chrdev_region(relayMajorMinor, 1);
}

module_init(relayModule_init);
module_exit(relayModule_exit);


