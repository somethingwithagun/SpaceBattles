#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	Game game(sf::Vector2f(1300, 1000), 30);

	game.run();
}