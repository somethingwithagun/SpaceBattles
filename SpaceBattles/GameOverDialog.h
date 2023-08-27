#pragma once

#include "DialogBase.h"

#define GOS_DEFAULT_BACKGROUND_SIZE sf::Vector2f(700,500)
#define GOS_DEFAULT_BUTTON_SIZE sf::Vector2f(200,50)
#define GOS_DEFAULT_BUTTON_COLOR sf::Color(189, 186, 177)


class GameOverScreen : public DialogBase
{

public:
	GameOverScreen();
	~GameOverScreen();

	virtual void update(const float& dt) override;
	virtual void render(sf::RenderTarget* target) override;

};

