#include <SFML/Graphics.hpp>
#include <list>
#include <random>
#include <vector>
#include <string>
#include <memory>
#include <unordered_set>

#include <SFML/Graphics.hpp>
using namespace sf;

template <class Parent, class Children, class Deleter>
std::unique_ptr<Parent, Deleter> static_unique_pointer_cast(std::unique_ptr<Children, Deleter>&& r) {
    auto p = static_cast<Parent*>(r.release());
    return std::unique_ptr<Parent, Deleter>(p, std::move(r.get_deleter()));
}

template<class Parent, class Children, class Deleter>
std::unique_ptr<Children, Deleter> dynamic_unique_ptr_cast(std::unique_ptr<Parent, Deleter>&& r) {
    if (Children* res = dynamic_cast<Children*>(r.get())) {
        r.release();
        return std::unique_ptr<Children, Deleter>(res, std::move(r.get_deleter()));
    }
    return std::unique_ptr<Children, Deleter>(nullptr, r.get_deleter());
}

const float DEGTORAD = 0.017453f;

enum Constants
{
    width = 1200,
    heigth = 800,
};

class Animation
{
public:
    Animation() {}

    Animation(Texture& t, int x0, int y0, int width, int height, int count, float Speed)
    {
        currentFrame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(IntRect(x0 + i * width, y0, width, height));

        sprite.setTexture(t);
        sprite.setOrigin(width / 2, height / 2);
        sprite.setTextureRect(frames[0]);
    }


    void update()
    {
        currentFrame += speed;
        int n = frames.size();
        if (currentFrame >= n) currentFrame -= n;
        if (n > 0) 
            sprite.setTextureRect(frames[static_cast <int> (currentFrame)]);
    }

    bool isEnd()
    {
        return currentFrame + speed >= frames.size();
    }

    Sprite& getSprite() {
        return sprite;
    }

private:
    float currentFrame;
    float speed;
    Sprite sprite;
    std::vector<IntRect> frames;
};


class Entity
{
public:
    Entity()
    {
        isLife = true;
    }

    void settings(Animation& a, int X, int Y, float Angle = 0, int R = 1)
    {
        anim = a;
        x = X; y = Y;
        angle = Angle;
        radius = R;
    }

    virtual void update() {};

    void draw(RenderWindow& app)
    {
        anim.getSprite().setPosition(x, y);
        anim.getSprite().setRotation(angle + 90);
        app.draw(anim.getSprite());
        
        CircleShape circle(radius);
        circle.setFillColor(Color(255, 0, 0, 170));
        circle.setPosition(x, y);
        circle.setOrigin(radius, radius);
        app.draw(circle);
    }

    float getX() {
        return x;
    }

    float getY() {
        return y;
    }

    int getRadius() {
        return radius;
    }

    void setRadius(int r) {
        radius = r;
    }

    float getAngle() {
        return angle;
    }

    void changeAngle(float f) {
        angle += f;
    }

    std::string getName() {
        return name;
    }

    void setName(std::string n) {
        name = n;
    }

    bool getIsLife() {
        return isLife;
    }

    void setIsLife(bool b) {
        isLife = b;
    }

    Animation& getAnim() {
        return anim;
    }

    virtual ~Entity() {};

protected:
    float x, y, dx, dy, angle;
    std::string name;
    bool isLife;
    Animation anim;
private:
    int radius;
};


class asteroid : public Entity
{
public:
    asteroid()
    {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
    }

    void update()
    {
        x += dx;
        y += dy;

        if (x > Constants::width)
            x = 0;  
        if (x < 0) 
            x = Constants::width;
        if (y > Constants::heigth)
            y = 0;  
        if (y < 0) 
            y = Constants::heigth;
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
        dx = cos(angle * DEGTORAD) * 6;
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%7-3;  /*try this*/
        x += dx;
        y += dy;

        if (x > Constants::width || x < 0 || y > Constants::heigth || y < 0) isLife = 0;
    }

};


class player : public Entity
{
public:
    player()
    {
        name = "player";
    }

    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        else
        {
            dx *= 0.99;
            dy *= 0.99;
        }

        int maxSpeed = 15;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;

        if (x > Constants::width)
            x = 0;
        if (x < 0)
            x = Constants::width;
        if (y > Constants::heigth)
            y = 0;
        if (y < 0)
            y = Constants::heigth;
    }

    bool getThrust() {
        return thrust;
    }

    void setThrust(bool b) {
        thrust = b;
    }

    void setDX(float f) {
        dx = f;
    }

    void setDY(float f) {
        dy = f;
    }

    void setAnim(Animation a) {
        anim = a;
    }

