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
	while (1)
	{
		int nFds = epoll.wait(-1);

		if (nFds < 0)
			return (1);

		for (int i = 0; i < nFds; i++)
		{
			if (epoll.getEvents()[i].data.fd == serverSocket.getSocketFd())
			{
				Socket clientSocket;
				serverSocket.accept(clientSocket);

				clientSocket.setBlocking(false);

				epoll.registerFd(clientSocket.getSocketFd(), EPOLLIN);

				std::string response =
					"HTTP/1.0 400 OK\r\n"
					"Content-Type: text/plain\r\n"
					"\r\n"
					"Hello 42\n";

				ssize_t sent = write(clientSocket.getSocketFd(), response.c_str(), response.size());
				if (sent < 0)
				{
					std::cerr << "Error: write failed" << std::endl;
				}
			}
		}
	}
    return 0;
}
