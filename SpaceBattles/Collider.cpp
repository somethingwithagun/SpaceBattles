#include "Collider.h"
#include <iostream>
Collider::Collider(sf::Sprite& sprite, float offset_x, float offset_y, float width, float height) : m_body(sprite)
{
	m_offset_x = offset_x;
	m_offset_y = offset_y;

	m_hitbox = sf::RectangleShape(sf::Vector2f(width, height));
	m_hitbox.setPosition(m_hitbox.getPosition() + sf::Vector2f(m_offset_x, m_offset_y));
}

Collider::~Collider()
{

}

void Collider::render(sf::RenderTarget* target)
{
	target->draw(m_hitbox);
}

/*bool Collider::checkCollision(const Collider& other)
{
	sf::Vector2f delta(this->getPos() - other.getPos());

	sf::Vector2f halfSize(this->getHalfSize() + other.getHalfSize());

	if ((std::abs(delta.x) - halfSize.x) < 0 && (std::abs(delta.y) - halfSize.y) < 0)
		return true;

	// else
	return false;
}
bool Collider::checkCollision(const sf::Vector2f& coords, const sf::Vector2f& size)
{
	sf::Vector2f delta(this->getPos() - coords);

	sf::Vector2f halfSize(this->getHalfSize() + sf::Vector2f(size.x / 2.0f, size.y / 2.0f));

	//std::cout << "half size: " << halfSize.x << ' ' << halfSize.y << '\n';
	//std::cout << "delta: " << delta.x << ' ' << delta.y << '\n';

	if ((std::abs(delta.x) - halfSize.x) < 0 && (std::abs(delta.y) - halfSize.y) < 0)
		return true;

	// else
	return false;
}*/
bool Collider::checkCollision(const sf::FloatRect& frect)
{
	return m_hitbox.getGlobalBounds().intersects(frect);
}
bool Collider::checkMouseCollision(const sf::Vector2f& mousePos)
{
	sf::Vector2f delta(this->getPos() - mousePos);

	sf::Vector2f halfSize(this->getHalfSize() + sf::Vector2f(1, 1));

	if ((std::abs(delta.x) - halfSize.x) < 0 && (std::abs(delta.y) - halfSize.y) < 0)
		return true;

	// else
	return false;
}

void Collider::updateHitboxPosition()
{
	m_hitbox.setOrigin(m_hitbox.getSize().x / 2 + m_offset_x, m_hitbox.getSize().y / 2 + m_offset_y);
	m_hitbox.setPosition(m_body.getPosition());// + sf::Vector2f(m_offset_x, m_offset_y));
}

void Collider::updateHitboxSize()
{
	m_hitbox.setSize(sf::Vector2f(m_body.getGlobalBounds().width, m_body.getGlobalBounds().height));
}

void Collider::updateRotation()
{
	m_hitbox.setRotation(m_body.getRotation());
}

const sf::Vector2f& Collider::getPos() const
{
	return this->m_hitbox.getPosition();
}
const sf::FloatRect& Collider::getGlobalBounds() const
{
	return m_hitbox.getGlobalBounds();
}
void Collider::setSize(const sf::Vector2f& size)
{
	m_hitbox.setSize(size);
}
void Collider::setOffset(const float& x, const float& y)
{
	m_offset_x = x;
	m_offset_y = y;
}
void Collider::setPosition(const sf::Vector2f& pos)
{
	m_hitbox.setPosition(pos);
}
const sf::Vector2f& Collider::getOrigin() const
{
	return m_hitbox.getOrigin();
}
const sf::Vector2f Collider::getHalfSize() const
{
	return sf::Vector2f(this->m_hitbox.getSize().x / 2.0f, this->m_hitbox.getSize().y / 2.0f);
}