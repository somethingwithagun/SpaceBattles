#include "TwoBombs.h"

TwoBombs::TwoBombs(std::vector<Bullet*>& bullets) : RangedWeapon(bullets)
{
	m_first_shoots = true;
	m_weapon_counter = -1;
	m_first_visible = true;
	m_second_visible = true;

	if (!this->m_weapon_texture.loadFromFile("images/bomb.png"))
		std::cerr << "Texture not found!\n";

	if (!this->m_bullet_texture.loadFromFile("images/bomb.png"))
		std::cerr << "Texture not found!\n";

	this->m_first_weapon_sprite.setTexture(m_weapon_texture);
	this->m_second_weapon_sprite.setTexture(m_weapon_texture);

	m_first_weapon_sprite.setOrigin
	(
		m_first_weapon_sprite.getGlobalBounds().width / 2.f - 80,
		m_first_weapon_sprite.getGlobalBounds().height / 2.f - 30 // lower
	);

	m_second_weapon_sprite.setOrigin
	(
		m_second_weapon_sprite.getGlobalBounds().width / 2.f + 80,
		m_second_weapon_sprite.getGlobalBounds().height / 2.f - 30 // lower
	);

	m_first_weapon_sprite.setScale(0.5, 0.5);
	m_second_weapon_sprite.setScale(0.5, 0.5);

	m_damage = 25;
	m_delay = 1.f;
	m_counter = 0;
}

TwoBombs::~TwoBombs()
{
	for (int i = 0; i < m_bullets.size(); i++)
		if (m_bullets[i])
			delete m_bullets[i];

}

void TwoBombs::update(const float& rotation_angle, const sf::Vector2f center, const float& dt)
{

	for (int i = 0; i < m_bullets.size(); i++)
		if (m_bullets[i])
			m_bullets[i]->update();

	m_first_weapon_sprite.setPosition(center);
	m_second_weapon_sprite.setPosition(center);

	m_first_weapon_sprite.setRotation(rotation_angle);
	m_second_weapon_sprite.setRotation(rotation_angle);

	m_counter += dt;

	if (m_weapon_counter >= 0 && m_weapon_counter < m_delay)
		m_weapon_counter += dt;
	else
	{
		m_weapon_counter = -1;
		m_first_visible = true;
		m_second_visible = true;
	}
}

bool TwoBombs::shoot(const sf::String& owner, const float& rotation_angle, const sf::Vector2f& center, const sf::Vector2f& origin)
{
	if (m_counter >= m_delay)
	{
		// shoot animation
		if (m_first_shoots)
		{
			m_bullets.push_back(new Bullet(owner, rotation_angle, -120, -120, center, origin, 50, m_damage, m_bullet_texture));
			m_first_visible = false;
		}
		else
		{
			m_bullets.push_back(new Bullet(owner, rotation_angle, 70, -120, center, origin, 50, m_damage, m_bullet_texture));
			m_second_visible = false;
		}

		m_first_shoots ^= 1;
		m_weapon_counter = 0;
		m_counter = 0;

		return true;
	}
	return false;
}

void TwoBombs::render(sf::RenderTarget* target)
{
	for (int i = 0; i < m_bullets.size(); i++)
		if (m_bullets[i])
			m_bullets[i]->render(target);

	if (m_first_visible)
		target->draw(m_first_weapon_sprite);

	if (m_second_visible)
		target->draw(m_second_weapon_sprite);
}
