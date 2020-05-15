#include "Entity.h"

void Entity::draw(sf::RenderWindow& app)
{
    anim.getSprite().setPosition(x, y);
    anim.getSprite().setRotation(angle + 90);
    app.draw(anim.getSprite());

    sf::CircleShape circle(R);
    circle.setFillColor(sf::Color(255, 0, 0, 170));
    circle.setPosition(x, y);
    circle.setOrigin(R, R);
}

bool Entity::isCollide(std::shared_ptr <Entity> e) {
    return (x - e->x) * (x - e->x) +
        (y - e->y) * (y - e->y) <
        (R + e->R) * (R + e->R);
}