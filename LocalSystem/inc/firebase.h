/**
 * @file firebase.h
 * @author PIgate
 * @brief Firebase communication Module
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef FIREBASE_H
#define FIREBASE_H

/**
 * @brief Sends an Entry to the database
 * 
 * @param PIgate_ID 
 * @param Plate 
 * @return -EXIT_FAILURE: if and error occurs ;
 *          Otherwise EXIT_SUCCESS 
 *        
 */
int sendEntry(const char* PIgate_ID, const char* Plate);


/**
 * @brief Requests and Receive all the existent plates in the database.
 * Also it put all the received plates into the recPlatePIPE
 * 
 * @return -EXIT_FAILURE: if and error occurs ;
 *          Otherwise EXIT_SUCCESS 
 */
int receivePlates(void);


/**
 * @brief Verifies if the user wants to open the gate by the WebApp
 * 
 * @param PIgate_ID 
 * @return -EXIT_FAILURE if and error occurs , 
 *          0 to Not Open the Gate and 
 *          1 to Open the Gate ;
 */
int isToOpen(const char* PIgate_ID);


/**
 * @brief Checks if the given PIgate_ID exists in the Database
 * 
 * @param PIgate_ID 
 * @return -EXIT_FAILURE: if and error occurs ;
 *          Otherwise EXIT_SUCCESS 
 */
int validPIgate(const char* PIgate_ID);

#endif //FIREBASE_H