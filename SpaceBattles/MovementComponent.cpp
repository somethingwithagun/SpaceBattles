#include "MovementComponent.h"

MovementComponent::MovementComponent(float max_velocity, float acceleration, float deceleration, float max_rotation, float rotation_acceleration,
	float rotation_deceleration, sf::Sprite& sprite)
	: m_max_velocity(max_velocity), m_sprite(sprite), m_acceleration(acceleration), m_deceleration(deceleration),
	m_max_rotation(max_rotation), m_rotation_acceleration(rotation_acceleration), m_rotation_deceleration(rotation_deceleration)
{
	m_velocity = sf::Vector2f(0, 0);
	m_rotation = 0.f;
}

MovementComponent::~MovementComponent()
{
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return m_velocity;
}

const float& MovementComponent::getRotation() const
{
	return m_rotation;
}

void MovementComponent::move(const float& dir_x, const float& dir_y, const float& rotation, const float& dt)
{
	// Acceleration

	m_velocity.x += m_acceleration * std::cos((rotation + 90) * (3.14159265359 / 180)) * dir_x;

	if (std::abs(m_velocity.x) > m_max_velocity)
		m_velocity.x = (m_velocity.x > 0.f) ? m_max_velocity : -m_max_velocity;

	m_velocity.y += m_acceleration * std::sin((rotation + 90) * (3.14159265359 / 180)) * dir_y;

	if (std::abs(m_velocity.y) > m_max_velocity)
		m_velocity.y = (m_velocity.y > 0.f) ? m_max_velocity : -m_max_velocity;

}

void MovementComponent::rotate(const float& angle, const float& dt)
{
	m_rotation += m_rotation_acceleration * angle;
	if (std::abs(m_rotation) > m_max_rotation)
		m_rotation = (m_rotation > 0.f) ? m_max_rotation : -m_max_rotation;
}

void MovementComponent::update(const float& dt)
{

	if (m_rotation > 0.f)
	{
		m_rotation -= m_rotation_deceleration;
		if (m_rotation < 0.f)
			m_rotation = 0.f;
	}
	else if (m_rotation < 0.f)
	{
		m_rotation += m_rotation_deceleration;
		if (m_rotation > 0.f)
			m_rotation = 0.f;
	}
	m_sprite.rotate(m_rotation * dt);

	//Deceleration
	if (m_velocity.x > 0.f)
	{
		m_velocity.x -= m_deceleration;
		if (m_velocity.x < 0.f)
			m_velocity.x = 0.f;
	}
	else if (m_velocity.x < 0.f)
	{
		m_velocity.x += m_deceleration;
		if (m_velocity.x > 0.f)
			m_velocity.x = 0.f;
	}

	if (m_velocity.y > 0.f)
	{
		m_velocity.y -= m_deceleration;
		if (m_velocity.y < 0.f)
			m_velocity.y = 0.f;
	}
	else if (m_velocity.y < 0.f)
	{
		m_velocity.y += m_deceleration;
		if (m_velocity.y > 0.f)
			m_velocity.y = 0.f;
	}
	m_sprite.move(m_velocity * dt);
}
