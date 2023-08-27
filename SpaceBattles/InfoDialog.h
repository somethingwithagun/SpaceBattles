#pragma once
#include "DialogBase.h"

class InfoDialog :
	public DialogBase
{

private:

public:
	InfoDialog(const sf::String& text, const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color, Buttons mode = static_cast<Buttons>(Buttons::Accept | Buttons::Reject | Buttons::Close));
	~InfoDialog();

	virtual void update(const float& dt) override;
	virtual void render(sf::RenderTarget* target) override;

};

