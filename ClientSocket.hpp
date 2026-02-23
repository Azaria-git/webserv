/*
   File: ClientSocket.hpp
   By: Azaria
   Created: 2026/02/20 15:12:01
*/

#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

# include <exception>
# include "Socket.hpp"
# define MAXREADBYTES 1024
#define	CHARARRAY(name, size) char name[size]

class ClientSocket : public Socket
{
	public :
		ClientSocket(const SocketInfo& socketInfo);
		~ClientSocket();

		std::string recv(unsigned int count);
		void 		appendBuff(const std::string& newBuff);
		std::string getBuff( void ) const;

		class Eagain : public std::exception{ };

	private :
		ClientSocket();
		ClientSocket(const ClientSocket& clientSocketType);
		ClientSocket& operator=(const ClientSocket& clientSocketType);

		std::string _buff;
};

#endif /* CLIENTSOCKET_HPP */
