#include "InfoDialog.h"

InfoDialog::InfoDialog(const sf::String& text, const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color, Buttons mode) : DialogBase(background_size, buttons_size, buttons_color, mode)
{
	m_text.setString(text);
}

InfoDialog::~InfoDialog()
{
}

void InfoDialog::update(const float& dt)
{
	DialogBase::update(dt);
}

void InfoDialog::render(sf::RenderTarget* target)
{
	DialogBase::render(target);
}

