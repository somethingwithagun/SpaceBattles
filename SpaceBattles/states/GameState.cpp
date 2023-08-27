#include "GameState.h"
#include <Windows.h>
GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supported_keys) : State(window, supported_keys)
{
	this->init_textures();

	m_first_frame = true;

	m_rotation_angle = 0;

	m_view = new sf::View;
	m_view->setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	m_view->reset(sf::FloatRect(m_position.x, m_position.y, m_window->getSize().x, m_window->getSize().y));
	m_view->setCenter(m_position);

	m_gui_view = new sf::View;
	m_gui_view->setSize(static_cast<sf::Vector2f>(m_window->getSize()));
	m_gui_view->setCenter(m_gui_view->getSize().x / 2, m_gui_view->getSize().y / 2);
	m_gui_view->reset(sf::FloatRect(0, 0, m_window->getSize().x, m_window->getSize().y));

	m_counterA = 0.f;
	m_counterSocket = 0.f;
	m_exit_button_pressed = false;

	m_font = new sf::Font;
	m_font->loadFromFile("fonts/FiraCode-Medium.ttf");

	m_exit_button = new Button(sf::Vector2f(50, 50), "X", *m_font);
	m_exit_button->setPosition(sf::Vector2f(m_window->getSize().x - 20, 20));


	m_position = sf::Vector2f(0, 0);

	m_background_texture = new sf::Texture;
	m_background_texture->loadFromFile("images/space.png");

	//m_background_texture_size = sf::Vector2f(static_cast<sf::Vector2f>(m_window->getSize()));
	m_background_texture_size = sf::Vector2f(1000, 500);

	m_left_wall = new sf::RectangleShape(sf::Vector2f(100, m_world_size_y * 2));
	m_left_wall->setPosition(sf::Vector2f(-m_world_size_x, 0));
	m_left_wall->setOrigin(m_left_wall->getSize().x / 2, m_left_wall->getSize().y / 2);

	m_right_wall = new sf::RectangleShape(sf::Vector2f(100, m_world_size_y * 2));
	m_right_wall->setPosition(sf::Vector2f(m_world_size_x, 0));
	m_right_wall->setOrigin(m_right_wall->getSize().x / 2, m_right_wall->getSize().y / 2);

	m_top_wall = new sf::RectangleShape(sf::Vector2f(m_world_size_x * 2, 100));
	m_top_wall->setPosition(sf::Vector2f(0, -m_world_size_y + m_top_wall->getSize().y / 2));
	m_top_wall->setOrigin(m_top_wall->getSize().x / 2, m_top_wall->getSize().y / 2);

	m_bottom_wall = new sf::RectangleShape(sf::Vector2f(m_world_size_x * 2, 100));
	m_bottom_wall->setPosition(sf::Vector2f(0, m_world_size_y - m_bottom_wall->getSize().y / 2));
	m_bottom_wall->setOrigin(m_bottom_wall->getSize().x / 2, m_bottom_wall->getSize().y / 2);

	this->init_keybinds();
	this->init_map();

}

GameState::~GameState()
{
	for (int i = 0; i < m_meteors.size(); i++)
		delete m_meteors[i];

	delete m_background_texture;
	delete m_left_wall;
	delete m_right_wall;
	delete m_top_wall;
	delete m_bottom_wall;
	delete m_view;
	delete m_gui_view;
	delete m_player;
}

void GameState::rotateViewAt(sf::Vector2f coord, sf::View& view, float rotation)
{
	const sf::Vector2f offset{ coord - view.getCenter() };
	const float rotationInRadians{ rotation * 3.141592653f / 180.f };
	const float sine{ std::sin(rotationInRadians) }; // remember to "#include <cmath>" for this
	const float cosine{ std::cos(rotationInRadians) }; // remember to "#include <cmath>" for this
	const sf::Vector2f rotatedOffset{ cosine * offset.x - sine * offset.y, sine * offset.x + cosine * offset.y };

	view.rotate(rotation);

	view.move(offset - rotatedOffset);
}

