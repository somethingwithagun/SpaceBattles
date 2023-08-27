#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

static const float fire_offset = 100.f;

class AnimationComponent
{
private:

	class Animation
	{
	public:

		// constructors/destructors

		explicit Animation(sf::Sprite& sprite, sf::Texture& texture_sheet, float animation_timer, float offset_x, float offset_y, int start_frame_x, int start_frame_y, int frames_x, int frames_y, int width, int height)
			: m_sprite(sprite), m_texture_sheet(texture_sheet), m_width(width), m_height(height), m_animation_timer(animation_timer)
		{
			m_timer = 0.f;

			m_start_rect = sf::IntRect(start_frame_x * width + offset_x, start_frame_y * height + offset_y, width, height);
			m_current_rect = m_start_rect;
			m_end_rect = sf::IntRect((frames_x - 1) * width + offset_x, frames_y * height + offset_y, width, height);

			m_sprite.setTexture(m_texture_sheet, true);
			m_sprite.setTextureRect(m_start_rect);

			//m_sprite.setOrigin(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
		}
		virtual ~Animation() {}

		// variables

		sf::Texture& m_texture_sheet;
		sf::Sprite& m_sprite;
		float m_animation_timer;
		float m_timer;
		int m_width;
		int m_height;
		sf::IntRect m_start_rect;
		sf::IntRect m_current_rect;
		sf::IntRect m_end_rect;

		std::string m_name;


		// functions
		void play(const float& dt)
		{
			m_timer += 200.f * dt;
			if (m_timer >= m_animation_timer)
			{
				// reset timer
				m_timer = 0.f;

				// Animate
				if (m_current_rect != m_end_rect)
					m_current_rect.left += m_width;
				else // reset
					m_current_rect.left = m_start_rect.left;

				m_sprite.setTextureRect(m_current_rect);

				auto bounds = m_sprite.getLocalBounds();

				if (m_name == "MOVE")
					m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, (m_sprite.getLocalBounds().height - fire_offset) / 2);
				else
					m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, (m_sprite.getLocalBounds().height) / 2);

			}
		}
		void pause();

		void reset()
		{
			m_timer = 0.f;
			m_current_rect = m_start_rect;
		}
	};

	sf::Sprite& m_sprite;
	sf::Texture& m_texture_sheet;

	std::map<std::string, Animation*> m_animations;

	std::string m_current_animation_name;

public:
	explicit AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();


	// functions
	void add_animation(const std::string& key,
		float animation_timer, float offset_x, float offset_y, int start_frame_x, int start_frame_y,
		int frames_x, int frames_y, int width, int height);

	void play(const std::string& key, const float& dt);

	const std::string& get_current_animation_name() const;


};

