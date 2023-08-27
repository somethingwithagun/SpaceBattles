#include "State.h"

State::State(sf::RenderWindow* window, std::map<std::string, int>* supported_keys)
	: m_window(window), m_supported_keys(supported_keys)
{
	m_quit = false;
	m_game_quit = false;
}

State::~State()
{
}

const bool& State::get_quit() const
{
	return m_quit;
}

const bool& State::get_game_quit() const
{
	return m_game_quit;
}


void State::check_for_quit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds["CLOSE"])))
	{
		m_game_quit = true;
	}
}

void State::setServerIp(const sf::String& ip)
{
	m_server_ip = ip;
}

void State::setServerPort(const sf::String& port)
{
	m_server_port = port;
}

void State::setUsername(const sf::String& username)
{
	m_username = username;
}

const sf::String& State::getServerIp() const
{
	return m_server_ip;
}

const sf::String& State::getServerPort() const
{
	return m_server_port;
}

const sf::String& State::getUsername() const
{
	return m_username;
}
