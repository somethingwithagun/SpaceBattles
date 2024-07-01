#pragma once
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/Packet.hpp>
#include <iostream>

class Online
{
public:
	Online(const sf::Uint16& port);
	~Online();

protected:
	bool m_connected;
	bool m_client;
	bool m_host;
	sf::TcpSocket* m_socket = nullptr;

	sf::Uint16 m_port;
};

