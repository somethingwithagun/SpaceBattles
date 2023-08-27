#ifndef BUTTON_H
#define BUTTON_H

#include "Collider.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>

class GameOverScreen;

const sf::Vector2f defaultButtonSize(200, 100);

class Button : public sf::Drawable
{
public:
	Button(sf::Vector2f size, sf::String text, sf::Font font, sf::Color bgColor = sf::Color::Red, int textSize = DEFAULT_CHARACTER_SIZE);
	Button(sf::Vector2f size, sf::String text, sf::Color bgColor = sf::Color::Red, int textSize = DEFAULT_CHARACTER_SIZE);

	~Button();

	void update(double deltaT);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// setters
	void setText(sf::String text);

	void setFont(sf::Font font);

	void setCharacterSize(int size);

	void setPressed(bool newPressed);

	void setFillColor(sf::Color color, sf::Color textColor = sf::Color::White);

	void setPosition(sf::Vector2f pos);

	void setEnabled(bool enabled);

	// getters
	const bool& getEnabled() const;

	const sf::FloatRect& getGlobalBounds() const;

	const sf::Vector2f getSize() const;

	const sf::Vector2f getPosition() const;

	const int getCharacterSize() const;

	const sf::Color& getButtonColor() const;

	const sf::Color& getTextColor() const;

	const sf::Font& getFont() const;

	const sf::String& getString() const;

private:
	sf::Color m_source_background_color;
	sf::Color m_source_text_color;

	bool m_enabled;
	bool pressed;
	double counter;
	sf::RectangleShape m_body;
	sf::Text text;
	sf::Font font;
};
#endif