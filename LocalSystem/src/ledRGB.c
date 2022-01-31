#include "../inc/ledRGB.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

enum status {ON = 0, OFF};
char isLedRGBModuleActive = OFF;

const char red[] = "red";
const char green[] = "green";
const char yellow[] = "yellow";
const char idleArray[] = "idle";
status_t ledStatus = idle;

int initLedRGB(void)
{

    if( system("insmod ledRGB.ko") == 0 )
    {
        isLedRGBModuleActive = ON;
        return EXIT_SUCCESS;
    }

    return -EXIT_FAILURE;
}

void remLedRGB(void)
{
    system("rmmod ledRGB.ko");
    isLedRGBModuleActive = OFF;
    return;
}

int ledRGBStatus(status_t status)
{
    if(isLedRGBModuleActive == ON)
    {
        int fd0 = open("/dev/ledRGB0", O_WRONLY);
        if(fd0 == -1)
        {   /*ERROR OPENING ledRGB0*/
            printf("ERROR: Cannot Open ledRGB Device Driver \n");
            return -EXIT_FAILURE;
        }   

        switch (status)
        {
            case allow:
                if( write(fd0,&green,1) != 1)
                {
                    printf("Failure: Write Operation in ledRGB Device Driver \n");
                    return -EXIT_FAILURE;
                }
                ledStatus = allow;
                break;

            case denied:
                if( write(fd0,&red,1) != 1)
                {
                    printf("Failure: Write Operation in ledRGB Device Driver \n");
                    return -EXIT_FAILURE;
                }
                ledStatus = denied;
                break;

            case warning:
                if( write(fd0,&yellow,1) != 1)
                {
                    printf("Failure: Write Operation in ledRGB Device Driver \n");
                    return -EXIT_FAILURE;
                }
                ledStatus = warning;
                break;

            case idle:
                if( write(fd0,&idleArray,1) != 1)
                {
                    printf("Failure: Write Operation in ledRGB Device Driver \n");
                    return -EXIT_FAILURE;
                }
                ledStatus = idle;
                break;
            
            default:
                printf("ERROR: Invalid Status for ledRGB \n ");
                return -EXIT_FAILURE;
                break;
        }
        
        close(fd0);
        return EXIT_SUCCESS;
    }

    printf("ERROR: ledRGB module is not active");
    return -EXIT_FAILURE;


}

int getLedRGBStatus(void)
{
    if(isLedRGBModuleActive == ON)
    {
        return ledRGBStatus;
    }

    printf("ERROR: ledRGB module is not active");
    return -EXIT_FAILURE;
}




