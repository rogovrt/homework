#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include "Entity.h"
#include "Randomizer.h"
#include "Vizualizer.h"

const float W = 1200.0f;
const float H = 800.0f;

float DEGTORAD = 0.017453f;


class asteroid : public Entity
{
public:
    asteroid()
    {
        Randomizer r(-4.0f, 8.0f);
        dx = r.roll(); //float rand
        dy = r.roll(); //float rand
        name = "asteroid";
    }

    void update()
    {
        x += dx;
        y += dy;

        if (x > W) x = 0;  if (x < 0) x = W;
        if (y > H) y = 0;  if (y < 0) y = H;
    }
};


class bullet : public Entity
{
public:
    bullet()
    {
        name = "bullet";
    }

    void  update()
    {
        dx = cos(angle * DEGTORAD) * 6.0f;
        dy = sin(angle * DEGTORAD) * 6.0f;
        x += dx;
        y += dy;

        if (x > W || x<0 || y>H || y < 0) life = 0;
    }

};


class player : public Entity
{
public:
    bool thrust;
    int score = 0;

    player()
    {
        name = "player";
        life = 3;
    }

    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2f;
            dy += sin(angle * DEGTORAD) * 0.2f;
        }
        else
        {
            dx *= 0.99f;
            dy *= 0.99f;
        }

        float maxSpeed = 15.0f;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;

        if (x > W) x = 0; if (x < 0) x = W;
        if (y > H) y = 0; if (y < 0) y = H;
    }
};

int main()
{
    Randomizer r360(0.0f, 360.0f);
    Randomizer rH(0.0f, W);
    Randomizer rW(0.0f, H);

    sf::Texture t1, t3, t4, t5, t6, t7;
    t1.loadFromFile("images/spaceship.png");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    t1.setSmooth(true);

    Animation sExplosion(t3, 0.0f, 0.0f, 256.0f, 256.0f, 48, 0.5f);
    Animation sRock(t4, 0.0f, 0.0f, 64.0f, 64.0f, 16, 0.2f);
    Animation sRock_small(t6, 0.0f, 0.0f, 64.0f, 64.0f, 16, 0.2f);
    Animation sBullet(t5, 0.0f, 0.0f, 32.0f, 64.0f, 16, 0.8f);
    Animation sPlayer(t1, 40.0f, 0.0f, 40.0f, 40.0f, 1, 0.0f);
    Animation sPlayer_go(t1, 40.0f, 40.0f, 40.0f, 40.0f, 1, 0.0f);
    Animation sExplosion_ship(t7, 0.0f, 0.0f, 192.0f, 192.0f, 64, 0.5f);


    std::list<std::shared_ptr<Entity>> entities;

    for (int i = 0; i < 15; i++)
    {
        auto a = std::make_shared<asteroid>();
        a->settings(sRock, rW.roll(), rH.roll(), r360.roll(), 25);
        entities.push_back(a);
    }

    auto p = std::make_shared<player>();
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    auto vizualizer = new Vizualizer();

    while (vizualizer->isWork())
    {
        vizualizer->isClose();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            auto b = std::make_shared<bullet>();
            b->settings(sBullet, p->x, p->y, p->angle, 10);
            entities.push_back(b);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p->angle += 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  p->angle -= 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) p->thrust = true;
        else p->thrust = false;

        for (auto a : entities)
            for (auto b : entities)
            {
                if (a->name == "asteroid" && b->name == "bullet")
                    if (a->isCollide(b))
                    {
                        a->life = 0;
                        b->life = 0;
                        if (a->R > 15) p->score += 2;
                        else p->score++;
                        auto e = std::make_shared<Entity>();
                        e->settings(sExplosion, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);


                        for (int i = 0; i < 2; i++)
                        {
                            if (a->R > 15) {
                                auto e = std::make_shared<asteroid>();
                                e->settings(sRock_small, a->x, a->y, r360.roll(), 15); //all to float
                                entities.push_back(e);
                            }
                        }

                    }

                if (a->name == "player" && b->name == "asteroid")
                    if (a->isCollide(b))
                    {
                        b->life = 0;
                        a->life--;
                        auto e = std::make_shared<Entity>();
                        e->settings(sExplosion_ship, a->x, a->y);
                        e->name = "explosion";
                        entities.push_back(e);

                        p->settings(sPlayer, W / 2, H / 2, 0, 20);
                        p->dx = 0; p->dy = 0;
                    }
            }


        if (p->thrust)  p->anim = sPlayer_go;
        else   p->anim = sPlayer;


        for (auto e : entities)
            if (e->name == "explosion")
                if (e->anim.isEnd()) { e->life = 0; }

        Randomizer r(0, 150);
        if (static_cast <int >(r.roll()) == 0) 
        {
            auto a = std::make_shared<asteroid>();
            a->settings(sRock, 0, rH.roll(), r360.roll(), 25); 
            entities.push_back(a);
        }

        for (auto i = entities.begin(); i != entities.end();)
        {
            auto e = *i;

            e->update();
            e->anim.update();

            if (e->life == false) { i = entities.erase(i); }
            else i++;
        }
        std::string s;
        s = "Life: " + std::to_string(p->life) + "\n" +
            "Score: " + std::to_string(p->score) + "\n";
        vizualizer->draw(entities, s, (p->life > 0));
    }
    return 0;
}