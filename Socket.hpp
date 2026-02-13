/*
   File: Socket.hpp
   By: Azaria
   Created: 2026/02/13 16:01:56
*/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>

class Socket
{
	public :
		Socket();
		~Socket();

		bool socket(int domain, int type, int protocol=0);

		int getSocketFd( void ) const;
	    bool setsockopt(int level, int optname, unsigned int optlen);

	private :
		Socket(const Socket& socketType);
		Socket& operator=(const Socket& socketType);

		int _socketFd;
};

#endif /* SOCKET_HPP */