void GameState::acceptSocket()
{
	if (listener->listen(std::stoi(m_server_port.toAnsiString())) != sf::Socket::Done)
	{
		std::cout << "couldn't listen\n";

		//		m_is_host = false;

		delete listener;
		listener = nullptr;
	}
	else
	{
		sf::TcpSocket* socket = new sf::TcpSocket;

		if (listener->accept(*socket) != sf::Socket::Done)
		{
			std::cout << "couldn't enstablish connection to the socket\n";
			delete socket;
		}
		else
		{
			m_connected = true;
			m_is_host = true;
			m_is_client = false;

			sf::Packet players_info;
			std::string players_info_str;

			players_info << (static_cast<int>(m_opposite_players.size()) + 1);

			// about host himself
			players_info << m_player->getName() << m_player->getPos().x << m_player->getPos().y << m_player->getRotation();

			for (int i = 0; i < m_opposite_players.size(); i++)
				players_info << m_opposite_players[i]->getName() << m_opposite_players[i]->getPos().x << m_opposite_players[i]->getPos().y << m_opposite_players[i]->getRotation();

			socket->send(players_info);

			// getting player name
			sf::Packet opp_player_data;

			socket->receive(opp_player_data);

			sf::String opp_name;
			opp_player_data >> opp_name;

			sf::Packet meteors_info;

			meteors_info << static_cast<int>(m_meteors.size());

			for (int i = 0; i < m_meteors.size(); i++)
				meteors_info << m_meteors[i]->getPos().x << m_meteors[i]->getPos().y << m_meteors[i]->getRotation()
				<< m_meteors[i]->getScale().x << m_meteors[i]->getScale().y << m_meteors[i]->get_direction()
				<< m_meteors[i]->getRotationSpeed();

			if (socket->send(meteors_info) != sf::Socket::Done)
				std::cout << "Couldn't send meteors info\n";



			std::cout << opp_name.toAnsiString() << " connected!\n";

			Player* new_player = new Player(opp_name, 100, sf::Vector2f(0, 0), m_bullets, m_textures["PLAYER_SHEET"]);

			sf::Packet new_player_info;
			sf::String newplmsg = "newpl";
			new_player_info << newplmsg << new_player->getName() << new_player->getPos().x << new_player->getPos().y << new_player->getRotation();

			for (int i = 0; i < m_opposite_players.size(); i++)
				if (m_sockets[m_opposite_players[i]]->send(new_player_info) != sf::Socket::Done)
					std::cerr << "Unexpected error!\n";


			m_sockets[new_player] = socket;
			m_opposite_players.push_back(new_player);

			acceptToSocket = std::async(std::launch::async, &GameState::acceptSocket, this);
			//		delete listener;
			//		listener = nullptr;
		}

	}

}

void GameState::init()
{
	srand(time(0));
	for (int i = 0; i < m_max_meteors; i++)
	{
		bool direction = rand() % 2; // if 1 then right, else left
		m_meteors.push_back(new Meteor(sf::Vector2f((rand() % m_world_size_x * 2) - m_world_size_x, (rand() % m_world_size_y * 2) - m_world_size_y), sf::Vector2f((5.f + (rand() % 10)) / 10.f, (5.f + (rand() % 10)) / 10.f),
			direction ? 1 : -1, (rand() % 180) + 90, m_textures["METEOR_SHEET"]));
	}

	if (m_username.getSize())
	{
		m_player = new Player(m_username, 100, sf::Vector2f(0, 0), m_bullets, m_textures["PLAYER_SHEET"]);
		sf::Vector2f position = static_cast<sf::Vector2f>(m_window->getSize());
		m_player->setPos(sf::Vector2f(0, 0));


		m_hp_bar.setFillColor(sf::Color::Red);
		m_hp_bar.setSize(sf::Vector2f(m_player->getHealth() * 5, 20));
		m_hp_bar.setPosition(sf::Vector2f(10, 10));

	}
	else
	{
		std::cerr << "Error in user input!";
		m_quit = true;
	}


	if (m_username.getSize() && m_server_ip.getSize() && m_server_port.getSize())
	{
		if (!m_connected)
		{
			if (!m_socket)
				m_socket = new sf::TcpSocket;
			//192.168.141.2
			sf::Socket::Status status = m_socket->connect(sf::IpAddress(m_server_ip), std::stoi(m_server_port.toAnsiString()));

			if (status != sf::Socket::Done)
			{
				std::cout << "couldn't connect\n";
				m_connected = false;
			}
			else
			{
				m_connected = true;
				m_is_client = true;
				m_is_host = false;

				std::cout << "connected as client\n";

				sf::Packet players_info;
				m_socket->receive(players_info);

				int players_amount;
				players_info >> players_amount;

				for (int i = 0; i < players_amount; i++)
				{
					sf::String name;
					float x, y, rotation;
					players_info >> name >> x >> y >> rotation;

					std::cout << name.toAnsiString() << std::endl;

					m_opposite_players.push_back(new Player(name, 100, sf::Vector2f(x, y), m_bullets, m_textures["PLAYER_SHEET"]));
					m_opposite_players[m_opposite_players.size() - 1]->setRotation(rotation);
				}

				// sending current player name
				sf::Packet name;
				name << m_player->getName();

				m_socket->send(name);

				sf::Packet meteors_info;
				m_socket->receive(meteors_info);

				int meteors_amount;
				meteors_info >> meteors_amount;

				while (m_meteors.size() != 0)
				{
					delete m_meteors[m_meteors.size() - 1];
					m_meteors.pop_back();
				}

				for (int i = 0; i < meteors_amount; i++)
				{
					int dir;
					float x, y, rotation, scale_x, scale_y, rotation_speed;
					meteors_info >> x >> y >> rotation >> scale_x >> scale_y >> dir >> rotation_speed;

					m_meteors.push_back(new Meteor(sf::Vector2f(x, y), sf::Vector2f(scale_x, scale_y), dir, rotation_speed, m_textures["METEOR_SHEET"]));
					m_meteors[m_meteors.size() - 1]->setRotation(rotation);
				}

				while (m_bullets.size() != 0)
				{
					delete m_bullets[m_bullets.size() - 1];
					m_bullets.pop_back();
				}
			}
		}

	}
	else if (m_username.getSize() && m_server_port.getSize())
	{
		if (!m_is_client)
		{
			if (!listener)
			{
				listener = new sf::TcpListener;
			}
			if (!m_socket)
				m_socket = new sf::TcpSocket;

			acceptToSocket = std::async(std::launch::async, &GameState::acceptSocket, this);
		}
	}


}

