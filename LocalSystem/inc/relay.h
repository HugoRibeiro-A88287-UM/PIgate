/**
 * @file relay.h
 * @author PIgate
 * @brief Relay Module
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef RELAY_H
#define RELAY_H


/**
 * @brief Possible Gate Status
 * 
 */
typedef enum {gateOpen = 0, gateClose} gateStatus_t;


/**
 * @brief Initialize the relay Module. This function must be called before any other. 
 * 
 * @return -EXIT_FAILURE if an error occurs ; Otherwise EXIT_SUCCESS
 */
int initRelay(void);


/**
 * @brief Deactivate the relay Module. 
 * 
 */
void remRelay(void);


/**
 * @brief Activate the relay to open the gate. 
 * To do that, this function creates a pulse that will be used to trigger the gate.
 * 
 * @return -EXIT_FAILURE if an error occurs ; Otherwise EXIT_SUCCESS
 */
int openGate(void);


/**
 * @brief Get the Gate Status object
 * 
 * @return -EXIT_FAILURE if an error occurs ; Otherwise the gate Status
 */
int getGateStatus(void);


#endif //RELAY_H