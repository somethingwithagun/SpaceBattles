#include "Player.h"

Player::Player(sf::String name, int max_health, sf::Vector2f pos, std::vector<Bullet*>& bullets, sf::Texture& texture_sheet) :Entity(pos)
{
	m_name = name;
	m_max_health = max_health;
	m_health = max_health;

	m_two_bombs = new TwoBombs(bullets);

	create_movement_component(200.f, 20.f, 1.f, 25.f, 5.f, 1.f);

	create_animation_component(texture_sheet);

	m_animation_component->add_animation("MOVING", 100.f, 0, -5, 0, 0, 4, 0, 272, 570);
	m_animation_component->add_animation("IDLE", 100.f, 0, 80, 1, 1, 4, 1, 272, 480);

	m_body.setScale(0.5, 0.5);

	create_collider_component(0, 0, 271 * 0.5, 480 * 0.5);
}

Player::~Player()
{
}

bool Player::shoot()
{
	return m_two_bombs->shoot(m_name, m_body.getRotation(), m_body.getPosition(), m_body.getOrigin() / 2.f);
}

void Player::update(const float& dt)
{
	Entity::update(dt);

	if (m_player_state == Idle)
		m_animation_component->play("IDLE", dt);
	else if (m_player_state == Moving)
		m_animation_component->play("MOVING", dt);

	if (m_two_bombs)
		m_two_bombs->update(m_body.getRotation(), m_body.getPosition(), dt);

	if (m_c)
	{
		m_body.setOrigin(m_body.getOrigin().x, m_c->getOrigin().y * 2);

		m_c->updateHitboxPosition();
		m_c->updateRotation();
	}
}

void Player::render(sf::RenderTarget* target)
{
	m_two_bombs->render(target);

	target->draw(m_body);
}

const sf::String& Player::getName() const
{
	return m_name;
}

const int& Player::getHealth() const
{
	return m_health;
}

void Player::decrease_health(const int& damage)
{
	m_health -= damage;
}

void Player::setHealth(const int& health)
{
	m_health = m_max_health = health;
}

void Player::set_state(PlayerState state)
{
	m_player_state = state;
}

Player::PlayerState Player::get_state()
{
	return m_player_state;
}

RangedWeapon* Player::get_weapon_handle()
{
	if (m_two_bombs)
		return m_two_bombs;

	return nullptr;
}

MovementComponent* Player::get_movement_component_ptr()
{
	if (m_movement_component)
		return m_movement_component;
}