void GameState::update(const float& dt)
{

	bool is_game_over_dialog_opened = false;

	if (m_game_over_dialog && m_game_over_dialog->get_is_open())
	{
		is_game_over_dialog_opened = true;

		m_game_over_dialog->update(dt);

		if (m_game_over_dialog->get_is_open() == false)
			switch (m_game_over_dialog->get_result())
			{
			case GameOverScreen::Accepted:
			{
				respawn_player_in_random_place();
				delete m_game_over_dialog;
				m_game_over_dialog = nullptr;
				break;
			}
			case GameOverScreen::Rejected:
			{
				m_quit = true;
				delete m_game_over_dialog;
				m_game_over_dialog = nullptr;
				break;
			}
			case GameOverScreen::Closed:
			{
				m_quit = true;
				delete m_game_over_dialog;
				m_game_over_dialog = nullptr;
				break;
			}
			default:
				std::cout << "Unhandled result\n";
				delete m_game_over_dialog;
				m_game_over_dialog = nullptr;
				break;
			}

	}

	if (!is_game_over_dialog_opened)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());

			if (m_exit_button->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
			{
				if (m_counterA >= 0.006)
				{
					m_exit_button->setPressed(true);
					m_counterA = 0;

					m_game_quit = true;

					m_exit_button_pressed = true;
				}
			}
		}

		//m_prev_bullets_amount = m_bullets.size();

		m_exit_button->update(dt);

		update_keybinds(dt);

		if (!(m_info_dialog && m_info_dialog->get_is_open()))
			update_input(dt);

		update_collisions(dt);

		if (m_info_dialog && m_info_dialog->get_is_open())
			m_info_dialog->update(dt);

		for (int i = 0; i < m_meteors.size(); i++)
		{
			m_meteors[i]->rotate(m_meteors[i]->get_direction(), dt);
			m_meteors[i]->update(dt);
		}

		m_player->update(dt);

		for (int i = 0; i < m_opposite_players.size(); i++)
			m_opposite_players[i]->update(dt);

		m_hp_bar.setSize(sf::Vector2f(m_player->getHealth() * 5, 20));

		// avoid the view rotation reset
		m_rotation_angle = m_view->getRotation();
		m_view->reset(sf::FloatRect(m_position.x, m_position.y, m_window->getSize().x, m_window->getSize().y));
		m_view->setRotation(m_rotation_angle);

		update_view_borders(dt);

		m_view->setCenter(m_position);

		if (m_first_frame)
		{
			m_window->setView(*m_view);
			m_first_frame = false;
		}
	}

	/* *** sending data *** */


	// clients send only their data
	if (m_is_client)
	{
		sf::Packet sendpkg;
		sf::String msg = "plrposrot"; // player position & rotation
		sendpkg << msg << m_player->getPos().x << m_player->getPos().y << m_player->getRotation();

		if (m_socket->send(sendpkg) != sf::Socket::Done)
			std::cout << "Couldn't send\n";
	}

	// meanwhile the host sends info abt himself and then sends all the clients data about all the things
	if (m_is_host)
	{
		//sf::Packet bullets_info;
		//bullets_info << m_bullets.size();
		//for (int i = 0; i < m_bullets.size(); i++)
		//	bullets_info << m_bullets[i]->getPosition().x << m_bullets[i]->getPosition().y << m_bullets[i]->getRotation() << m_bullets[i]->getOrigin().x << m_bullets[i]->getOrigin().y << m_bullets[i]->getSpeed() << m_bullets[i]->get_damage();


		for (int i = 0; i < m_opposite_players.size(); i++)
		{
			sf::Packet player_data;
			player_data << m_player->getName() << m_player->getPos().x << m_player->getPos().y << m_player->getRotation();

			//auto res = m_sockets[m_opposite_players[i]]->getRemoteAddress();

			sf::Socket::Status status = m_sockets[m_opposite_players[i]]->send(player_data);

			if (status == sf::Socket::Disconnected)
			{
				std::cout << m_opposite_players[i]->getName().toAnsiString() << " disconnected!\n";

				delete m_sockets[m_opposite_players[i]];

				delete m_opposite_players[i];
				m_opposite_players[i] = nullptr;

				std::swap(m_opposite_players[i], m_opposite_players[m_opposite_players.size() - 1]);

				m_opposite_players.pop_back();

				i--;
				break;
			}



			if (status != sf::Socket::Done)
				std::cout << "Couldn't send\n";



			sf::Packet sendpkg;
			sendpkg << m_opposite_players[i]->getName() << m_opposite_players[i]->getPos().x << m_opposite_players[i]->getPos().y << m_opposite_players[i]->getRotation();
			// send to every client information about single player
			for (int j = 0; j < m_opposite_players.size(); j++)
			{
				if (m_sockets[m_opposite_players[i]] != m_sockets[m_opposite_players[j]])
					if (m_sockets[m_opposite_players[j]]->send(sendpkg) != sf::Socket::Done)
						std::cout << "Couldn't send\n";
			}

			//if (m_sockets_map[m_players[i]]->send(bullets_info) != sf::Socket::Done)
			//	std::cout << "Couldn't send\n";

		}
	}

	/* *** receiving data *** */

	// clients receive data sent by host
	if (m_is_client)
	{
		for (int i = 0; i < m_opposite_players.size(); i++)
		{
			sf::Packet recv;

			if (m_socket->receive(recv) == sf::Socket::Done)
			{
				sf::String name;
				float x, y, rotation;
				recv >> name;

				if (name == "newpl") {
					recv >> name >> x >> y >> rotation;
					m_opposite_players.push_back(new Player(name, 100, sf::Vector2f(x, y), m_bullets, m_textures["PLAYER_SHEET"]));
					m_opposite_players[m_opposite_players.size() - 1]->setRotation(rotation);
					continue;
				}
				else if (name == "mtrdstr") {
					recv >> x >> y;
					sf::Vector2f destroyed_meteor_pos(x, y);

					for (int j = 0; j < m_meteors.size(); j++) {
						if (m_meteors[j]->getPos() == destroyed_meteor_pos)
						{
							//delete meteor
							delete m_meteors[j];
							m_meteors[j] = nullptr;

							std::swap(m_meteors[j], m_meteors[m_meteors.size() - 1]);

							m_meteors.pop_back();

							break;
						}
					}

					//std::cout << "Meteor not found (client)" << std::endl;

				}
				else if (name == "bltapprd")
				{
					float x, y, rotation, origin_x, origin_y, speed;
					int dmg;
					sf::String owner;
					recv >> owner >> x >> y >> rotation >> origin_x >> origin_y >> speed >> dmg;

					m_bullets.push_back(new Bullet(owner, rotation, -120, -120, sf::Vector2f(x, y), sf::Vector2f(origin_x, origin_y), speed, dmg, m_player->get_weapon_handle()->get_bullet_texture()));
				}
				else if (name == "plrhit")
				{
					sf::String name;
					sf::String bltowner;
					int dmg;
					recv >> name >> bltowner >> dmg;

					int player_index = 0;
					if (m_player->getName() != bltowner)
					{
						if (m_player->getName() == name)
						{
							m_player->decrease_health(dmg);

							for (int j = 0; j < m_bullets.size(); j++)
								if (m_player->getGlobalBounds().intersects(m_bullets[j]->getGlobalBounds()))
								{
									delete m_bullets[j];
									m_bullets[j] = nullptr;

									std::swap(m_bullets[j], m_bullets[m_bullets.size() - 1]);
									m_bullets.pop_back();

									j--;
								}
						}
						else
						{
							while (player_index < m_opposite_players.size() - 1 && m_opposite_players[player_index]->getName() != name)
								player_index++;

							//m_opposite_players[player_index]->decrease_health(dmg);

							for (int j = 0; j < m_bullets.size(); j++)
								if (m_opposite_players[player_index]->getGlobalBounds().intersects(m_bullets[j]->getGlobalBounds()))
								{
									delete m_bullets[j];
									m_bullets[j] = nullptr;

									std::swap(m_bullets[j], m_bullets[m_bullets.size() - 1]);
									m_bullets.pop_back();

									j--;
								}
						}
					}
				}
				else
				{
					recv >> x >> y >> rotation;

					int j = 0;

					for (j = 0; j < m_opposite_players.size() - 1; j++)
						if (m_opposite_players[j]->getName() == name)
							break;

					if (m_opposite_players[j]->getName() == name)
					{
						m_opposite_players[j]->setPos(sf::Vector2f(x, y));
						m_opposite_players[j]->setRotation(rotation);
					}
					else std::cout << "Error: unexisting player\n";
				}
			}
			else std::cout << "Couldn't receive\n";
		}
	}

	// host receives the data sent by client
	if (m_is_host)
	{
		for (int i = 0; i < m_opposite_players.size(); i++)
		{
			sf::Packet recv;

			if (m_sockets[m_opposite_players[i]]->receive(recv) == sf::Socket::Done)
			{
				float x, y, rotation;
				sf::String msg;

				recv >> msg;

				if (msg == "mtrdstr") {
					recv >> x >> y;
					sf::Vector2f destroyed_meteor_pos(x, y);

					for (int j = 0; j < m_meteors.size(); j++) {
						if (m_meteors[j]->getPos() == destroyed_meteor_pos)
						{
							//delete meteor
							delete m_meteors[j];
							m_meteors[j] = nullptr;

							std::swap(m_meteors[j], m_meteors[m_meteors.size() - 1]);

							m_meteors.pop_back();

							break;
						}
					}
					//	std::cout << "Meteor not found" << std::endl;
				}
				else if (msg == "bltapprd")
				{
					float x, y, rotation, origin_x, origin_y, speed;
					int dmg;
					sf::String owner;
					recv >> owner >> x >> y >> rotation >> origin_x >> origin_y >> speed >> dmg;

					m_bullets.push_back(new Bullet(owner, rotation, -120, -120, sf::Vector2f(x, y), sf::Vector2f(origin_x, origin_y), speed, dmg, m_player->get_weapon_handle()->get_bullet_texture()));
				}
				else if (msg == "plrhit")
				{
					sf::String name;
					sf::String bltowner;
					int dmg;
					recv >> name >> bltowner >> dmg;

					int player_index = 0;

					if (m_player->getName() != bltowner)
					{
						if (m_player->getName() == name)
						{
							m_player->decrease_health(dmg);

							for (int j = 0; j < m_bullets.size(); j++)
								if (m_player->getGlobalBounds().intersects(m_bullets[j]->getGlobalBounds()))
								{
									delete m_bullets[j];
									m_bullets[j] = nullptr;

									std::swap(m_bullets[j], m_bullets[m_bullets.size() - 1]);
									m_bullets.pop_back();

									j--;
								}
						}
						else
						{
							while (player_index < m_opposite_players.size() - 1 && m_opposite_players[player_index]->getName() != name)
								player_index++;

							//m_opposite_players[player_index]->decrease_health(dmg);

							for (int j = 0; j < m_bullets.size(); j++)
								if (m_opposite_players[player_index]->getGlobalBounds().intersects(m_bullets[j]->getGlobalBounds()))
								{
									delete m_bullets[j];
									m_bullets[j] = nullptr;

									std::swap(m_bullets[j], m_bullets[m_bullets.size() - 1]);
									m_bullets.pop_back();

									j--;
								}
						}
					}

				}
				else
				{
					recv >> x >> y >> rotation;

					m_opposite_players[i]->setPos(sf::Vector2f(x, y));
					m_opposite_players[i]->setRotation(rotation);
				}
			}
			else std::cout << "Couldn't receive\n";
		}
	}
	m_view->setRotation(m_player->getRotation());

	m_counterA += dt;
	m_counterB += dt;
	m_counterSocket += dt;
}

