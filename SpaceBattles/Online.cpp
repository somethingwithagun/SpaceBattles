#include "Online.h"

Online::Online(const sf::Uint16& port) : m_port(port)
{
	m_connected = false;
	m_client = false;
	m_host = false;

	m_socket = new sf::TcpSocket;
}

Online::~Online()
{
}