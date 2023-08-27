#include "LobbyState.h"
#include <iostream>

LobbyState::LobbyState(sf::RenderWindow* w, std::map<std::string, int>* supported_keys) : State(w, supported_keys)
{

	m_font = new sf::Font;
	m_font->loadFromFile("fonts/FiraCode-Medium.ttf");

	m_exit_button = new Button(sf::Vector2f(50, 50), "X", *m_font);
	m_exit_button->setPosition(sf::Vector2f(m_window->getSize().x - 20, 20));
	//m_exit_button->setPosition(sf::Vector2f(900, 900));

	m_start_button = new Button(sf::Vector2f(300, 100), "Start", *m_font, sf::Color(56, 176, 70), 30);
	m_start_button->setPosition(sf::Vector2f(220, m_window->getSize().y - 130));

	m_connect_button = new Button(sf::Vector2f(300, 100), "Connect", *m_font, sf::Color(56, 176, 70), 30);
	m_connect_button->setPosition(sf::Vector2f(m_start_button->getPosition().x + m_start_button->getSize().x + 40, m_window->getSize().y - 130));

	m_create_button = new Button(sf::Vector2f(300, 100), "Create", *m_font, sf::Color(56, 176, 70), 30);
	m_create_button->setPosition(sf::Vector2f(m_connect_button->getPosition().x + m_connect_button->getSize().x + 40, m_window->getSize().y - 130));

	m_username_input.setFont(m_font);
	m_username_input.setPosition(sf::Vector2f(200, 100));
	m_username_input.setAlign(TextInput::TextAlign::Left);
	m_username_input.setPlaceholderText("Username");

	m_server_ip_input.setFont(m_font);
	m_server_ip_input.setPosition(sf::Vector2f(200, 150));
	m_server_ip_input.setCharactersLimit(15);
	m_server_ip_input.setAlign(TextInput::TextAlign::Left);
	m_server_ip_input.setPlaceholderText("Host ip");

	m_server_port_input.setCharactersLimit(5);
	m_server_port_input.setFont(m_font);
	m_server_port_input.setPosition(sf::Vector2f(200, 200));
	m_server_port_input.setAlign(TextInput::TextAlign::Left);
	m_server_port_input.setPlaceholderText("Port");

	m_background_texture = new sf::Texture;
	m_background_texture->loadFromFile("images/menu-screen.png");

	m_background = new sf::RectangleShape(sf::Vector2f(m_window->getSize()));
	m_background->setTexture(m_background_texture);

	this->init_keybinds();
}

LobbyState::~LobbyState()
{
	delete m_start_button;
	delete m_exit_button;

	delete m_font;
}

void LobbyState::push_next_state(std::stack<State*>& states)
{
	states.push(new GameState(m_window, m_supported_keys));
}

