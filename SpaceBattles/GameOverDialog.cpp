#include "GameOverDialog.h"

GameOverScreen::GameOverScreen() : DialogBase(GOS_DEFAULT_BACKGROUND_SIZE, GOS_DEFAULT_BUTTON_SIZE, GOS_DEFAULT_BUTTON_COLOR)
{
	setText("You died. Nihil desperandum.\n Would you like to revive?");
	m_accept_button->setText("Yes");
	m_reject_button->setText("No");
}

GameOverScreen::~GameOverScreen()
{
}

void GameOverScreen::update(const float& dt)
{
	DialogBase::update(dt);
}

void GameOverScreen::render(sf::RenderTarget* target)
{
	DialogBase::render(target);
}
