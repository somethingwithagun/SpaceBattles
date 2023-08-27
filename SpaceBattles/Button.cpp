#include "Button.h"

Button::Button(sf::Vector2f size, sf::String text, sf::Font font, sf::Color bgColor, int textSize)
	: m_body(size), font(font), counter(0.0), pressed(false)
{
	m_source_background_color = bgColor;
	m_source_text_color = sf::Color::White;

	this->m_body.setFillColor(bgColor);
	this->text.setString(text);

	//this->text.setPosition(this->m_body.getPosition().x - this->text.getGlobalBounds().width / 2, this->m_body.getPosition().y - this->m_body.getGlobalBounds().height / 8);
	this->text.setPosition(m_body.getPosition());
	this->text.setFont(this->font);
	this->text.setCharacterSize(textSize);

	this->m_body.setOrigin(this->m_body.getGlobalBounds().width / 2, this->m_body.getGlobalBounds().height / 2);
	this->text.setOrigin(this->text.getGlobalBounds().width / 2, (this->text.getGlobalBounds().height / 2) + this->text.getGlobalBounds().height / 4);

}

Button::Button(sf::Vector2f size, sf::String text, sf::Color bgColor, int textSize)
	: m_body(size), counter(0.0), pressed(false)
{
	m_source_background_color = bgColor;
	m_source_text_color = sf::Color::White;

	this->m_body.setFillColor(bgColor);

	this->text.setString(text);
	this->text.setFont(this->font);
	this->text.setCharacterSize(textSize);

	this->m_body.setOrigin(this->m_body.getGlobalBounds().width / 2, this->m_body.getGlobalBounds().height / 2);
	this->text.setOrigin(this->text.getGlobalBounds().width / 2, (this->text.getGlobalBounds().height / 2) + this->text.getGlobalBounds().height / 4);

	this->text.setPosition(m_body.getPosition());
	//this->text.setPosition(this->m_body.getPosition().x - this->text.getGlobalBounds().width / 2, this->m_body.getPosition().y - this->m_body.getGlobalBounds().height / 8);

	this->font.loadFromFile(DEFAULT_FONT_PATH);
}

Button::~Button()
{
}
const bool& Button::getEnabled() const
{
	return m_enabled;
}
const sf::FloatRect& Button::getGlobalBounds() const
{
	return m_body.getGlobalBounds();
}

void Button::update(double deltaT)
{
	this->text.setOrigin(this->text.getGlobalBounds().width / 2, (this->text.getGlobalBounds().height / 2) + this->text.getGlobalBounds().height / 4);
	if (counter >= 0.006)
	{
		if (pressed)
		{
			pressed = false;
			counter = 0;
		}
	}
	counter += deltaT;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body);
	target.draw(text);
}


void Button::setText(sf::String text)
{
	this->text.setString(text);
}

void Button::setFont(sf::Font font)
{
	this->font = font;
}

void Button::setCharacterSize(int size)
{
	this->text.setCharacterSize(size);
}

void Button::setPressed(bool newPressed)
{
	pressed = newPressed;
}

void Button::setFillColor(const sf::Color color, const sf::Color textColor)
{
	this->m_body.setFillColor(color);
	this->text.setFillColor(textColor);

	m_source_background_color = color;
	m_source_text_color = textColor;
}

void Button::setPosition(sf::Vector2f pos)
{
	this->m_body.setPosition(pos);
	this->text.setPosition(this->m_body.getPosition());
}

void Button::setEnabled(bool enabled)
{
	m_enabled = enabled;

	if (m_enabled)
	{
		m_body.setFillColor(m_source_background_color);
		text.setFillColor(m_source_text_color);
	}
	else
	{
		m_body.setFillColor(sf::Color(m_source_background_color.r, m_source_background_color.g, m_source_background_color.b, m_source_background_color.a - 80));
		text.setFillColor(sf::Color(m_source_text_color.r, m_source_text_color.g, m_source_text_color.b, m_source_text_color.a - 80));
	}
}

const sf::Vector2f Button::getSize() const
{
	return m_body.getSize();
}

const sf::Vector2f Button::getPosition() const
{
	return m_body.getPosition();
}

const int Button::getCharacterSize() const
{
	return this->text.getCharacterSize();
}

const sf::Color& Button::getButtonColor() const
{
	return this->m_body.getFillColor();
}

const sf::Color& Button::getTextColor() const
{
	return this->text.getFillColor();
}

const sf::Font& Button::getFont() const
{
	return this->font;
}
const sf::String& Button::getString() const
{
	return this->text.getString();
}
