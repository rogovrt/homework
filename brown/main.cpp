#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <array>

namespace C {
	const int num_cells = 30;
	const int num_points = 300000;
	const float cell_size = 16.0f;
}

class Ranodmizer {
public:
	Ranodmizer(int m) : gen(rd()), dis(0, m) {} // тут для распределения к-р копирования, можно просто (0, m)
	int roll() { // const x2
		return dis(gen);
	}
private:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<int> dis;
};

class Model {
public:
	Model() {
		Ranodmizer r(C::num_cells - 1);
		for (int i = 0; i < C::num_cells; ++i) {
			for (int j = 0; j < C::num_cells; ++j) {
				cells[i][j] = 0;
			}
		}
		for (int i = 0; i < C::num_points; ++i) {
			cells[r.roll()][r.roll()]++;
		}
	}

	void update() {
		int a, i_new, j_new;
		Ranodmizer r8(8);
		for (int i = 0; i < C::num_cells; ++i) {
			for (int j = 0; j < C::num_cells; ++j) {
				int num = cells[i][j];
				for (int k = 0; k < num; ++k) {
					a = r8.roll();
					i_new = i - 1 + a / 3;
					j_new = j - 1 + a % 3;
					if (i_new < 0) i_new++;
					if (j_new < 0) j_new++;
					if (i_new > 29) i_new--;
					if (j_new > 29) j_new--;
					cells[i_new][j_new]++;
					cells[i][j]--;
				}
			}
		}
	}

	auto& getCells() {
		return cells;
	}
private:
	std::array<std::array <int, C::num_cells>, C::num_cells> cells;
};

class View {
public:
	View() {
		int step = 255 / 22;
		for (int i = 0; i < 21; ++i) {
			colors.push_back(sf::Color(0, 0, step * i));
		}
	}
	void draw(sf::RenderWindow& app, std::array<std::array <int, C::num_cells>, C::num_cells>& cells) {
		int c;
		for (int i = 0; i < C::num_cells; ++i) {
			for (int j = 0; j < C::num_cells; ++j) {
				sf::RectangleShape r;
				r.setSize(sf::Vector2f(C::cell_size, C::cell_size));
				r.setPosition(static_cast <float> (i * C::cell_size), static_cast <float> (j * C::cell_size));
				c = static_cast <int> (cells[i][j] / 50);
				if (c > 20) c = 20;
				if (c < 0) c = 0;
				r.setFillColor(colors[c]);
				app.draw(r);
			}
		}
	}
private:
	std::vector <sf::Color> colors;
};

class Visualizer {
public:
	Visualizer() {}
	void run() {
		sf::RenderWindow app(sf::VideoMode(C::num_cells * C::cell_size, C::num_cells * C::cell_size), "Brown");

		std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

		const std::chrono::microseconds delay(50000);

		std::chrono::microseconds timer(0);

		Model m;
		View v;
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
				m.update();
				v.draw(app, m.getCells());
				app.display();
			}
		}
	}
};

int main() {
	Visualizer v;
	v.run();
	return 0;
}

