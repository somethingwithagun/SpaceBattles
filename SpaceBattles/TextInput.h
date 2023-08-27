#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"

#define DEFAULT_BACKGROUND_SIZE sf::Vector2f(220,30)
#define DEFAULT_CHARACTER_LIMIT 20

class TextInput
{

public:
	TextInput();
	TextInput(const sf::Vector2f& bgsize, sf::Font* font, const int character_size = DEFAULT_CHARACTER_SIZE);
	~TextInput();

	enum TextAlign
	{
		Right = 0,
		Left = 1,
		Center = 2
	};

	void update(double deltaT);
	void render(sf::RenderTarget* target = nullptr);

	/* SETTERS */

	void setPlaceholderText(const sf::String& text);

	void setAlign(TextAlign align);

	void setActive(bool active);

	void setFont(sf::Font* font);

	void setBackgroundColor(const sf::Color& color);

	void setTextColor(const sf::Color& color);

	void setCharacterSize(const unsigned int& size);

	void setSize(const sf::Vector2f& size);

	void setPosition(const sf::Vector2f& pos);

	void setText(const sf::String& text);

	void setCharactersLimit(const unsigned int limit);

	/* GETTERS */

	const int getCharactersLimit() const;

	const sf::FloatRect getGlobalBounds() const;

	const sf::Vector2f getSize() const;

	const int getCharacterSize() const;

	const sf::String& getText() const;

	const sf::String& getPlaceholderText() const;

	const sf::Color& getTextColor() const;

	const sf::Color& getBackgroundColor() const;

	const bool getActive() const;

	const TextAlign& getAlign() const;
private:

	sf::Font* m_font = nullptr;
	bool m_is_third_prty_font;

	sf::RectangleShape m_background;

	sf::Text m_label;

	sf::Text m_placeholder_label;

	sf::String m_text;

	sf::String m_placeholder_text;

	bool m_active = false;

	double m_counter = 0.0;

	TextAlign m_alignment = Center;

	int m_characters_limit = DEFAULT_CHARACTER_LIMIT;

};

