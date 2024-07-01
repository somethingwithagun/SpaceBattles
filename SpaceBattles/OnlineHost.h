#pragma once
#include "Online.h"
#include <SFML/Network/TcpListener.hpp>
class OnlineHost :
	public Online
{
public:
	OnlineHost(const sf::Uint16& port);
	~OnlineHost();

	void acceptSocket(); // a function used to accept incoming sockets

private:
	sf::TcpListener* m_listener = nullptr;
};

