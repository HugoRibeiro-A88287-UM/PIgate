/**
 * @file ledRGB.h
 * @author PIgate
 * @brief ledRGB Module
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LEDRGB_H
#define LEDRGB_H

/**
 * @brief Possible LED status
 * 
 */
typedef enum {allow = 0 , denied , warning, idle}status_t;


/**
 * @brief Initialize the Module. This functions must be called before any other.
 * 
 * @return -EXIT_FAILURE if an error occurs ; Otherwise EXIT_SUCCESS
 */
int initLedRGB(void);


/**
 * @brief Deactivate the ledRGB Module
 * 
 */
void remLedRGB(void);


/**
 * @brief Put the RGB leds with the respective state color
 * 
 * @param status Program Status
 * @return -EXIT_FAILURE if an error occurs ; Otherwise EXIT_SUCCESS 
 */
int ledRGBStatus(status_t status);


/**
 * @brief Get the LedRGB Status object
 * 
 * @return -EXIT_FAILURE if an error occurs ; Otherwise returns the status 
 */
int getLedRGBStatus(void);


#endif //LEDRGB_H