void GameState::update_input(const float& dt)
{
	if (m_window->hasFocus())
	{
		this->check_for_quit();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("TURN_LEFT"))))
			m_player->rotate(-1.f, dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("TURN_RIGHT"))))
			m_player->rotate(1.f, dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("MOVE_UP"))))
		{
			m_player->move(-1.f, -1.f, dt);
			m_player->set_state(Player::PlayerState::Moving);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("MOVE_DOWN"))))
		{
			m_player->move(1.f, 1.f, dt);
			m_player->set_state(Player::PlayerState::Moving);
		}

		if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("MOVE_DOWN"))))
			&& !(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("MOVE_UP")))))
			m_player->set_state(Player::PlayerState::Idle);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(m_keybinds.at("SHOOT"))))
		{
			if (m_player->shoot() && m_connected)
			{
				sf::Packet bullet_info;

				//float x, y, rotation, origin_x, origin_y, speed;
				//int dmg;
				sf::String msg = "bltapprd";
				bullet_info << msg << m_bullets[m_bullets.size() - 1] << m_bullets[m_bullets.size() - 1]->getPosition().x << m_bullets[m_bullets.size() - 1]->getPosition().y << m_bullets[m_bullets.size() - 1]->getRotation() << m_bullets[m_bullets.size() - 1]->getOrigin().x << m_bullets[m_bullets.size() - 1]->getOrigin().y << m_bullets[m_bullets.size() - 1]->getSpeed() << m_bullets[m_bullets.size() - 1]->get_damage();

				if (m_is_client) {
					if (m_socket->send(bullet_info) != sf::Socket::Done)
						std::cout << "Error couldn't send bullet info" << std::endl;
				}
				else if (m_is_host) {
					for (int i = 0; i < m_opposite_players.size(); i++)
					{
						if (m_sockets[m_opposite_players[i]]->send(bullet_info) != sf::Socket::Done)
							std::cout << "Error couldn't send bullet info" << std::endl;
					}
				}
			}

			//m_bullets.push_back(new Bullet(rotation, -120, -120, sf::Vector2f(0, 0), sf::Vector2f(origin_x, origin_y), speed, dmg, pl->get_weapon_handle()->get_bullet_texture()));
		}
	}
	m_last_move_dt = dt;

	this->m_player->update(dt);

	if (m_player->getHealth() <= 0)
	{
		m_player->setHealth(0);
		open_game_over_dialog();
		m_player->setPos(sf::Vector2f(m_world_size_x + 5000, m_world_size_y + 5000));
	}

	for (int i = 0; i < m_opposite_players.size(); i++)
		m_opposite_players[i]->update(dt);

	//listener->listen(std::stoi(m_server_port.toAnsiString()));

}

