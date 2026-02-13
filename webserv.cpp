/*
   File: webserv.cpp
   By: Azaria
   Created: 2026/02/13 15:05:40
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
# include "Socket.hpp"

int main()
{
	Socket serverSocket;

	if (!serverSocket.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
		return (1);
	if (!serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1))
		return (1);

	int serverFd = serverSocket.getSocketFd();

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    if (bind(serverFd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
	{
        std::cerr << "Error: bind failed" << std::endl;
        close(serverFd);
        return (1);
    }

    if (listen(serverFd, SOMAXCONN) < 0)
	{
        std::cerr << "Error: listen failed" << std::endl;
        close(serverFd);
        return (1);
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    int clientFd = -1;
    clientFd = accept(serverFd, NULL, NULL);
    if (clientFd < 0) {
        std::cerr << "Error: accept failed" << std::endl;
        close(serverFd);
        return (1);
    }

    std::string response = 
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello 42\n";

    ssize_t sent = write(clientFd, response.c_str(), response.size());
    if (sent < 0)
	{
        std::cerr << "Error: write failed" << std::endl;
    }

    close(clientFd);
    close(serverFd);

    return 0;
}
