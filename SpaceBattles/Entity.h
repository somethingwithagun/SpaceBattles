#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "collider.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"

static const sf::Vector2f defaultEntitySize(100, 150);
//static const float fire_offset = 60;

class Entity
{
public:
	Entity(sf::Vector2f position);
	virtual	~Entity();

	void create_animation_component(sf::Texture& texture_sheet);
	void create_movement_component(float max_velocity, float acceleration, float deceleration, float max_rotation, float rotation_acceleration, float rotation_deceleration);
	void create_collider_component(float offset_x, float offset_y, float width, float height);

	// update
	virtual void update(const float& dt);

	// render
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

	// move
	virtual void move(const float& dir_x, const float& y, const float& dt);
	virtual void move(const sf::Vector2f& to);

	// rotate
	virtual void rotate(const float& dir, const float& dt);

	//getters
	const sf::Vector2f& getPos() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::Vector2f& getSize() const;
	const sf::Sprite& getBody() const;
	Collider& getCollider() const;
	const sf::Vector2f& getScale() const;
	const float getRotationSpeed() const;
	const float getRotation() const;

	//setters
	void setRotation(const float& rotation);
	void setPos(sf::Vector2f position);
	void setTexture(const sf::Texture& texture);

protected:
	AnimationComponent* m_animation_component = nullptr;
	MovementComponent* m_movement_component = nullptr;
	Collider* m_c;

	float m_rotation_speed;

	sf::Sprite m_body;
	sf::Vector2f m_size;
};