void LobbyState::update(const float& dt)
{
	bool username_entered;
	bool ip_entered;
	bool port_entered;

	username_entered = ip_entered = port_entered = false;

	if (m_username_input.getText().getSize() < 4)
		username_entered = false;
	else
		username_entered = true;

	if (m_server_ip_input.getText().getSize() < 8)
		ip_entered = false;
	else
		ip_entered = true;

	if (m_server_port_input.getText().getSize() < 3)
		port_entered = false;
	else
		port_entered = true;

	if (username_entered && ip_entered && port_entered)
	{
		m_connect_button->setEnabled(true);
	}
	else
		m_connect_button->setEnabled(false);

	if (username_entered && port_entered)
	{
		m_create_button->setEnabled(true);
	}
	else
		m_create_button->setEnabled(false);

	if (username_entered)
	{
		m_start_button->setEnabled(true);
	}
	else
		m_start_button->setEnabled(false);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (m_exit_button->getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
		{
			if (m_counterA >= 0.006)
			{
				m_exit_button->setPressed(true);
				m_counterA = 0;

				m_game_quit = true;

				//m_exit_button_pressed = true;
			}
		}
		else if (m_start_button->getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_start_button->getEnabled())
		{
			if (m_counterB >= 0.006)
			{
				m_start_button->setPressed(true);
				m_counterB = 0;

				m_username = m_username_input.getText();

				m_quit = true;
			}
		}
		else if (m_connect_button->getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_create_button->getEnabled())
		{
			if (m_counterB >= 0.006)
			{
				m_connect_button->setPressed(true);
				m_counterB = 0;

				m_username = m_username_input.getText();
				m_server_ip = m_server_ip_input.getText();
				m_server_port = m_server_port_input.getText();

				m_quit = true;
			}
		}
		else if (m_create_button->getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_create_button->getEnabled())
		{
			if (m_counterB >= 0.006)
			{
				m_create_button->setPressed(true);
				m_counterB = 0;

				m_username = m_username_input.getText();
				m_server_port = m_server_port_input.getText();

				m_quit = true;
			}
		}
		else if (m_username_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
		{
			if (m_counterC >= 0.006)
			{
				m_username_input.setActive(true);
				m_counterC = 0;
			}
		}
		else if (m_server_ip_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
		{
			if (m_counterC >= 0.006)
			{
				m_server_ip_input.setActive(true);
				m_counterC = 0;
			}
		}

		else if (m_server_port_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
		{
			if (m_counterC >= 0.006)
			{
				m_server_port_input.setActive(true);
				m_counterC = 0;
			}
		}

		if (!m_server_port_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_server_port_input.getActive())
			m_server_port_input.setActive(false);

		if (!m_server_ip_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_server_ip_input.getActive())
			m_server_ip_input.setActive(false);

		if (!m_username_input.getGlobalBounds().contains(
			static_cast<sf::Vector2f>(sf::Mouse::getPosition())) && m_username_input.getActive())
			m_username_input.setActive(false);
	}

	update_keybinds(dt);
	update_input(dt);

	m_exit_button->update(dt);
	m_start_button->update(dt);
	m_connect_button->update(dt);
	m_create_button->update(dt);

	m_username_input.update(dt);
	m_server_ip_input.update(dt);
	m_server_port_input.update(dt);

	m_counterA += dt;
	m_counterB += dt;
	m_counterC += dt;
}

void LobbyState::update_input(const float& dt)
{
	this->check_for_quit();
}

void LobbyState::update_keybinds(const float& dt)
{
	this->check_for_quit();
}

void LobbyState::render(sf::RenderTarget* target)
{
	target->draw(*m_background);
	target->draw(*m_exit_button);
	target->draw(*m_start_button);
	target->draw(*m_connect_button);
	target->draw(*m_create_button);

	//std::cout << m_start_button->getSize().x << ' ' << m_start_button->getSize().y << ' ' << m_start_button->getPosition().x << ' ' << m_start_button->getPosition().y << std::endl;

	m_username_input.render(target);
	m_server_ip_input.render(target);
	m_server_port_input.render(target);
}

void LobbyState::process_event(sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::TextEntered:

		if (ev.text.unicode > 'z')
			std::cerr << "LOBBYSTATE::PROCESS_EVENT::Error character is out of range!" << std::endl;
		else
		{
			if (m_username_input.getActive())
			{
				if (std::isprint(ev.text.unicode))
					m_username_input.setText(m_username_input.getText() + char(ev.text.unicode));
				else if (ev.text.unicode == 8) // backspace
					m_username_input.setText(m_username_input.getText().substring(0, m_username_input.getText().getSize() - 1));

			}
			else if (m_server_ip_input.getActive())
			{
				if (std::isprint(ev.text.unicode))
					m_server_ip_input.setText(m_server_ip_input.getText() + char(ev.text.unicode));
				else if (ev.text.unicode == 8) // backspace
					m_server_ip_input.setText(m_server_ip_input.getText().substring(0, m_server_ip_input.getText().getSize() - 1));
			}

			if (m_server_port_input.getActive())
			{
				if (std::isdigit(ev.text.unicode))
					m_server_port_input.setText(m_server_port_input.getText() + char(ev.text.unicode));
				else if (ev.text.unicode == 8) // backspace
					m_server_port_input.setText(m_server_port_input.getText().substring(0, m_server_port_input.getText().getSize() - 1));
			}
		}
		break;

	case sf::Event::KeyPressed:
		if (ev.key.control && ev.key.code == sf::Keyboard::V)
		{
			if (m_username_input.getActive())
				m_username_input.setText(m_username_input.getText() + sf::Clipboard::getString());
			else if (m_server_ip_input.getActive())
				m_server_ip_input.setText(m_server_ip_input.getText() + sf::Clipboard::getString());
			else if (m_server_port_input.getActive())
				m_server_port_input.setText(m_server_port_input.getText() + sf::Clipboard::getString());

		}
		break;

	default:
		break;
	}
}

bool LobbyState::end_state()
{

	if (m_game_quit)
	{
		// exit
		//m_window->close();
		return true;
	}

	// else start the game
	// just do nothing
	return true;
}

void LobbyState::init()
{
}

void LobbyState::init_keybinds()
{
	std::ifstream keys("config/lobbystate_keybinds.ini");

	if (!keys.is_open())
	{
		std::cout << "Cannot open the gamestate keybinds config! Setting the default keybinds\n";

		m_keybinds["CLOSE"] = this->m_supported_keys->at("ESCAPE");
	}
	//else

	std::string key;
	std::string key2;

	while (keys >> key >> key2)
	{
		this->m_keybinds[key] = m_supported_keys->at(key2);
	}

	keys.close();
}