void GameState::update_collisions(const float& dt)
{
	// also for bullets
	for (int i = 0; i < m_bullets.size(); i++)
	{
		if (m_bullets[i])
			if (
				m_bullets[i]->getGlobalBounds().intersects(m_left_wall->getGlobalBounds())
				|| m_bullets[i]->getGlobalBounds().intersects(m_right_wall->getGlobalBounds())
				|| m_bullets[i]->getGlobalBounds().intersects(m_top_wall->getGlobalBounds())
				|| m_bullets[i]->getGlobalBounds().intersects(m_bottom_wall->getGlobalBounds())

				)
			{
				delete m_bullets[i];
				m_bullets[i] = nullptr;

				std::swap(m_bullets[i], m_bullets[m_bullets.size() - 1]);

				m_bullets.pop_back();
				i--;
			}
			else
			{
				for (int j = 0; j < m_meteors.size(); j++)
				{
					if (m_bullets[i]->getGlobalBounds().intersects(m_meteors[j]->getGlobalBounds()))
					{
						m_meteors[j]->decrease_integrity(m_bullets[i]->get_damage());

						std::cout << m_meteors[j]->getIntegrity() << std::endl;

						if (m_meteors[j]->getIntegrity() <= 0)
						{

							if (m_connected)
							{
								sf::Packet destroyed_meteor_info;
								sf::String destr_meteor_msg = "mtrdstr";
								destroyed_meteor_info << destr_meteor_msg << m_meteors[j]->getPos().x << m_meteors[j]->getPos().y;

								if (m_is_client) {
									// send info to the host
									if (m_socket->send(destroyed_meteor_info) != sf::Socket::Done)
										std::cout << "ERROR Couldn't send meteor info" << std::endl;
								}
								else if (m_is_host) {
									// send info to all the clients

									for (int c = 0; c < m_opposite_players.size(); c++)
										if (m_sockets[m_opposite_players[c]]->send(destroyed_meteor_info) != sf::Socket::Done)
											std::cout << "ERROR Couldn't send meteor info" << std::endl;
								}
							}
							//delete meteor
							delete m_meteors[j];
							m_meteors[j] = nullptr;
							std::swap(m_meteors[j], m_meteors[m_meteors.size() - 1]);

							m_meteors.pop_back();
							j--;

							//break;
						}

						delete m_bullets[i];
						m_bullets[i] = nullptr;

						std::swap(m_bullets[i], m_bullets[m_bullets.size() - 1]);

						m_bullets.pop_back();
						//i--;
						break;
					}
				}
				if (m_bullets.size() > 0)
				{

					for (int j = 0; j < m_opposite_players.size(); j++)
					{
						if (m_opposite_players[j]->getGlobalBounds().intersects(m_bullets[i]->getGlobalBounds()))
						{
							if (!m_connected)
								m_opposite_players[j]->decrease_health(m_bullets[i]->get_damage());
							else
							{
								sf::Packet player_hit_info;
								sf::String player_hit_msg = "plrhit";
								sf::String player_hit_name = m_opposite_players[j]->getName();
								player_hit_info << player_hit_msg << player_hit_name << m_bullets[j]->getOwner() << m_bullets[i]->get_damage();

								if (m_is_client) {
									// send info to the host
									if (m_socket->send(player_hit_info) != sf::Socket::Done)
										std::cout << "ERROR Couldn't send hit info" << std::endl;
								}
								else if (m_is_host) {
									// send info to all the clients

									for (int c = 0; c < m_opposite_players.size(); c++)
										if (m_sockets[m_opposite_players[c]]->send(player_hit_info) != sf::Socket::Done)
											std::cout << "ERROR Couldn't send hit info" << std::endl;
								}
							}

							delete m_bullets[i];
							m_bullets[i] = nullptr;

							std::swap(m_bullets[i], m_bullets[m_bullets.size() - 1]);

							m_bullets.pop_back();
							i--;
							break;
						}
					}
				}
			}
	}


	for (int i = 0; i < m_meteors.size(); i++)
	{
		if (m_player->getCollider().checkCollision(m_meteors[i]->getGlobalBounds()))
		{
			if (m_counterB >= 1)
			{
				m_meteors[i]->decrease_integrity(m_player->getHealth() * 0.5);

				if (m_meteors[i]->getIntegrity() < 0)
					m_meteors[i]->setIntegrity(0);

				m_player->decrease_health(m_meteors[i]->getIntegrity() * 0.2);



				if (m_meteors[i]->getIntegrity() <= 0)
				{
					if (m_connected)
					{
						sf::Packet destroyed_meteor_info;
						sf::String destr_meteor_msg = "mtrdstr";
						destroyed_meteor_info << destr_meteor_msg << m_meteors[i]->getPos().x << m_meteors[i]->getPos().y;

						if (m_is_client) {
							// send info to the host
							if (m_socket->send(destroyed_meteor_info) != sf::Socket::Done)
								std::cout << "ERROR Couldn't send meteor info" << std::endl;
						}
						else if (m_is_host) {
							// send info to all the clients

							for (int c = 0; c < m_opposite_players.size(); c++)
								if (m_sockets[m_opposite_players[c]]->send(destroyed_meteor_info) != sf::Socket::Done)
									std::cout << "ERROR Couldn't send meteor info" << std::endl;
						}
					}

					//delete meteor
					delete m_meteors[i];
					m_meteors[i] = nullptr;

					std::swap(m_meteors[i], m_meteors[m_meteors.size() - 1]);
					m_meteors.pop_back();
					i--;
				}

				m_counterB = 0;
				break;
			}


			if (m_player->getRotation() < 180.f)
				m_player->move(12.f, 0.f, dt);
			else
				m_player->move(-12.f, 0.f, dt);

		}
	}
	// left wall
	if (m_player->getCollider().checkCollision(m_left_wall->getGlobalBounds()))
		if (m_player->getRotation() >= 180.f)
			m_player->move(-m_player->get_movement_component_ptr()->getVelocity().x, 0, dt);
		else
			m_player->move(m_player->get_movement_component_ptr()->getVelocity().x, 0, dt);


	// right wall
	if (m_player->getCollider().checkCollision(m_right_wall->getGlobalBounds()))
		if (m_player->getRotation() < 180.f)
			m_player->move(m_player->get_movement_component_ptr()->getVelocity().x, 0, dt);
		else
			m_player->move(-m_player->get_movement_component_ptr()->getVelocity().x, 0, dt);

	// top wall
	if (m_player->getCollider().checkCollision(m_top_wall->getGlobalBounds()))
		if (m_player->getRotation() <= 270 && m_player->getRotation() >= 90)
			m_player->move(0, m_player->get_movement_component_ptr()->getVelocity().y, dt);
		else
			m_player->move(0, -m_player->get_movement_component_ptr()->getVelocity().y, dt);

	// bottom wall
	if (m_player->getCollider().checkCollision(m_bottom_wall->getGlobalBounds()))
		if (m_player->getRotation() > 270 || m_player->getRotation() < 90)
			m_player->move(0, -m_player->get_movement_component_ptr()->getVelocity().y, dt);
		else
			m_player->move(0, m_player->get_movement_component_ptr()->getVelocity().y, dt);
}

