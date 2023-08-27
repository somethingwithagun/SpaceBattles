#pragma once
#include "RangedWeapon.h"
#include <iostream>

class TwoBombs :
	public RangedWeapon
{
private:
	bool m_first_shoots;
	float m_weapon_counter;
	bool m_first_visible;
	bool m_second_visible;

public:
	TwoBombs(std::vector<Bullet*>& bullets);
	virtual ~TwoBombs();

	void update(const float& rotation_angle, const sf::Vector2f center, const float& dt) override;
	bool shoot(const sf::String& owner, const float& rotation_angle, const sf::Vector2f& center, const sf::Vector2f& origin) override;
	void render(sf::RenderTarget* target = nullptr) override;

};

