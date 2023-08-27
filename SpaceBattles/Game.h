#pragma once

#define GAME_DEBUG

#include <fstream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "player.h"
#include "states/GameState.h"
#include "States/LobbyState.h"
#include "states/State.h"

/*enum State {
	Init,
	Menu,
	Active,
	Respawn,
	Exit
};*/

class Game
{
public:
	Game(const sf::Vector2f& window_size, const int fps);
	~Game();

	/////////////////////
	////   UPDATE   ////
	///////////////////

	// update function - function where the all logics happen every frame
	void update();

	// update dt function - function to update the frame drawing speed
	void update_dt();



	///////////////////
	////  RENDER   ////
	///////////////////

	// run function
	void run();

	// render function - renders the objects and all the graphics
	void render();

	// inits all the states
	void init_states();

	void init_keys();

	//getters
	//void setState(State state);

private:
	const sf::View* m_default_view = nullptr;

	sf::Clock m_dt_clock;

	std::map<std::string, int> m_supported_keys;

	float m_dt;

	//counters
	float m_counter_a; // exit counter 

	std::stack<State*> m_states;
	//State m_state = State::Init;
	sf::RenderWindow* m_window = nullptr;
};

