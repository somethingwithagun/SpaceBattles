#pragma once
#include "Entity.h"

class Meteor :
	public Entity
{
private:
	float m_integrity;
	int m_direction;

public:
	Meteor(const sf::Vector2f& position, const sf::Vector2f& scale, const int& direction, const float& rotation_speed,
		sf::Texture& texture_sheet);
	virtual ~Meteor();

	void decrease_integrity(const float& damage);

	const float& getIntegrity() const;
	void setIntegrity(const float& integrity);
	const int& get_direction() const;

	void update(const float& dt) override;
	void render(sf::RenderTarget* target = nullptr) override;

};

