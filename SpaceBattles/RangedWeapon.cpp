#include "RangedWeapon.h"
#include <iostream>

int RangedWeapon::get_damage() const
{
	return m_damage;
}

RangedWeapon::RangedWeapon(std::vector<Bullet*>& bullets) : m_bullets(bullets)
{

}

RangedWeapon::~RangedWeapon()
{
}

const sf::Texture& RangedWeapon::get_bullet_texture() const
{
	return m_bullet_texture;
}
