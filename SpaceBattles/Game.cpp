#include "Game.h"

Game::Game(const sf::Vector2f& window_size, const int fps)
{
	m_dt_clock.restart();

	m_window = new sf::RenderWindow(sf::VideoMode(window_size.x, window_size.y), "SpaceBattles", sf::Style::Close | sf::Style::Fullscreen);
	m_window->setFramerateLimit(fps);

	sf::Image icon;
	icon.loadFromFile(DEFAULT_ICON_PATH);

	m_window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


	m_default_view = &m_window->getDefaultView();

	this->init_keys();
	this->init_states();
}

Game::~Game()
{
	while (!m_states.empty())
	{
		delete m_states.top();

		m_states.pop();
	}
	delete m_window;
}

void Game::update()
{
	if (!m_states.empty())
	{
		m_states.top()->update(m_dt);

		if (m_states.top()->get_quit())
		{
			if (m_counter_a >= 0.8)
			{
				m_counter_a = 0.f;

				if (m_states.top()->end_state())
				{
					sf::String username = m_states.top()->getUsername();
					sf::String server_ip = m_states.top()->getServerIp();
					sf::String server_port = m_states.top()->getServerPort();

					State* state = m_states.top();
					m_states.pop();

					m_window->setView(*m_default_view);
					state->push_next_state(m_states);
					delete state;

					if (!m_states.empty())
					{
						m_states.top()->setUsername(username);
						m_states.top()->setServerIp(server_ip);
						m_states.top()->setServerPort(server_port);

						m_states.top()->init();
					}
				}
			}
		}
		else if (m_states.top()->get_game_quit())
		{
			if (m_states.top()->end_state())
				// end of the game
				while (!m_states.empty())
					m_states.pop();
		}
	}
	// app end
	else
	{
		m_window->close();
	}

	m_counter_a += m_dt;
}

void Game::update_dt()
{
	m_dt = m_dt_clock.restart().asSeconds();
}

void Game::run()
{
	while (m_window->isOpen())
	{
		sf::Event e;
		while (m_window->pollEvent(e))
		{
			if (!m_states.empty())
				m_states.top()->process_event(e);

			if (e.type == sf::Event::Closed)
				m_window->close();
		}

		this->update_dt();
		this->update();
		this->render();
	}
}

void Game::render()
{
	m_window->clear();

	if (!m_states.empty())
		m_states.top()->render(m_window);

	m_window->display();
}

void Game::init_states()
{
	//m_states.push(new GameState(m_window, &(this->m_supported_keys)));

	//m_window->setView(*m_default_view); // cause gamestate changes it
	m_states.push(new LobbyState(m_window, &(this->m_supported_keys)));

}

void Game::init_keys()
{
	std::ifstream keys("config/supported_keys.ini");
	if (!keys.is_open())
	{
		std::cout << "Cannot open the supported keys config! Setting the default keys\n";

		this->m_supported_keys["ESCAPE"] = sf::Keyboard::Key::Escape;
		this->m_supported_keys["A"] = sf::Keyboard::Key::A;
		this->m_supported_keys["D"] = sf::Keyboard::Key::D;
		this->m_supported_keys["W"] = sf::Keyboard::Key::W;
		this->m_supported_keys["S"] = sf::Keyboard::Key::S;
		this->m_supported_keys["F"] = sf::Keyboard::Key::F;
	}
	//else

	std::string key = "";
	int key_value = 0;

	while (keys >> key >> key_value)
	{
		this->m_supported_keys[key] = key_value;
	}

	keys.close();




#ifdef GAME_DEBUG

	for (auto i : m_supported_keys)
		std::cout << i.first << ' ' << i.second << std::endl;

#endif
}

/*
void Game::setState(State state)
{
	m_state = state;
}
*/