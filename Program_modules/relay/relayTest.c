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
    printf("Relay Device Driver Test\n");

    unsigned int count = 0;
    printf("\n");

    printf("\n\nInserting Device Driver...\n");
    system("insmod relay.ko");

    printf("\nCheck devicer driver:\n");
    system("lsmod");

    printf("\nIs the device driver in /dev:\n");
    system("ls -l /dev/relay0");
    sleep(3);

    int fd0 = open("/dev/relay0", O_RDWR);
    char ledOn = '1';
    char ledOff = '0';
    char pBuff;


    printf("\n Testing Output relay (2 times): \n");
    sleep(1);
    while(count != 2)
    {
        
        write(fd0, &ledOn, 1);
        printf("Relay ON! \n ");
        read(fd0, &pBuff, 1);
        printf("Relay Value: %c \n", pBuff);
        sleep(4);
       
        write(fd0, &ledOff, 1);
        printf("Relay OFF! \n ");
        read(fd0,&pBuff,1);
        printf("Relay Value: %c \n", pBuff);
        sleep(4);
        count++;
    }

    printf("Closing Device Driver.\n");
    close(fd0);

    putchar('\n');
    printf("Removing Device Driver.\n");
    system("rmmod relay");

    return 0;

}