#pragma once
#include <random>

class Randomizer {
public:
	Randomizer(float start, float quantity) : gen(rd()), dis(std::uniform_real_distribution<float>(start, quantity)) {}

	void reset(float start, float quantity) {
		dis = std::uniform_real_distribution <float>(start, quantity);
	}

	float roll() {
		return dis(gen);
	}
private:
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
};