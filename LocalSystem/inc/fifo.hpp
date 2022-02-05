/**
 * @file fifo.h
 * @author srhugo Pedro Martins
 * @brief FIFO module
 * @version 0.1
 * @date 2021-05
 * @review 2022-02-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _FIFO_MODULE_H
#define _FIFO_MODULE_H


#include <stdint.h>
#include "/home/hugo/Downloads/buildroot-2021.02.5/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/opencv2/opencv.hpp"

extern "C" {
#include "utilits.h"
}

/**
 * @brief C String Defenition
 * 
 */
typedef char arrayString[PLATESSIZE];

/**
 * @brief Char FIFO
 * 
 */
typedef struct serialFifo
{
    uint8_t writeIndex;
    uint8_t readIndex;
	uint8_t buff_len;
    char *p_buff;

}fifo8_t;

/**
 * @brief Unsigned Short FIFO
 * 
 */
typedef struct serialFifo16
{
    uint16_t writeIndex;
    uint16_t readIndex;
	uint16_t buff_len;
    uint16_t *p_buff;

}fifo16_t;

/**
 * @brief unsigned int FIFO
 * 
 */
typedef struct serialFifo32
{
    uint16_t writeIndex;
    uint16_t readIndex;
	uint16_t buff_len;
    uint32_t *p_buff;

}fifo32_t;

/**
 * @brief c String FIFO
 * 
 */
typedef struct serialFifoString
{
    uint32_t writeIndex;
    uint32_t readIndex;
	uint32_t buff_len;
    arrayString* p_buff;

}fifoString_t;


/**
 * @brief OpenCv Mat FIFO
 * 
 */
typedef struct serialFifoPhoto
{
    uint32_t writeIndex;
    uint32_t readIndex;
	uint32_t buff_len;
    cv::Mat* p_buff;

}fifoPhoto_t;



/**
 * @brief Initializes one fifo with the default indexs and 
 * and receive parameters
 * 
 * @param fifo Specifics the fifo
 * @param array Pointer to a buffer that fifo can storage info
 * @param len Buffer length
 * @return  Return EXIT_SUCESS if all went well, or ENODATA
 *						if an error occurs
 */
char fifo8_init(fifo8_t *fifo, char *array, uint8_t len);
char fifo16_init(fifo16_t *fifo, uint16_t *array, uint16_t len);
char fifo32_init(fifo32_t *fifo, uint32_t *array, uint16_t len);
int fifoString_init(fifoString_t *fifo, arrayString* array, uint32_t len);
int fifoPhoto_init(fifoPhoto_t *fifo, cv::Mat* array, uint32_t len);


/**
 * @brief Adds the wanted data into the fifo
 * 
 * @param fifo Specifics the fifo
 * @param data What the user want to add in the fifo
 * @return Return EXIT_SUCESS if all went well, or -ENOBUFS
 *						if an error occurs 
 */
char fifo8_push(fifo8_t *fifo,char byte);
char fifo16_push(fifo16_t *fifo,uint16_t integer);
char fifo32_push(fifo32_t *fifo,uint32_t integer);
int fifoString_push(fifoString_t *fifo,const char* data);
int fifoPhoto_push(fifoPhoto_t *fifo,cv::Mat data);

/**
 * @brief This funtion gets a byte from the fifo
 * 
 * @param fifo Specifics the fifo
 * @param return Where the string will be return
 * @return return one position of the buffer or 
 *      If the fifo is empty return -ENODATA
 *      Otherwise error -EXIT_FAILURE
 */
int fifo8_pop(fifo8_t *fifo);
int32_t fifo16_pop(fifo16_t *fifo);
int64_t fifo32_pop(fifo32_t *fifo);
int fifoString_pop(fifoString_t *fifo,char* returnString);
int fifoPhoto_pop(fifoPhoto_t *fifo,cv::Mat* returnString);

/**
 * @brief Get the FifoBuffSize object
 * 
 * @param fifo Specifics the fifo
 * @return Return the  
 */
char get_Fifo8BuffSize(fifo8_t fifo);
uint16_t get_Fifo16BuffSize(fifo16_t fifo);
uint16_t get_Fifo32BuffSize(fifo32_t fifo);
uint32_t get_FifoStringBuffSize(fifoString_t fifo);
uint32_t get_FifoPhotoBuffSize(fifoPhoto_t fifo);

/**
 * @brief Discarts all the fifo content. No way to reverse
 * 
 * @param fifo Specifics the fifo
 */
void clear_fifo8(fifo8_t *fifo);
void clear_fifo16(fifo16_t *fifo);
void clear_fifo32(fifo32_t *fifo);
void clear_fifoString(fifoString_t *fifo);
void clear_fifoString(fifoPhoto_t *fifo);

#endif

