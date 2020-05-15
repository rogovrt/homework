#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Animation.h"

class Entity
{
public:
    Entity()
    {
        life = 1;
    }

    void settings(Animation& a, float X, float Y, float Angle = 0, float radius = 1)
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update() {};

    void draw(sf::RenderWindow& app);

    bool isCollide(std::shared_ptr<Entity> e);

    virtual ~Entity() {};

public:
    float x, y, dx, dy, R, angle;
    int life;
    std::string name;
    Animation anim;
};
