#pragma once
#include <SFML/Graphics.hpp>
class MovementComponent
{
private:
	sf::Vector2f m_velocity;

	float m_max_velocity;
	float m_acceleration;
	float m_deceleration;

	sf::Sprite& m_sprite;

	float m_rotation;
	float m_rotation_acceleration;
	float m_rotation_deceleration;
	float m_max_rotation;
public:
	MovementComponent(float max_velocity, float acceleration, float deceleration, float max_rotation, float rotation_acceleration, float rotation_deceleration, sf::Sprite& sprite);
	virtual ~MovementComponent();

	const sf::Vector2f& getVelocity() const;
	const float& getRotation() const;

	void move(const float& dir_x, const float& dir_y, const float& rotation, const float& dt);
	void rotate(const float& angle, const float& dt);
	void update(const float& dt);

};

