#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>
#include <chrono>
#include <array>
#include <memory>

enum Const {
	w = 30,
	h = 20,
	n = 300000,
	l = 16
};

class Model {
public:
	std::array<std::array <int, Const::w + 2>, Const::h + 2> current;

	Model() {
		//std::fill(current.begin(), current.end(), 0);
		for (int i = 0; i < Const::h + 2; ++i) {
			for (int j = 0; j < Const::w + 2; ++j) {
				current[i][j] = 0;
			}
		}
		std::uniform_int_distribution<std::mt19937::result_type> d30(1, 30);
		std::uniform_int_distribution<std::mt19937::result_type> d20(1, 20);
		std::mt19937 gen(rd());
		for (int i = 0; i < n; ++i) {
			current[d20(gen)][d30(gen)]++;
		}
	}

	void update() {
		std::uniform_int_distribution<std::mt19937::result_type> d(0, 8);
		std::mt19937 gen(rd());
		//std::fill(updated.begin(), updated.end(), 0);
		for (int i = 0; i < Const::h + 2; ++i) {
			for (int j = 0; j < Const::w + 2; ++j) {
				updated[i][j] = 0;
			}
		}
		int a;
		for (int i = 1; i < Const::h + 1; ++i) {
			for (int j = 1; j < Const::w + 1; ++j) {
				for (int k = 0; k < current[i][j]; ++k) {
					a = d(gen);
					updated[i - 1 + a / 3][j - 1 + a % 3]++;
				}
			}
		}
		current = updated;
	}
private:
	std::random_device rd;
	std::array<std::array <int, Const::w + 2>, Const::h + 2> updated;
};

class View {
public:
	View() {
		int step = (int)(255 / 22);
		for (int i = 0; i < 21; ++i) {
			colors.push_back(sf::Color(0, 0, step * i));
		}
	}
	void draw(sf::RenderWindow& app, std::array<std::array <int, Const::w + 2>, Const::h + 2>& a) {
		int c;
		for (int i = 1; i < Const::h + 1; ++i) {
			for (int j = 1; j < Const::w + 1; ++j) {
				sf::RectangleShape r;
				r.setSize(sf::Vector2f(16, 16));
				r.setPosition((float)(i * 16), (float)(j * 16));
				c = (int)(a[i][j] / 50);
				if (c > 21)
					c = 21;
				r.setFillColor(colors[c]);
				app.draw(r);
			}
		}
	}
private:
	std::vector <sf::Color> colors;
};

int main() {
	sf::RenderWindow app(sf::VideoMode(Const::h * 16, Const::w * 16), "Brown");
	
	std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

	const std::chrono::microseconds delay(50000);

	std::chrono::microseconds timer(0);
	
	std::unique_ptr<Model> m(new Model());
	std::unique_ptr<View> v(new View());
	while (app.isOpen())
	{
		timer += std::chrono::duration_cast <std::chrono::microseconds> (
			std::chrono::steady_clock::now() - time_point);

		time_point = std::chrono::steady_clock::now();

		sf::Event event;

		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				app.close();
			}
		}
		if (timer > delay)
		{
			timer = std::chrono::microseconds(0);
			m->update();
			v->draw(app, m->current);
			app.display();
		}
	}
	return 0;
}
