/*
   File: Socket.cpp
   By: Azaria
   Created: 2026/02/13 16:01:41
*/

# include "Socket.hpp"
# include "iostream"

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Socket::Socket()
	: _socketFd(-1)
{ }

Socket::Socket(const Socket& socketType)
{
	*this = socketType;
}

Socket& Socket::operator=(const Socket& socketType)
{
	if (this != &socketType)
	{
		_socketFd = socketType._socketFd;
	}
	return (*this);
}

Socket::~Socket()
{ }

/* ************************************************************************** */
/*                         Specific Constructors                              */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */

int Socket::getSocketFd( void ) const
{
	return (_socketFd);
}

/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

bool Socket::socket(int domain, int type, int protocol)
{
    _socketFd = ::socket(domain, type, protocol);

    if (_socketFd < 0)
	{
		std::cerr << "Error: Cannot create socket" << std::endl;
        return (false);
	}
	return (true);
}

bool Socket::setsockopt(int level, int optname, unsigned int optlen)
{
    int opt = optlen;

    if (::setsockopt(_socketFd, level, optname, &opt, sizeof(opt)) < 0)
	{
        std::cerr << "Error: setsockopt failed" << std::endl;
        return (false);
    }
	return (true);
}