private:
    bool thrust;
};


bool isCollide(std::unique_ptr<Entity>&& a, std::unique_ptr<Entity>&& b)
{
    return (b->getX() - a->getX()) * (b->getX() - a->getX()) +
        (b->getY() - a->getY()) * (b->getY() - a->getY()) <
        (a->getRadius() + b->getRadius()) * (a->getRadius() + b->getRadius());
}

std::unique_ptr<player> dynamic_unique_ptr_cast_sp(std::unique_ptr<Entity>&& r) {
    player* res = dynamic_cast<player*>(r.get());
    r.release();
    return std::unique_ptr<player>(res);
}


int main()
{
    srand(time(0));

    RenderWindow app(VideoMode(Constants::width, Constants::heigth), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4, t5, t6, t7;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sExplosion(t3, 0, 0, 256, 256, 48, 0.5);
    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    Animation sRock_small(t6, 0, 0, 64, 64, 16, 0.2);
    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);
    Animation sPlayer(t1, 40, 0, 40, 40, 1, 0);
    Animation sPlayer_go(t1, 40, 40, 40, 40, 1, 0);
    Animation sExplosion_ship(t7, 0, 0, 192, 192, 64, 0.5);


    std::list<std::unique_ptr<Entity>> entities;

    for (int i = 0; i < 15; i++)
    {
        std::unique_ptr<Entity> a(new asteroid());
        a->settings(sRock, rand() % Constants::width, rand() % Constants::heigth, rand() % 360, 25);
        entities.push_back(a);
    }

    std::unique_ptr<Entity> p(new player());
    p->settings(sPlayer, 200, 200, 0, 20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Space)
                {
                    std::unique_ptr<Entity> b(new bullet());
                    b->settings(sBullet, p->getX(), p->getY(), p->getAngle(), 10);
                    entities.push_back(b);
                }
        }

        std::unique_ptr<player> p1 = dynamic_unique_ptr_cast_sp(std::move(p));

        if (Keyboard::isKeyPressed(Keyboard::Right)) p1 ->changeAngle(3);
        if (Keyboard::isKeyPressed(Keyboard::Left))  p1 ->changeAngle(-3);
        if (Keyboard::isKeyPressed(Keyboard::Up)) p1->setThrust(true);
        else p1->setThrust(false);


        for (auto&& a : entities)
            for (auto&& b : entities)
            {
                if (a->getName() == "asteroid" && b->getName() == "bullet")
                    if (isCollide(std::move(a), std::move(b)))
                    {
                        a->setIsLife(false);
                        b->setIsLife(false);

                        std::unique_ptr<Entity> e(new Entity());
                        e->settings(sExplosion, a->getX(), a->getY());
                        e->setName("explosion");
                        entities.push_back(e);


                        for (int i = 0; i < 2; i++)
                        {
                            if (a->getRadius() == 15) continue;
                            std::unique_ptr<Entity> e(new asteroid());
                            e->settings(sRock_small, a->getX(), a->getY(), rand() % 360, 15);
                            entities.push_back(e);
                        }

                    }

                if (a->getName() == "player" && b->getName() == "asteroid")
                    if (isCollide(std::move(a), std::move(b)))
                    {
                        b->setIsLife(false);

                        std::unique_ptr<Entity> e(new Entity());
                        e->settings(sExplosion_ship, a->getX(), a->getY());
                        e->setName("explosion");
                        entities.push_back(e);

                        p->settings(sPlayer, Constants::width / 2, Constants::heigth / 2, 0, 20);
                        p1->setDX(0); p1->setDY(0);
                    }
            }


        if (p1->getThrust())  p1->setAnim(sPlayer_go);
        else   p1->setAnim(sPlayer);


        for (auto&& e : entities)
            if (e->getName() == "explosion")
                if (e->getAnim().isEnd()) e->setIsLife(false);

        if (rand() % 150 == 0)
        {
            std::unique_ptr<Entity> a(new asteroid());
            a->settings(sRock, 0, rand() % Constants::heigth, rand() % 360, 25);
            entities.push_back(a);
        }

        /*for (auto&& i = entities.begin(); i != entities.end();)
        {
            auto e = std::move(i);
            e->get()->update();
            e->get()->getAnim().update();

            if (e->get()->getIsLife() == false) { i = entities.erase(i); delete e; }
            else i++;
        }*/

        /*for (auto&& e : entities) {
            e->update();
            e->getAnim().update();
        }*/

        //////draw//////
        app.draw(background);
        //for (auto&& i : entities) i->draw(app);
        app.display();
    }

    return 0;
}