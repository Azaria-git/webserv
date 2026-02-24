/*
   File: webserv.cpp
   By: Azaria
   Created: 2026/02/13 15:05:40
*/

# include <iostream>
# include <unistd.h>
# include <sys/epoll.h>
# include "Socket.hpp"
# include "ServerSocket.hpp"
# include "ClientSocket.hpp"
# include "Epoll.hpp"
# include <map>

int main()
{
	ServerSocket serverSocket;

	if (!serverSocket.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
		return (1);
	if (!serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1))
		return (1);	
	if (!serverSocket.setBlocking(false))
		return (1);
	if (!serverSocket.bind("", 8080))
		return (1);
	if (!serverSocket.listen())
		return (1);

    std::cout << "Server listening on port 8080..." << std::endl;

	Epoll epoll;

	if (!epoll.epollCreate())
		return (1);

	epoll.setMaxEvent(MAX_EVENTS);

	epoll.registerFd(serverSocket.getSocketFd(), EPOLLIN);
	std::map<int, ClientData> clientsData;

	while (1)
	{
		int nFds = epoll.wait(-1);

		if (nFds < 0)
		{
			serverSocket.close();
			epoll.close();
			return (1);
		}

		for (int i = 0; i < nFds; i++)
		{
			int currentFd = epoll.getEvents()[i].data.fd;

			if (currentFd == serverSocket.getSocketFd())
			{
				ClientData clientData;
				clientData.clientInfo = serverSocket  .accept();
				
				if (clientData.clientInfo.fd < 0)
					return (1);
				
				epoll.registerFd(clientData.clientInfo.fd, EPOLLIN | EPOLLET);
				clientsData.insert(std::make_pair(clientData.clientInfo.fd, clientData));
			}
			else
			{
				if (epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP))
				{
					::close(currentFd);
					clientsData.erase(currentFd);
					continue;
				}
				if (epoll.getEvents()[i].events & EPOLLIN)
				{
					ClientSocket client(clientsData[currentFd]);

					client.setBlocking(false);
					while (true)
					{
						std::string data;

						try
						{
							data = client.recv(MAXREADBYTES);
							if (data.empty())
							{
								client.close();
								clientsData.erase(client.getSocketFd());
								break;
							}
							client.appendBuffer(data);
						}
						catch(const ClientSocket::Eagain& e)
						{
							break;
						}
					}
					if (client.getSocketFd() < 0)
					{
						continue ;
					}

					std::string response =
						"HTTP/1.0 400 KO\r\n"
						"Content-Type: text/plain\r\n"
						"\r\n"
						"Hello 42\n";

					size_t totLen = 0;
					size_t responseSize = response.size();

					while (true)
					{
						ssize_t len = 0;

						try
						{
							len = client.send(response);
							if (len <= 0)
							{
								clientsData.erase(client.getSocketFd());
								client.close();
								break;
							}
							totLen += len;
							if (totLen == responseSize)
								break;
							response = response.c_str() + totLen;
						}
						catch(const ClientSocket::Eagain& e)
						{
							epoll.modify(client.getSocketFd(), EPOLLOUT);
							clientsData[client.getSocketFd()].buffer = response;
							break;
						}
					}
				}
				if (epoll.getEvents()[i].events & EPOLLOUT)
				{
					ClientSocket client(clientsData[currentFd]);

					std::string response = client.getBuffer();

					size_t totLen = 0;
					size_t responseSize = response.size();

					while (true)
					{
						ssize_t len = 0;

						try
						{
							len = client.send(response);
							if (len <= 0)
							{
								clientsData.erase(client.getSocketFd());
								client.close();
								break;
							}
							totLen += len;
							if (totLen == responseSize)
							{
								epoll.modify(client.getSocketFd(), EPOLLIN);
								break;
							}
							response = response.c_str() + totLen;
						}
						catch(const ClientSocket::Eagain& e)
						{
							clientsData[client.getSocketFd()].buffer = response;
							break;
						}
					}
				}
			}
		}
	}
    return 0;
}

