#pragma once

#include <SFML/Graphics.hpp>
#include "../Button.h"
#include "State.h"
#include "../TextInput.h"
#include "GameState.h";

#include <iostream>
#include <fstream>

class LobbyState : public State
{
public:
	LobbyState(sf::RenderWindow* w, std::map<std::string, int >* supported_keys);
	~LobbyState();

	virtual void push_next_state(std::stack<State*>& states) override;

	// update function (overrode)
	void update(const float& dt) override;

	void update_input(const float& dt);

	// udpate keybinds function (override)
	void update_keybinds(const float& dt) override;

	// render function (overrode)
	void render(sf::RenderTarget* target = nullptr) override;

	void process_event(sf::Event& ev) override;

	// end state function (overrode)
	bool end_state() override;

	void init() override;

private:
	// functions
	void init_keybinds();

	//variables

	// counters

	double m_counterA = 0; // exit button counter
	double m_counterB = 0; // start button counter
	double m_counterC = 0; // text fields counter

	sf::Texture* m_background_texture = nullptr;
	sf::RectangleShape* m_background = nullptr;

	TextInput m_username_input;
	TextInput m_server_ip_input;
	TextInput m_server_port_input;

	sf::Clock m_clock;
	sf::Font* m_font = nullptr;

	// closes the game
	Button* m_exit_button = nullptr;

	Button* m_connect_button = nullptr;

	Button* m_create_button = nullptr;

	// starts the game
	Button* m_start_button = nullptr;
};

