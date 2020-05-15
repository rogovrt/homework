#pragma once

#include <SFML/Graphics.hpp>
#include <list>
#include "Entity.h"

class Vizualizer {
public:
	Vizualizer() : m_app(sf::RenderWindow (sf::VideoMode(1200, 800), "Asteroids!")) {
		m_font.loadFromFile("consolas.ttf");
		m_app.setFramerateLimit(60);
		m_text.setFont(m_font);
		m_text.setCharacterSize(14U);
		m_text.setFillColor(sf::Color::Red);
		m_text.setPosition(10.0f, 10.0f);
		flag = true;
		t2.loadFromFile("images/background.jpg");
		t2.setSmooth(true);
	}

	bool isWork();

	void isClose();

	void draw(const std::list<std::shared_ptr<Entity>>& entities, std::string& s, bool b);

private:
	sf::RenderWindow m_app;
	sf::Text m_text;
	sf::Font m_font;
	sf::Texture t2;
	sf::Sprite back;
	bool flag;
};