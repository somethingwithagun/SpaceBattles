#include "DialogBase.h"

DialogBase::DialogBase(const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color, Buttons mode)
	: m_buttons_size(buttons_size)
{
	m_is_opened = false;
	m_result = Result::None;

	m_font.loadFromFile(DEFAULT_FONT_PATH);

	m_background.setSize(background_size);
	m_background.setOrigin(m_background.getSize().x / 2, m_background.getSize().y / 2);
	m_background.setFillColor(sf::Color(130, 128, 124));

	m_text.setString("");
	m_text.setFont(m_font);
	m_text.setPosition(m_background.getPosition().x - (m_text.getGlobalBounds().width / 2), m_background.getPosition().y - 50);

	sf::Vector2f reject_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (buttons_size.x / 2) - 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
	sf::Vector2f accept_btn_pos(m_background.getPosition().x - (m_background.getSize().x / 2) + (buttons_size.x / 2) + 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
	sf::Vector2f close_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (50 / 2), m_background.getPosition().y - (m_background.getSize().y / 2) + (50 / 2));
	//sf::Vector2f accept_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (m_accept_button->getSize().x / 2) - 40, m_background.getPosition().y);

	switch (mode)
	{
	case DialogBase::Accept:
		m_accept_button = new Button(buttons_size, "Accept", m_font, buttons_color);
		m_accept_button->setPosition(accept_btn_pos);
		break;
	case DialogBase::Reject:
		m_reject_button = new Button(buttons_size, "Reject", m_font, buttons_color);
		m_reject_button->setPosition(reject_btn_pos);
		break;
	case DialogBase::Close:
		m_close_button = new Button(sf::Vector2f(50, 50), "X", m_font, sf::Color::Red);
		m_close_button->setPosition(close_btn_pos);
		break;
	case DialogBase::Accept | DialogBase::Reject | DialogBase::Close:
		m_accept_button = new Button(buttons_size, "Accept", m_font, buttons_color);
		m_accept_button->setPosition(accept_btn_pos);

		m_reject_button = new Button(buttons_size, "Reject", m_font, buttons_color);
		m_reject_button->setPosition(reject_btn_pos);

		m_close_button = new Button(sf::Vector2f(50, 50), "X", m_font, sf::Color::Red);
		m_close_button->setPosition(close_btn_pos);
		break;

	case DialogBase::Accept | DialogBase::Reject:
		m_accept_button = new Button(buttons_size, "Accept", m_font, buttons_color);
		m_accept_button->setPosition(accept_btn_pos);

		m_reject_button = new Button(buttons_size, "Reject", m_font, buttons_color);
		m_reject_button->setPosition(reject_btn_pos);
		break;
	default:
		break;
	}
}

DialogBase::DialogBase()
{
}

DialogBase::~DialogBase()
{
	if (m_accept_button)
		delete m_accept_button;
	if (m_reject_button)
		delete m_reject_button;
	if (m_close_button)
		delete m_close_button;

	if (m_background_texture)
		delete m_background_texture;
}

void DialogBase::update(const float& dt)
{
	m_text.getGlobalBounds().width / 2, (m_text.getGlobalBounds().height / 2) + m_text.getGlobalBounds().height / 4;

	if (m_accept_button)
		m_accept_button->update(dt);

	if (m_close_button)
		m_close_button->update(dt);

	if (m_reject_button)
		m_reject_button->update(dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());

		if (m_close_button && m_close_button->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterA >= 0.006)
			{
				m_close_button->setPressed(true);
				m_counterA = 0;

				m_is_opened = false;

				m_result = Result::Closed;
			}
		}
		else if (m_accept_button && m_accept_button->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterB >= 0.006)
			{
				m_accept_button->setPressed(true);
				m_counterB = 0;

				m_is_opened = false;

				m_result = Result::Accepted;
			}
		}
		else if (m_reject_button && m_reject_button->getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
		{
			if (m_counterC >= 0.006)
			{
				m_reject_button->setPressed(true);
				m_counterC = 0;

				m_is_opened = false;

				m_result = Result::Rejected;
			}
		}
	}
	m_counterA += dt;
	m_counterB += dt;
	m_counterC += dt;
}

void DialogBase::render(sf::RenderTarget* target)
{

	if (m_is_opened)
	{
		target->draw(m_background);

		if (m_accept_button)
			target->draw(*m_accept_button);

		if (m_close_button)
			target->draw(*m_close_button);

		if (m_reject_button)
			target->draw(*m_reject_button);

		target->draw(m_text);
	}
}

void DialogBase::show()
{
	m_is_opened = true;
}

void DialogBase::close()
{
	m_is_opened = false;
}

void DialogBase::setText(const sf::String& text)
{
	m_text.setString(text);
	m_text.setPosition(m_background.getPosition().x - (m_text.getGlobalBounds().width / 2), m_background.getPosition().y - 50);
}

void DialogBase::setPosition(const sf::Vector2f& pos)
{
	m_background.setPosition(pos);

	m_text.setPosition(m_background.getPosition().x - (m_text.getGlobalBounds().width / 2), m_background.getPosition().y - 50);

	if (m_accept_button)
	{
		sf::Vector2f accept_btn_pos(m_background.getPosition().x - (m_background.getSize().x / 2) + (m_buttons_size.x / 2) + 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
		m_accept_button->setPosition(accept_btn_pos);
	}

	if (m_reject_button)
	{
		sf::Vector2f reject_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (m_buttons_size.x / 2) - 40, m_background.getPosition().y + (m_background.getSize().y / 2) - 40);
		m_reject_button->setPosition(reject_btn_pos);
	}

	if (m_close_button)
	{
		sf::Vector2f close_btn_pos(m_background.getPosition().x + (m_background.getSize().x / 2) - (50 / 2), m_background.getPosition().y - (m_background.getSize().y / 2) + (50 / 2));
		m_close_button->setPosition(close_btn_pos);
	}
}

DialogBase::Result DialogBase::get_result() const
{
	return m_result;
}

const bool& DialogBase::get_is_open() const
{
	return m_is_opened;
}
