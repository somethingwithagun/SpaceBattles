#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet
{
private:
	sf::Vector2f m_traectory;
	sf::Sprite m_sprite;
	sf::Texture m_sprite_texture;

	const float m_speed;
	const int m_damage;


public:
	Bullet(const sf::String& owner, const float& rotation, const float& offset_x, const float& offset_y, const sf::Vector2f& center, const sf::Vector2f& origin, const float& speed, const int& damage, sf::Texture texture);
	virtual ~Bullet();

	void render(sf::RenderTarget* target = nullptr);
	void update();

	const sf::String m_owner;

	const int& get_damage() const;

	const sf::String getOwner() const;
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getOrigin() const;
	const float getSpeed() const;
	const float getRotation() const;
	const sf::Vector2f& getSize() const;
	const sf::FloatRect& getGlobalBounds() const;
};