void GameState::update_view_borders(const float& dt)
{
	// X
	if (m_player->getCollider().getPos().x < 0)
		// controls left border
		if (m_player->getCollider().getPos().x > -m_world_size_x + m_view->getSize().x / 4)
			m_position.x = m_player->getCollider().getPos().x;
		else m_position.x = -m_world_size_x + m_view->getSize().x / 4;
	else
		// controls right
		if (m_player->getCollider().getPos().x < m_world_size_x - m_view->getSize().x / 4)
			m_position.x = m_player->getCollider().getPos().x;
		else m_position.x = m_world_size_x - m_view->getSize().x / 4;


	// Y
	if (m_player->getCollider().getPos().y < 0)
		// controls top border
		if (m_player->getCollider().getPos().y > -m_world_size_y + m_view->getSize().y / 4)
			m_position.y = m_player->getCollider().getPos().y;
		else m_position.y = -m_world_size_y + m_view->getSize().y / 4;
	else
		// controls bottom border
		if (m_player->getCollider().getPos().y < m_world_size_y - m_view->getSize().y / 4)
			m_position.y = m_player->getCollider().getPos().y;
		else m_position.y = m_world_size_y - m_view->getSize().y / 4;

}

void GameState::render(sf::RenderTarget* target)
{
	for (int i = 0; i < m_map_background.size(); i++)
		for (int j = 0; j < m_map_background[i].size(); j++)
			target->draw(m_map_background[i][j]);

	target->draw(*m_left_wall);
	target->draw(*m_right_wall);
	target->draw(*m_top_wall);
	target->draw(*m_bottom_wall);

	bool is_game_over_dialog_opened = false;

	if (m_game_over_dialog)
	{
		if (m_game_over_dialog->get_is_open())
		{
			is_game_over_dialog_opened = true;
		}
	}

	if (!is_game_over_dialog_opened)
	{
		for (int i = 0; i < m_meteors.size(); i++)
			m_meteors[i]->render(target);

		m_player->render(target);

		for (int i = 0; i < m_opposite_players.size(); i++)
			m_opposite_players[i]->render(target);

	}

	render_gui(target);
}

