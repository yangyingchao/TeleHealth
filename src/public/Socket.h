#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>


#define SERVER_PORT           "5678"

/**
 * @name PrepareSocket - Prepare a socket for connecting or listening.
 * @param forListen - Flag for Listen
 * @return int
 */
int PrepareSocket(const char* host = NULL, bool forListen = true);

#endif /* _SOCKET_H_ */
