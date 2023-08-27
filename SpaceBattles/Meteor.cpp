#include "Meteor.h"

Meteor::Meteor(const sf::Vector2f& position, const sf::Vector2f& scale, const int& direction, const float& rotation_speed,
	sf::Texture& texture_sheet) : Entity(position)
{
	std::cout << "METEOR SPAWNED AT " << position.x << ", " << position.y << '\n';
	std::cout << "METEOR SCALE " << scale.x << ", " << scale.y << '\n';

	m_body.setScale(scale);
	m_body.setPosition(position);

	m_integrity = 150.f; // hp
	m_rotation_speed = rotation_speed;

	create_animation_component(texture_sheet);
	create_movement_component(0, 0, 0, rotation_speed, 10, 0);
	create_collider_component(0, 0, getSize().x, getSize().y);

	m_animation_component->add_animation("IDLE", 100.f, 0, 0, 0, 0, 1, 0, 200, 250);
	m_body.setOrigin(m_body.getGlobalBounds().width / 2.f, m_body.getGlobalBounds().height / 2.f);

	m_direction = direction;
}

Meteor::~Meteor()
{
}

void Meteor::decrease_integrity(const float& damage)
{
	m_integrity -= damage;
}

const float& Meteor::getIntegrity() const
{
	return m_integrity;
}

void Meteor::setIntegrity(const float& integrity)
{
	m_integrity = integrity;
}

const int& Meteor::get_direction() const
{
	return m_direction;
}


void Meteor::update(const float& dt)
{
	Entity::update(dt);
}

void Meteor::render(sf::RenderTarget* target)
{
	target->draw(m_body);
}