void GameState::render_gui(sf::RenderTarget* target)
{
	bool is_game_over_dialog_opened = false;
	// set the gui view to render the gui in fixed position
	target->setView(*m_gui_view);

	if (m_game_over_dialog && m_game_over_dialog->get_is_open())
	{
		is_game_over_dialog_opened = true;
		m_game_over_dialog->render(target);
	}

	if (!is_game_over_dialog_opened)
	{
		target->draw(*m_exit_button);
		target->draw(m_hp_bar);
	}

	if (m_info_dialog && m_info_dialog->get_is_open())
		m_info_dialog->render(m_window);

	// then set the main view
	target->setView(*m_view);
}

void GameState::process_event(sf::Event& ev)
{
}

void GameState::init_keybinds()
{
	std::ifstream keys("config/gamestate_keybinds.ini");

	if (!keys.is_open())
	{
		std::cout << "Cannot open the gamestate keybinds config! Setting the default keybinds\n";

		m_keybinds["CLOSE"] = this->m_supported_keys->at("ESCAPE");

		m_keybinds["TURN_LEFT"] = this->m_supported_keys->at("A");

		m_keybinds["TURN_RIGHT"] = this->m_supported_keys->at("D");

		m_keybinds["MOVE_UP"] = this->m_supported_keys->at("W");

		m_keybinds["MOVE_DOWN"] = this->m_supported_keys->at("S");

		m_keybinds["SHOOT"] = this->m_supported_keys->at("F");
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

void GameState::init_map()
{
	int background_repeat_x = m_world_size_x * 2 / m_background_texture_size.x;
	int background_repeat_y = m_world_size_y * 2 / m_background_texture_size.y;

	m_map_background.resize(background_repeat_y);

	float posx = -m_world_size_x;
	float posy = -m_world_size_y;

	for (int i = 0; i < background_repeat_y; i++)
	{
		posx = -m_world_size_x;
		for (int j = 0; j < background_repeat_x; j++)
		{
			m_map_background[i].push_back(sf::RectangleShape(m_background_texture_size));
			m_map_background[i][j].setPosition(posx, posy);
			m_map_background[i][j].setTexture(m_background_texture);

			posx += m_map_background[i][j].getSize().x;

		}
		posy += m_background_texture_size.y;
	}
}

void GameState::init_textures()
{
	if (!m_textures["PLAYER_SHEET"].loadFromFile("images/spaceship_sheet.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}
	if (!m_textures["METEOR_SHEET"].loadFromFile("images/meteor_sheet.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_METEOR_TEXTURE";
	}
}

void GameState::open_game_over_dialog()
{
	m_game_over_dialog = new GameOverScreen;
	m_game_over_dialog->setPosition(sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2));
	m_game_over_dialog->show();
}

void GameState::respawn_player_in_random_place()
{
	m_player->setHealth(100);
	std::srand(std::time(0));

	float x, y, rotation;
	x = (rand() % m_world_size_x * 2) - m_world_size_x;
	y = (rand() % m_world_size_y * 2) - m_world_size_y;

	rotation = (rand() % 360 * 2) - 360;

	m_player->setPos(sf::Vector2f(x, y));
	m_player->setRotation(rotation);
}

void GameState::update_keybinds(const float& dt)
{
	check_for_quit();
}

bool GameState::end_state()
{
	if (m_quit)
	{
		if (m_is_client && m_socket)
			m_socket->disconnect();

		if (listener)
			delete listener;

		std::cout << "exit to lobby\n";

		return true;
	}

	if (!m_info_dialog)
	{
		m_info_dialog = new InfoDialog("Are you sure you want to leave?", GOS_DEFAULT_BACKGROUND_SIZE, GOS_DEFAULT_BUTTON_SIZE, GOS_DEFAULT_BUTTON_COLOR);
		m_info_dialog->setPosition(sf::Vector2f(m_window->getSize().x / 2, m_window->getSize().y / 2));
		m_info_dialog->show();
	}

	if (m_info_dialog && !m_info_dialog->get_is_open())
		if (m_info_dialog->get_result() == InfoDialog::Accepted)
		{
			if (m_is_client && m_socket)
				m_socket->disconnect();

			if (listener)
				delete listener;

			std::cout << "exit\n";

			delete m_info_dialog;
			m_info_dialog = nullptr;

			return true;
		}
		else
		{
			m_game_quit = false;

			delete m_info_dialog;
			m_info_dialog = nullptr;
		}


	//m_quit = false;
	return false;

}

void GameState::push_next_state(std::stack<State*>& states)
{
	states.push(new LobbyState(m_window, m_supported_keys));
}
