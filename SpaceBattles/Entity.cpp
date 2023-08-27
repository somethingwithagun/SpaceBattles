#include "Entity.h"

Entity::Entity(sf::Vector2f position)
{
	this->m_rotation_speed = 90.f;

	//this->m_size = sf::Vector2f(0, 0);

	this->m_body.setPosition(position);
	//this->m_body.setOrigin(this->m_size.x / 2.0, this->m_size.y / 2.0f);

	//this->m_c = new Collider(m_body, 0, -500, defaultEntitySize.x, defaultEntitySize.y);
}


Entity::~Entity()
{
	delete m_animation_component;
	delete m_movement_component;
	delete m_c;
}

void Entity::create_animation_component(sf::Texture& texture_sheet)
{
	m_animation_component = new AnimationComponent(m_body, texture_sheet);
}

void Entity::create_movement_component(float max_velocity, float acceleration, float deceleration, float max_rotation, float rotation_acceleration, float rotation_deceleration)
{
	m_movement_component = new MovementComponent(max_velocity, acceleration, deceleration, max_rotation, rotation_acceleration, rotation_deceleration, m_body);
}

void Entity::create_collider_component(float offset_x, float offset_y, float width, float height)
{
	m_c = new Collider(m_body, offset_x, offset_y, width, height);
}

void Entity::update(const float& dt)
{
	if (m_movement_component)
		m_movement_component->update(dt);

	//std::cout << m_body.getPosition().x << ' ' << m_body.getPosition().y << '\n';
}

const sf::Sprite& Entity::getBody() const
{
	return m_body;
}
Collider& Entity::getCollider() const
{
	return *this->m_c;
}
const sf::Vector2f& Entity::getScale() const
{
	return m_body.getScale();
}
const float Entity::getRotationSpeed() const
{
	return m_rotation_speed;
}
const float Entity::getRotation() const
{
	return m_body.getRotation();
}

const sf::Vector2f& Entity::getSize() const
{
	sf::Vector2f size(m_body.getGlobalBounds().width, m_body.getGlobalBounds().height);
	//std::cout << '\n' << size.x << ' ' << size.y << '\n';
	return size;
}
void Entity::move(const float& dir_x, const float& dir_y, const float& dt)
{
	if (m_body.getTexture() && m_movement_component) // if not nullptr
	{
		m_movement_component->move(dir_x, dir_y, m_body.getRotation(), dt);

	}
}
void Entity::move(const sf::Vector2f& to)
{
	m_body.setPosition(m_body.getPosition() + to);
}
void Entity::rotate(const float& dir, const float& dt)
{
	//	this->m_body.rotate(dir * m_rotation_speed * dt);
	if (m_body.getTexture() && m_movement_component) // if not nullptr
	{
		m_movement_component->rotate(dir, dt);

	}
}
const sf::Vector2f& Entity::getPos() const
{
	return this->m_body.getPosition();
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return m_body.getGlobalBounds();
}
void Entity::setRotation(const float& rotation)
{
	m_body.setRotation(rotation);
}
void Entity::setPos(sf::Vector2f position)
{
	this->m_body.setPosition(position);
}

void Entity::setTexture(const sf::Texture& texture)
{
	m_body.setTexture(texture);
}
