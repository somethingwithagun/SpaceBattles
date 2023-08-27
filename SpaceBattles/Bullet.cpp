#include "Bullet.h"

void Bullet::update()
{
	m_sprite.move(m_traectory);
}

const int& Bullet::get_damage() const
{
	return m_damage;
}

const sf::String Bullet::getOwner() const
{
	return m_owner;
}

const sf::Vector2f& Bullet::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::Vector2f& Bullet::getOrigin() const
{
	return m_sprite.getOrigin();
}

const float Bullet::getSpeed() const
{
	return m_speed;
}

const float Bullet::getRotation() const
{
	return m_sprite.getRotation();
}

const sf::Vector2f& Bullet::getSize() const
{
	return sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
}

const sf::FloatRect& Bullet::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

Bullet::Bullet(const sf::String& owner, const float& rotation, const float& offset_x, const float& offset_y, const sf::Vector2f& center, const sf::Vector2f& origin, const float& speed, const int& damage, sf::Texture texture) : m_speed(speed), m_damage(damage), m_owner(owner)
{
	m_sprite_texture = texture;

	m_sprite.setTexture(m_sprite_texture);
	m_sprite.setRotation(rotation);
	m_sprite.setOrigin(sf::Vector2f(origin.x + offset_x, origin.y + offset_y));
	m_sprite.setPosition(center);
	m_sprite.setScale(0.5, 0.5);

	m_traectory.x = m_speed * std::cos((rotation + 90) * (3.14 / 180)) * -1;
	m_traectory.y = m_speed * std::sin((rotation + 90) * (3.14 / 180)) * -1;


}
void Bullet::render(sf::RenderTarget* target)
{
	target->draw(m_sprite);
}
Bullet::~Bullet()
{
}
