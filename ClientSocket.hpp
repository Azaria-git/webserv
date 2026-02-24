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

typedef struct SClientData
{
	SocketInfo clientInfo;

    std::string buffer;
} 		ClientData;

class ClientSocket : public Socket
{
	public :
		ClientSocket(const ClientData& clientData);
		~ClientSocket();

		std::string getBuffer( void ) const;
		void setBuffer(const std::string& buff);


		std::string recv(unsigned int count);
		ssize_t 	send(const std::string& buff);
		void 		appendBuffer(const std::string& newBuff);
		class Eagain : public std::exception{ };

	private :
		ClientSocket();
		ClientSocket(const ClientSocket& clientSocketType);
		ClientSocket& operator=(const ClientSocket& clientSocketType);

		std::string _buffer;
};

#endif /* CLIENTSOCKET_HPP */
