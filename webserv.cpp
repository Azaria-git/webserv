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

	std::map<int, SocketInfo> clientsInfo;

	while (1)
	{
		int nFds = epoll.wait(-1);

		if (nFds < 0)
			return (1);

		for (int i = 0; i < nFds; i++)
		{
			int currentFd = epoll.getEvents()[i].data.fd;

			if (currentFd == serverSocket.getSocketFd())
			{
				SocketInfo newClient = serverSocket.accept();
				
				if (newClient.fd < 0)
					return (1);
				
				epoll.registerFd(newClient.fd, EPOLLIN | EPOLLET);
				clientsInfo.insert(std::make_pair(newClient.fd, newClient));
			} 
			else
			{
				if (epoll.getEvents()[i].events & (EPOLLERR | EPOLLHUP))
				{
					::close(currentFd);
					clientsInfo.erase(currentFd);
					continue;
				}

				ClientSocket client(clientsInfo[currentFd]);

				client.setBlocking(false);
				if (epoll.getEvents()[i].events & EPOLLIN)
				{
					// char buffer[1024];
					// int bytes;
					// while ((bytes = read(fd, buffer, sizeof(buffer))) > 0) {
					// 	c.readBuffer.append(buffer, bytes);
					// }

					// if (bytes == 0) {
					// 	// Client a fermé la connexion
					// 	close(fd);
					// 	clients.erase(fd);
					// 	continue;
					// }
					// // Ici tu peux traiter c.readBuffer et préparer c.writeBuffer
					// // Exemple : echo
					// c.writeBuffer += c.readBuffer;
					// c.readBuffer.clear();

					// // Ajouter EPOLLOUT si on a des données à écrire
					// if (!c.writeBuffer.empty()) {
					// 	struct epoll_event ev_out;
					// 	ev_out.events = EPOLLIN | EPOLLOUT | EPOLLET;
					// 	ev_out.data.fd = fd;
					// 	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev_out);
					// }
					while (true)
					{
						std::string data;

						try
						{
							data = client.recv(MAXREADBYTES);
						}
						catch(const ClientSocket::Eagain& e)
						{
							break;
						}
						if (data.empty())
						{
							client.close();
							clientsInfo.erase(currentFd);
							break;
						}
						client.appendBuff(data);
					}
					if (client.getBuff().empty())
					{
						continue ;
					}
					std::cout << "ok\n";
					epoll.modify(currentFd, EPOLLOUT);
				}
				if (epoll.getEvents()[i].events & EPOLLOUT)
				{
					std::cout << "hahaha writer\n";
					// while (!c.writeBuffer.empty()) {
					// 	int bytes = write(fd, c.writeBuffer.c_str(), c.writeBuffer.size());
					// 	if (bytes < 0) {
					// 		if (errno == EAGAIN) break; // socket pleine
					// 	}
					// 	c.writeBuffer.erase(0, bytes);
					// }

					// if (c.writeBuffer.empty()) {
					// 	struct epoll_event ev_in;
					// 	ev_in.events = EPOLLIN | EPOLLET;
					// 	ev_in.data.fd = fd;
					// 	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev_in);
					// }

					std::string response =
						"HTTP/1.0 400 KO\r\n"
						"Content-Type: text/plain\r\n"
						"\r\n"
						"Hello 42\n";

					ssize_t sent = write(client.getSocketFd(), response.c_str(), response.size());
					if (sent < 0)
					{
						std::cerr << "Error: write failed" << std::endl;
					}
					client.close();
				}
			}
		}
	}
    return 0;
}
