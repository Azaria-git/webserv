/*
   File: ClientSocket.cpp
   By: Azaria
   Created: 2026/02/20 15:11:46
*/

#include "ClientSocket.hpp"

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


/* ************************************************************************** */
/*                                Getters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                Setters                                     */
/* ************************************************************************** */


/* ************************************************************************** */
/*                             Other Methods                                  */
/* ************************************************************************** */

