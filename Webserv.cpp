/*
   File: Webserv.cpp
   By: Azaria
   Created: 2026/02/25 09:49:55
*/

#include "Webserv.hpp"

# include <signal.h>

extern volatile sig_atomic_t stop;

/* ************************************************************************** */
/*                            Canonical Form                                  */
/* ************************************************************************** */

Webserv::Webserv()
    : _isAlreadyInit(false)
{ }

Webserv::Webserv(const Webserv& webservType)
{
    *this = webservType;
}

Webserv& Webserv::operator=(const Webserv& webservType)
{
    if (this != &webservType)
    {
        return (*this);
    }
    return *this;
}

Webserv::~Webserv()
{
    this->clear();
}

/* ************************************************************************** */
/*                         Specific Constructors                              */
/* ************************************************************************** */

Webserv::Webserv(const std::string& fileConfigName)
    :   _isAlreadyInit(false)
        ,_fileConfigName(fileConfigName)
{ }

/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

bool Webserv::init( void )
{
    if (_isAlreadyInit)
        return (true);
    try
    {
        Config configTmp(_fileConfigName);
        _config = configTmp;
    }
    catch(const Config::ConfigException& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return (false);
    }

    if (!createServerSockets())
	{
        return (false);
	}

    /*
        donne tolotra---------------------------------------------
    */
        // -- epoll
        int maxEvent = MAX_EVENTS;

        if (!_epoll.epollCreate())
            return (false);        
        _epoll.setMaxEvent(maxEvent);

        for (size_t i = 0; i < _serverSockets.size(); i++)
        {
            if (!_epoll.registerFd(_serverSockets[i]->getSocketFd(), EPOLLIN))
                return (false);
        }
    //=============================================================
    _isAlreadyInit = true;
    return (true);
}

bool Webserv::createServerSockets( void )
{
    _serverSocketsNumber = _config.getConfId() + 1;

	std::map<int, UnitConf_t> configs = _config.getConfigs();

    for (int i = 0; i < _serverSocketsNumber; i++)
    {
        ServerSocket *tmp = new ServerSocket;
        if (!tmp)
            return (false);
        _serverSockets.push_back(tmp);

        /*
            donne tolotra---------------------------------------------
        */

            // socket -----------------------
        int domain = AF_INET;
        int type = SOCK_STREAM;
        int protocol = IPPROTO_TCP;

        if (!tmp->socket(domain, type, protocol))
            return (false);

            // sockopt -----------------------
        int level = SOL_SOCKET;
        int optname = SO_REUSEADDR;
        int optlen = 1;

        if (!tmp->setsockopt(level, optname, optlen))
            return (false);	

            // blocking ----------------------
        bool blocking = false;

        if (!tmp->setBlocking(blocking))
            return (false);
            // bind --------------------------

        std::string& host = configs[i].host;
        uint16_t port = configs[i].port;

        if (!tmp->bind(host, port))
            return (false);

        /*
            -------------------------------------------------------------
        */

        int backlog = SOMAXCONN;
        if (!tmp->listen(backlog))
            return (false);

        std::cout << "Server "<< i << ":"
                << std::endl
                << "    Host = " << host << std::endl
                << "    port = " << port << std::endl
                << "    No blocking" << std::endl
                << std::endl;
    }
    return (true);
}

void  Webserv::clear( void )
{
    _epoll.close();
    _serverSocketsNumber = 0;
    for (size_t i = 0; i < _serverSockets.size(); i++)
    {
        _serverSockets[i]->close();
        delete _serverSockets[i];
    }
    _serverSockets.clear();
    _clientsData.clear();
    _isAlreadyInit = false;
}

ServerSocket* Webserv::getServerSocket(int fd) const
{
    for (size_t i = 0; i < _serverSockets.size(); i++)
    {
        if (_serverSockets[i]->getSocketFd() == fd)
            return (_serverSockets[i]);
    }
    return (0);
}

void Webserv::run(void)
{
    /*
        donne tolotra---------------------------------------------
    */

        int epollTimeOut = -1;
    //------------------------------------------------------------
	while (!stop)
	{
		int nFds = _epoll.wait(epollTimeOut);

		if (nFds < 0)
		{
            this->clear();
			return ;
		}
		for (int i = 0; i < nFds; i++)
		{
			int currentFd = _epoll.getEvents()[i].data.fd;

            ServerSocket* serverSocket = getServerSocket(currentFd);
			if (serverSocket)
			{
				ClientData clientData;
				clientData.clientInfo = serverSocket->accept();
				
				if (clientData.clientInfo.fd < 0)
					return ;
				_epoll.registerFd(clientData.clientInfo.fd, EPOLLIN | EPOLLET);
				_clientsData.insert(std::make_pair(clientData.clientInfo.fd, clientData));
			}
			else
			{
				if (_epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP))
				{
					::close(currentFd);
					_clientsData.erase(currentFd);
					continue;
				}
				if (_epoll.getEvents()[i].events & EPOLLIN)
				{
					ClientSocket client(_clientsData[currentFd]);

					if (!readClientBuffer(client))
						continue;

					// http tache ------------------------------------------
					std::string response =
						"HTTP/1.0 400 KO\r\n"
						"Content-Type: text/plain\r\n"
						"\r\n"
						"Hello 42\n";
					client.setBuffer(response);
					// ------------------------------------------------------

					writeClientResponse(client);
				}
				if (_epoll.getEvents()[i].events & EPOLLOUT)
				{
					ClientSocket client(_clientsData[currentFd]);

					writeClientResponse(client);
				}
			}
		}
	}
}

bool Webserv::readClientBuffer(ClientSocket& client)
{
	/*
		donne tolotra---------------------------------------------
	*/

	int maxReadBytes = MAXREADBYTES;

		// --- bloking
	client.setBlocking(false);

	//------------------------------------------------------------
	
	while (true)
	{
		std::string data;

		try
		{
			data = client.recv(maxReadBytes);
			if (data.empty())
			{
				client.close();
				_clientsData.erase(client.getSocketFd());
				return (false);
			}
			client.appendBuffer(data);
		}
		catch(const ClientSocket::Eagain& e)
		{
			return (true);
		}
	}
	return (true);
}

void	Webserv::writeClientResponse(ClientSocket& client)
{
	size_t bytesSend = 0;
	std::string	response = client.getBuffer();
	size_t responseSize = response.size();

	while (true)
	{
		ssize_t len = 0;

		try
		{
			len = client.send(response);
			if (len <= 0)
			{
				_clientsData.erase(client.getSocketFd());
				client.close();
				return ;
			}
			bytesSend += len;
			if (bytesSend == responseSize)
			{
				_epoll.modify(client.getSocketFd(), EPOLLIN);
				return ;
			}
			response = response.c_str() + bytesSend;
		}
		catch(const ClientSocket::Eagain& e)
		{
			_epoll.modify(client.getSocketFd(), EPOLLOUT);
			_clientsData[client.getSocketFd()].buffer = response;
			return ;
		}
	}
}
