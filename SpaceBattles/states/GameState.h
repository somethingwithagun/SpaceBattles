#pragma once

#include "State.h"

#include "../player.h"
#include "../Button.h"
#include "../TwoBombs.h"
#include "../Meteor.h"
#include "../GameOverDialog.h"
#include "../InfoDialog.h"
#include "LobbyState.h"

#include <SFML/Network.hpp>
#include <thread>
#include <iostream>
#include <future>
#include <fstream>

#define debug(X) std::cout <<std::endl<< X << std::endl<<std::endl;

class GameState : public State
{
public:
	explicit GameState(sf::RenderWindow* window, std::map<std::string, int>* supported_keys);
	virtual ~GameState();


	void update_keybinds(const float& dt) override;

	bool end_state() override;

	virtual void push_next_state(std::stack<State*>& states) override;

	// update function (overrode)
	void update(const float& dt) override;
	void update_input(const float& dt);
	void update_collisions(const float& dt);
	void update_view_borders(const float& dt);

	// render function (overrode)
	void render(sf::RenderTarget* target = nullptr) override;
	void render_gui(sf::RenderTarget* target = nullptr);

	void process_event(sf::Event& ev) override;

	void rotateViewAt(sf::Vector2f coord, sf::View& view, float rotation);

	void acceptSocket();

	void init() override;

private:
	// funtions
	void init_keybinds();
	void init_map();
	void init_textures();

	void open_game_over_dialog();
	void respawn_player_in_random_place();

	// variables
	std::future<void> acceptToSocket;

	sf::TcpListener* listener = nullptr;
	sf::TcpSocket* m_socket = nullptr;

	//const char* m_address = "localhost";
	//const unsigned short m_port = 5555;

	bool m_is_host = false;
	bool m_connected = false;
	bool m_is_client = false;

	bool m_first_frame = true;

	const int m_world_size_x = 2000;
	const int m_world_size_y = 2000;

	float m_rotation_angle;

	float m_last_move_dt;

	sf::Vector2f m_position;

	//counters
	float m_counterA; // exit button counter
	float m_counterB; // player meteor collision counter
	float m_counterSocket; // socket key counter

	sf::View* m_view = nullptr;
	sf::View* m_gui_view = nullptr;

	Player* m_player = nullptr;

	std::vector<Player*> m_opposite_players;

	std::map <Player*, sf::TcpSocket*> m_sockets;

	sf::Font* m_font = nullptr;

	// closes the game
	Button* m_exit_button = nullptr;

	GameOverScreen* m_game_over_dialog = nullptr;
	InfoDialog* m_info_dialog = nullptr;

	std::size_t m_prev_bullets_amount;

	// Hpbar
	sf::RectangleShape m_hp_bar;

	bool m_exit_button_pressed;

	std::vector<std::vector<sf::RectangleShape>> m_map_background;
	std::vector<Bullet*> m_bullets;	 // we need this to process the collisions
	std::vector<Meteor*> m_meteors;

	const int m_max_meteors = 10;

	sf::Texture* m_background_texture = nullptr;
	sf::Vector2f m_background_texture_size;

	sf::RectangleShape* m_left_wall = nullptr; // these are just a bunch of meteors so the player cannot go through them ;)
	sf::RectangleShape* m_right_wall = nullptr; // these are just a bunch of meteors so the player cannot go through them ;)
	sf::RectangleShape* m_top_wall = nullptr; // these are just a bunch of meteors so the player cannot go through them ;)
	sf::RectangleShape* m_bottom_wall = nullptr; // these are just a bunch of meteors so the player cannot go through them ;)

};

