#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
public:
	Collider(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height);
	virtual ~Collider();

	void render(sf::RenderTarget* target);

	//bool checkCollision(const Collider& other);
	//bool checkCollision(const sf::Vector2f& coords, const sf::Vector2f& size);

	bool checkCollision(const sf::FloatRect& frect);

	bool checkMouseCollision(const sf::Vector2f& mousePos);

	void updateHitboxPosition();
	void updateHitboxSize();
	void updateRotation();

	//getters
	const sf::Vector2f& getOrigin() const;
	const sf::Vector2f getHalfSize() const;
	const sf::Vector2f& getPos() const;
	const sf::FloatRect& getGlobalBounds()const;

	//setters

	void setSize(const sf::Vector2f& size);
	void setOffset(const float& x, const float& y);
	void setPosition(const sf::Vector2f& pos);

private:
	float m_offset_x;
	float m_offset_y;

	sf::RectangleShape m_hitbox;
	sf::Sprite& m_body;
};
