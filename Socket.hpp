/*
   File: Socket.hpp
   By: Azaria
   Created: 2026/02/13 16:01:56
*/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
# include <string>

typedef struct sockaddr_in sockaddrIn;

class Socket
{
	public :
		Socket();
		~Socket();

		bool	socket(int domain, int type, int protocol=0);

		int		getSocketFd( void ) const;
	    bool	setsockopt(int level, int optname, unsigned int optlen);

		bool	bind(const std::string& host, uint16_t port);
		bool	inetPToN(int domain, const std::string& ipString
		, uint32_t& ipNum) const;
		bool	listen(int backlog=SOMAXCONN);
		bool 	accept(Socket& clientSocket);
		bool	setBlocking(bool set);

	private :
		Socket(const Socket& socketType);
		Socket& operator=(const Socket& socketType);

		int			_socketFd;
	    sockaddrIn 	_addr;
		socklen_t 	_AddrLen;
};

bool stringToInt(const std::string& str, int& outValue);

#endif /* SOCKET_HPP */
