#pragma once
#include "Item.h"
#include "Bullet.h"
class Item;

class RangedWeapon : public	Item
{

private:

protected:
	sf::Texture m_weapon_texture;
	sf::Texture m_bullet_texture;
	sf::Sprite m_first_weapon_sprite;
	sf::Sprite m_second_weapon_sprite;

	std::vector<Bullet*>& m_bullets;

	int m_damage;
	float m_delay;
	float m_counter;

	sf::Vector2f bullet_traectory;

	int get_damage() const;

	virtual void update(const float& rotation_angle, const sf::Vector2f center, const float& dt) = 0;
	virtual bool shoot(const sf::String& owner, const float& rotation_angle, const sf::Vector2f& center, const sf::Vector2f& origin) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

public:
	RangedWeapon(std::vector<Bullet*>& bullets);
	virtual ~RangedWeapon();

	const sf::Texture& get_bullet_texture() const;
};

