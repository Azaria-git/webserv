/*
   File: main.cpp
   By: Azaria
   Created: 2026/02/26 12:30:39
*/

# include "Webserv.hpp"

int main(int c, char **v)
{
	if (c != 2)
		return (1);

	Webserv webserv(v[1]);

	if (webserv.init())
		webserv.run();

	return (-1);
}
