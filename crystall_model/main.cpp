#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>

#include "functions.hpp"
#include "system.hpp"

int main(int argc, char ** argv)
{
	sf::RenderWindow window(sf::VideoMode(800U, 600U), "PHYSICS");

	sf::Vector2f min_point( 10.0f,  10.0f);
	sf::Vector2f max_point(750.0f, 550.0f);

	sf::Vector2f center = (min_point + max_point) * 0.5f;

	using particle_t = System::particle_t;
	/*
	const auto R = length(max_point - min_point) * 0.1f;
	const auto pi = 3.141593f;
	const auto count = 50U;

	auto angle = 0.0f;

	const auto r = 2.5f;

	std::vector < particle_t > particles;

	for (auto i = 0U; i < count; ++i)
	{
		angle = 2.0f * pi / count * i;

		auto position = sf::Vector2f(std::cos(angle), std::sin(angle)) * R + center;

		particles.push_back(std::make_shared < Particle > (position, position,
			sf::Vector2f(0.0f, 10.0f), r));
	}
	*/

	const auto r = 2.5f;
	sf::Vector2f v(-45.0f, -45.0f);
	sf::Vector2f top_left = center + v;
	sf::Vector2f step_right(10.0f, 0.0f);
	sf::Vector2f step_down(0.0f, 10.0f);
	std::array < std::array<particle_t, 10>, 10 > particles;
	//set size of particles on 10*10
	for (int i = 0; i < 10; ++i) {
		sf::Vector2f current = top_left;
		for (int j = 0; j < 10; ++j) {
			current += step_right;
			particles[i][j] = std::make_shared <Particle> (current, current, sf::Vector2f(0.0f, 10.0f), r);
		}
		top_left += step_down;
	}

	System system(min_point, max_point, particles);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		system.update();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			system.push(sf::Vector2f(0.0f, -2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			system.push(sf::Vector2f(0.0f, 2.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			system.push(sf::Vector2f(-2.0f, 0.0f));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			system.push(sf::Vector2f(2.0f, 0.0f));
		}

		window.clear();
		
		for (auto i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				sf::CircleShape circle(2.0f * r);
			
				circle.setPosition(system.particle(i, j)->position()); //incorrect wotl causee of [][]

				circle.setFillColor(sf::Color::Red);

				window.draw(circle);
			}
		}
	
		window.display();
	}
	
	// system("pause");

	return EXIT_SUCCESS;
}
