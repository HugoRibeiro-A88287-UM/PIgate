#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 


// struct pwm_device { 
//    const char *label; 
//    unsigned long flags; 
//    unsigned int hwpwm; 
//    unsigned int pwm; 
//    struct pwm_chip *chip; 
//    void *chip_data; 
 
//    struct pwm_args args;  
//    struct pwm_state state;
// }; 

void simpleDelay (void)
{
    volatile unsigned int i;
    for (i = 0; i != 100000000; i++);
}

int main(void)
{
    printf("ledRGB Device Driver Test\n");

    unsigned int count = 0;
    printf("\n");

    printf("\n\nInserting Device Driver...\n");
    system("insmod ledRGB.ko");

    printf("\nCheck devicer driver:\n");
    system("lsmod");

    printf("\nIs the device driver in /dev:\n");
    system("ls -l /dev/ledRGB0");
    sleep(3);

    //static void pwm_led_drive(struct pwm_device *pwm, struct private_data *priv) 

    /* Configure the PWM, applying a period and duty cycle */ 
    // pwm_config(pwm, priv->duty, priv->pwm_period); 
 

    // printf("\n Testing Output relay (2 times): \n");
    // sleep(1);
    // while(count != 2)
    // {
        
    //     /* Start toggling */ 
    //     pwm_enable(pchip->pwmd);
    //     sleep(4);
       
    //     /* And then stop toggling*/ 
    //     pwm_disable(pchip->pwmd); 
    //     sleep(4);
    //     count++;
    // }

    printf("Closing Device Driver.\n");

    putchar('\n');
    printf("Removing Device Driver.\n");
    system("rmmod ledRGB");

    return 0;

}
