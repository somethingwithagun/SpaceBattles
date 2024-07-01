#include "OnlineHost.h"

OnlineHost::OnlineHost(const sf::Uint16& port) : Online(port)
{
	m_listener = new sf::TcpListener;
}

OnlineHost::~OnlineHost()
{
}

void OnlineHost::acceptSocket()
{
	if (m_listener->listen(m_port) != sf::Socket::Done) {
		std::cout << "couldn't listen\n";

		//		m_is_host = false;

		//delete listener;
		//listener = nullptr;
	}
	else
	{
		sf::TcpSocket* socket = new sf::TcpSocket;

		if (m_listener->accept(*socket) != sf::Socket::Done)
		{
			std::cout << "couldn't enstablish connection to the socket\n";
			delete socket;
		}
		else
		{
			m_connected = true;

			/*sf::Packet players_info;
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
		*/
		}
	}
}
