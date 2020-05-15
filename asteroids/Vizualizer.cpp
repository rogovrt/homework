#include "Vizualizer.h"

bool Vizualizer::isWork() {
	return flag;
}

void Vizualizer::isClose() {
    sf::Event event;
    while (m_app.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_app.close();
    }
}

void Vizualizer::draw(const std::list<std::shared_ptr<Entity>>& entities, std::string& s, bool b) {
    back.setTexture(t2);
    flag = b;
    m_app.draw(back);
    for (auto i : entities) i->draw(m_app);
    m_text.setString(s);
    m_app.draw(m_text);
    m_app.display();
}