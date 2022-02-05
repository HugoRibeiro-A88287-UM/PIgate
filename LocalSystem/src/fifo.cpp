#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../inc/fifo.hpp"

using namespace std;
using namespace cv;


char fifo8_init(fifo8_t *fifo, char *array,uint8_t len){
		
		//Checks if array is NULL
		if(array == NULL)
			return -ENODATA; 

    fifo->writeIndex =0; //Reset indexs
    fifo->readIndex=0;
	fifo->buff_len = len;
    fifo->p_buff = array;
	
    return EXIT_SUCCESS;
}
char fifo16_init(fifo16_t *fifo, uint16_t *array, uint16_t len)
{
	//Checks if array is NULL
	if(array == NULL)
		return -ENODATA;

	fifo->writeIndex =0;
	fifo->readIndex=0;
	fifo->buff_len = len;
	fifo->p_buff = array;

	return EXIT_SUCCESS;
}
char fifo32_init(fifo32_t *fifo, uint32_t *array, uint16_t len)
{
	//Checks if array is NULL
	if(array == NULL)
		return -ENODATA;

	fifo->writeIndex =0;
	fifo->readIndex=0;
	fifo->buff_len = len;
	fifo->p_buff = array;

	return EXIT_SUCCESS;
}

int fifo8_pop(fifo8_t *fifo){
		
		//Checks if any byte to be read
		//If not return -ENODATA
    if(get_Fifo8BuffSize(*fifo) == 0 )
			return -ENODATA;
		
		
		//If yes gets the byte from the buffer and return it
    return (fifo->p_buff[fifo->readIndex++ & (fifo->buff_len-1)]);
}
int32_t fifo16_pop(fifo16_t *fifo)
{
	//Checks if any byte to be read
	//If not return -ENODATA
	if(get_Fifo16BuffSize(*fifo) == 0 )
		return -ENODATA;


	//If yes gets the byte from the buffer and return it
	return (fifo->p_buff[fifo->readIndex++ & (fifo->buff_len-1)]);
}
int64_t fifo32_pop(fifo32_t *fifo)
{
	//Checks if any byte to be read
	//If not return -ENODATA
	if(get_Fifo32BuffSize(*fifo) == 0 )
		return -ENODATA;


	//If yes gets the byte from the buffer and return it
	return (fifo->p_buff[fifo->readIndex++ & (fifo->buff_len-1)]);
}


char fifo8_push(fifo8_t *fifo,char byte){

	//Checks if there are space available in the buffer
	
	//If not, ERROR FIFO FULL
	if(get_Fifo8BuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;

	//If yes, storage the byte in position writeIndex
	fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)]= byte;
	fifo->writeIndex++;
	return EXIT_SUCCESS;
}
char fifo16_push(fifo16_t *fifo,uint16_t integer)
{
	//Checks if there are space available in the buffer

	//If not, ERROR FIFO FULL
	if(get_Fifo16BuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;

	//If yes, storage the byte in position writeIndex
	fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)]= integer;
	fifo->writeIndex++;
	return EXIT_SUCCESS;
}
char fifo32_push(fifo32_t *fifo,uint32_t integer)
{
	//Checks if there are space available in the buffer

	//If not, ERROR FIFO FULL
	if(get_Fifo32BuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;

	//If yes, storage the byte in position writeIndex
	fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)]= integer;
	fifo->writeIndex++;
	return EXIT_SUCCESS;
}

char get_Fifo8BuffSize(fifo8_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);

}
uint16_t get_Fifo16BuffSize(fifo16_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);

}
uint16_t get_Fifo32BuffSize(fifo32_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);

}

void clear_fifo8(fifo8_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}

void clear_fifo16(fifo16_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}

void clear_fifo32(fifo32_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}

int fifoString_init(fifoString_t *fifo, arrayString* array, uint16_t len)
{
	//Checks if array is NULL
	if(array == NULL)
		return -ENODATA; 

    fifo->writeIndex = 0; //Reset indexs
    fifo->readIndex = 0;
	fifo->buff_len = len;
    fifo->p_buff = array;
	
    return EXIT_SUCCESS;
}
int fifoPhoto_init(fifoPhoto_t *fifo, Mat* array, uint16_t len)
{
	//Checks if array is NULL
	if(array == NULL)
		return -ENODATA; 

    fifo->writeIndex = 0; //Reset indexs
    fifo->readIndex = 0;
	fifo->buff_len = len;
    fifo->p_buff = array;
	
    return EXIT_SUCCESS;
}


int fifoString_push(fifoString_t *fifo,const char* data)
{
	//Checks if there are space available in the buffer
	
	//If not, ERROR FIFO FULL
	if(get_FifoStringBuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;



	strcpy( fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)] , data) ;
	//If yes, storage the byte in position writeIndex

	fifo->writeIndex++;
	return EXIT_SUCCESS;
}
int fifoPhoto_push(fifoPhoto_t *fifo,Mat data)
{
	//Checks if there are space available in the buffer
	
	//If not, ERROR FIFO FULL
	if(get_FifoPhotoBuffSize(*fifo) == fifo->buff_len)
		return -ENOBUFS;



	fifo->p_buff[fifo->writeIndex & (fifo->buff_len-1)] = data ;
	//If yes, storage the byte in position writeIndex

	fifo->writeIndex++;
	return EXIT_SUCCESS;
}



int fifoString_pop(fifoString_t *fifo, char* returnString)
{
	//Checks if any byte to be read
	//If not return -ENODATA
    if(get_FifoStringBuffSize(*fifo) == 0 )
			return -ENODATA;

	//If yes gets the byte from the buffer and return it
    strcpy( returnString , (fifo->p_buff[fifo->readIndex & (fifo->buff_len-1)]));

	fifo->readIndex++;

	return EXIT_SUCCESS;

}
int fifoPhoto_pop(fifoPhoto_t *fifo, Mat* returnPhoto)
{
	//Checks if any byte to be read
	//If not return -ENODATA
    if(get_FifoPhotoBuffSize(*fifo) == 0 )
			return -ENODATA;

	//If yes gets the byte from the buffer and return it
    *returnPhoto = fifo->p_buff[fifo->readIndex & (fifo->buff_len-1)];

	fifo->readIndex++;

	return EXIT_SUCCESS;

}


uint32_t get_FifoStringBuffSize(fifoString_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);
}
uint32_t get_FifoPhotoBuffSize(fifoPhoto_t fifo)
{
	return (fifo.writeIndex - fifo.readIndex);
}


void clear_fifoString(fifoString_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}

void clear_fifoPhoto(fifoPhoto_t *fifo)
{
	fifo->readIndex = fifo->writeIndex;
}
