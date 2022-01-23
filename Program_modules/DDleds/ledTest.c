#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 

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

    int fd0 = open("/dev/ledRGB0", O_RDWR);
    const char r[] = "red";
    const char g[] = "green";
    const char y[] = "yellow";
    const char i[] = "idle";
    char receive[] = " ";


    printf("\n Testing led RGB: \n");
    sleep(1);
    while(count != 1)
    {
        write(fd0, &y, 1);
        printf("Warning Color: %s \n ", y);

        sleep(7);

        write(fd0, &g, 1);
        printf("Allowed Color: %s \n ", g );
        read(fd0,&receive,1);
        printf("PwmState: %s \n ", receive );
        

        sleep(7);


        write(fd0, &r, 1);
        printf("Denied Color: %s \n ", r );

        sleep(7);

        write(fd0, &i, 1);
        printf("Idle Color: %s \n ", i);

        sleep(7);
        
        count++;
    }

    read(fd0,&receive,1);
    printf("PwmState: %s \n ", receive );

    printf("Closing Device Driver.\n");

    putchar('\n');
    printf("Removing Device Driver.\n");

    system("rmmod ledRGB");
    

    return 0;

}
