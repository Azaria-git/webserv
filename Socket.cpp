/*
   File: Socket.cpp
   By: Azaria
   Created: 2026/02/13 16:01:41
*/

# include "Socket.hpp"
# include "iostream"
# include <sstream>
# include <vector>
# include <unistd.h>
# include <fcntl.h>

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Socket::Socket()
	: _socketFd(-1)
{
	this->_AddrLen = sizeof(this->_addr);
}

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
{
	if (_socketFd >= 0)
		::close(_socketFd);
}

/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */

int Socket::getSocketFd( void ) const
{
	return (_socketFd);
}

/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

bool Socket::socket(int domain, int type, int protocol)
{
    _addr.sin_family = domain;
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

bool	Socket::bind(const std::string& host, uint16_t port)
{
	_addr.sin_port = htons(port);

	uint32_t addrTmp;
	if (host.empty())
	    _addr.sin_addr.s_addr = addrTmp = INADDR_ANY;
	else
	{
		if (inetPToN(_addr.sin_family, host, addrTmp))
			return (false);
	    _addr.sin_addr.s_addr = addrTmp;
	}

    if (::bind(_socketFd, reinterpret_cast<struct sockaddr*>(&_addr), sizeof(_addr)) < 0)
	{
        std::cerr << "Error: bind failed" << std::endl;
        return (false);
    }
	return (true);
}

bool Socket::inetPToN(int domain, const std::string& ipString, uint32_t& ipNum) const
{
	if (domain != AF_INET)
		return (false);

    std::stringstream ss(ipString);
    std::string token;
    std::vector<int> parts;
	int num = 0;
	int i = 0;

	while (std::getline(ss, token, '.'))
	{
		if (i > 3)
			return (false);
		if (!stringToInt(token, num))
			return (false);
        if(num < 0 || num > 255)
			return (false);
		std::cout << "num: " << num  << std::endl;
        parts.push_back(num);
		i++;
	}
	ipNum = 0;
    ipNum = (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
	ipNum = htonl(ipNum);
	return (true);
}

bool Socket::listen(int backlog)
{
    if (::listen(_socketFd, backlog) < 0)
	{
        std::cerr << "Error: listen failed" << std::endl;
        return (false);
    }
	return (true);
}

bool 	Socket::accept(Socket& clientSocket)
{
    clientSocket._socketFd = ::accept(_socketFd
		, reinterpret_cast<struct sockaddr*>(&clientSocket._addr)
		, &(this->_AddrLen));
    if (clientSocket._socketFd < 0) {
        std::cerr << "Error: accept failed" << std::endl;
        return (false);
    }
	return (true);
}

bool Socket::setBlocking(bool set)
{
	if (_socketFd < 0)
		return (false);

    int flags = fcntl(_socketFd, F_GETFL, 0);

	if (flags == -1)
        return (false);

	if (set)
	{
		if (fcntl(_socketFd, F_SETFL, flags & ~O_NONBLOCK) == -1)
		{
	        std::cerr << "Error: Can't set flags blocking" << std::endl;
			return (false);
		}
	}
	else
	{
		if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) == -1)
		{
	        std::cerr << "Error: Can't set flags Non-blocking" << std::endl;
			return (false);
		}
	}
    return (true);
}

/* ************************************************************************** */
/*                             External Fonctions                             */
/* ************************************************************************** */


bool stringToInt(const std::string& str, int& outValue)
{
    std::stringstream ss(str);
    ss >> outValue;

	if (ss.fail() || !ss.eof())
        return (false);
    return (true);
}
