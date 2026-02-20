/*
   File: ClientSocket.hpp
   By: Azaria
   Created: 2026/02/20 15:12:01
*/

#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

# include "Socket.hpp"

class ClientSocket : public Socket
{
	public :
		ClientSocket();
		~ClientSocket();

	private :
		ClientSocket(const ClientSocket& clientSocketType);
		ClientSocket& operator=(const ClientSocket& clientSocketType);
};

#endif /* CLIENTSOCKET_HPP */
