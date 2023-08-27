#pragma once

#include <vector>
#include <map>
#include <stack>

#include <SFML/Graphics.hpp>

class State
{
public:
	explicit State(sf::RenderWindow* window, std::map<std::string, int>* supported_keys);
	virtual ~State();

	const bool& get_quit() const;
	const bool& get_game_quit() const;

	virtual void check_for_quit();

	virtual bool end_state() = 0;

	virtual void push_next_state(std::stack<State*>& states) = 0;

	virtual void init() = 0;

	void setServerIp(const sf::String& ip);
	void setServerPort(const sf::String& port);
	void setUsername(const sf::String& username);

	const sf::String& getServerIp() const;
	const sf::String& getServerPort() const;
	const sf::String& getUsername() const;

	virtual void update_keybinds(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
	virtual void process_event(sf::Event& ev) = 0;


protected:
	sf::RenderWindow* m_window = nullptr;

	std::map<std::string, int>* m_supported_keys = nullptr;
	std::map<std::string, int> m_keybinds;

	bool m_quit;
	bool m_game_quit;

	sf::String m_server_ip;
	sf::String m_server_port;
	sf::String m_username;

	std::map<std::string, sf::Texture> m_textures;

};

