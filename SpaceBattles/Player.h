#pragma once
#include <SFML/Graphics.hpp>

#include "Entity.h"

#include "TwoBombs.h"


class Player : public Entity
{
public:
	enum PlayerState
	{
		Idle = 0,
		Moving = 1
	};

public:
	Player(sf::String name, int max_health, sf::Vector2f pos, std::vector<Bullet*>& bullets, sf::Texture& texture_sheet);
	~Player();

	//virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	bool shoot();

	// update 
	virtual void update(const float& dt) override;

	// render
	virtual void render(sf::RenderTarget* target = nullptr) override;

	const sf::String& getName() const;

	const int& getHealth() const;
	void decrease_health(const int& damage);
	void setHealth(const int& health);

	void set_state(PlayerState state);

	PlayerState get_state();

	RangedWeapon* get_weapon_handle();

	MovementComponent* get_movement_component_ptr();

private:
	bool m_attacking;

	TwoBombs* m_two_bombs = nullptr;

	PlayerState m_player_state = Idle;

	int m_health;
	int m_max_health;
	sf::String m_name;
	//sf::Sprite m_player_sprite;
};

