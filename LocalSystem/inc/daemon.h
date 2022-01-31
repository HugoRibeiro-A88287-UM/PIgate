/**
 * @file daemon.h
 * @author PIgate
 * @brief Daemons Module
 * @version 0.1
 * @date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DAEMON_H
#define DAEMON_H

/**
 * @brief Inits the EntriesDB daemon
 * 
 * @return Daemon's PID in success
 *         EACCES if it isn't the first time 
 */
pid_t initDaemonEntriesDB(void);

/**
 * @brief Inits the UpdatePlate daemon
 * 
 * @return Daemon's PID in success
 *         EACCES if it isn't the first time 
 */
pid_t initDaemonUpdatePlate(void);

/**
 * @brief Inits the OpenGateDB daemon
 * 
 * @return Daemon's PID in success
 *         EACCES if it isn't the first time 
 */
pid_t initDaemonOpenGateDB(void);


#endif //DAEMON_H