#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <array>

class Const {
public:
	const static int w = 30;
	const static int n = 300000;
};

class Ranodmizer {
public:
	Ranodmizer(int m) : gen(rd()), dis(std::uniform_int_distribution<>(0, m)) {}
	int roll() {
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
		Ranodmizer r(Const::w - 1);
		r8 = new Ranodmizer(8);
		for (int i = 0; i < Const::w; ++i) {
			for (int j = 0; j < Const::w; ++j) {
				cells[i][j] = 0;
			}
		}
		for (int i = 0; i < Const::n; ++i) {
			cells[r.roll()][r.roll()]++;
		}
	}

	void update() {
		int a, i_new, j_new;
		for (int i = 0; i < Const::w ; ++i) {
			for (int j = 0; j < Const::w; ++j) {
				int num = cells[i][j];
				for (int k = 0; k < num; ++k) {
					a = r8->roll();
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

	std::array<std::array <int, Const::w>, Const::w>& getCells() {
		return cells;
	}
private:
	std::array<std::array <int, Const::w>, Const::w> cells;
	Ranodmizer* r8;
};

class View {
public:
	View() {
		int step = static_cast <int> (255 / 22);
		for (int i = 0; i < 21; ++i) {
			colors.push_back(sf::Color(0, 0, step * i));
		}
	}
	void draw(sf::RenderWindow& app, const std::array<std::array <int, Const::w>, Const::w>& cells) {
		int c;
		for (int i = 0; i < Const::w; ++i) {
			for (int j = 0; j < Const::w; ++j) {
				sf::RectangleShape r;
				r.setSize(sf::Vector2f(16, 16));
				r.setPosition(static_cast <float> (i * 16), static_cast <float> (j * 16));
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

int main() {
	sf::RenderWindow app(sf::VideoMode(Const::w * 16, Const::w * 16), "Brown");
	
	std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

	const std::chrono::microseconds delay(50000);

	std::chrono::microseconds timer(0);
	
	auto m = new Model();
	auto v = new View();
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
			v->draw(app, m->getCells());
			app.display();
		}
	}
	return 0;
}
