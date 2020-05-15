#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Animation
{
public:
    Animation() {}

    Animation(sf::Texture& t, float x, float y, float w, float h, int count, float Speed)
    {
        Frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(sf::IntRect(static_cast<int> (x + i * w), static_cast<int> (y), static_cast<int> (w), static_cast<int> (h)));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    sf::Sprite& getSprite() {
        return sprite;
    }

    void update();

    bool isEnd();

private:
    float Frame, speed;
    sf::Sprite sprite;
    std::vector<sf::IntRect> frames;
};