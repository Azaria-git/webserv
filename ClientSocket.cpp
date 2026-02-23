/*
   File: ClientSocket.cpp
   By: Azaria
   Created: 2026/02/20 15:11:46
*/

# include "ClientSocket.hpp"
# include <unistd.h>
# include <cerrno>

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

ClientSocket::ClientSocket()
	: Socket()
{ }

ClientSocket::ClientSocket(const ClientSocket& clientSocketType)
	: Socket(clientSocketType)
{
	*this = clientSocketType;
}

ClientSocket& ClientSocket::operator=(const ClientSocket& clientSocketType)
{
	if (this != &clientSocketType)
	{
		Socket::operator=(clientSocketType);
	}
	return (*this);
}

ClientSocket::~ClientSocket()
{ }

/* ************************************************************************** */
/*                         Specific Constructors                              */
/* ************************************************************************** */

ClientSocket::ClientSocket(const SocketInfo& socketInfo)
	: Socket()
{
	this->_socketFd = socketInfo.fd;
	this->_addr = socketInfo.addr;
	this->_addrLen = socketInfo.addrlen;
}


/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */

std::string ClientSocket::getBuff( void ) const
{
	return (_buff);
}

/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

std::string ClientSocket::recv(unsigned int count)
{
	CHARARRAY(buff, count);

	int bytes = ::read(_socketFd, buff, count);

	if (errno == EAGAIN)
		throw Eagain();

	if (bytes == 0)
		return ("");
	return (std::string(buff, bytes));
}

void ClientSocket::appendBuff(const std::string& newBuff)
{
	_buff += newBuff;
}
