#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet)
	: m_sprite(sprite), m_texture_sheet(texture_sheet)
{
}

AnimationComponent::~AnimationComponent()
{
	for (auto& i : m_animations)
		delete i.second;
}

void AnimationComponent::add_animation(const std::string& key,
	float animation_timer, float offset_x, float offset_y, int start_frame_x, int start_frame_y,
	int frames_x, int frames_y, int width, int height)
{
	m_animations[key] = new Animation(m_sprite, m_texture_sheet, animation_timer, offset_x, offset_y, start_frame_x, start_frame_y, frames_x, frames_y, width, height);
}

void AnimationComponent::play(const std::string& key, const float& dt)
{
	m_animations[key]->m_name = key;
	m_animations[key]->play(dt);
	m_current_animation_name = key;

}

const std::string& AnimationComponent::get_current_animation_name() const
{
	return m_current_animation_name;
}
