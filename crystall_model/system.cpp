#include "system.hpp"

// =============================================================================

void System::initialize()
{
	/*
	const auto size = m_particles.size();

	for (auto i = 0U; i < size; ++i)
	{
		m_links.push_back(Link(particle(i), particle((i + 1) % size), 1.0f));
	}
	*/
	int k = 0;
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			m_links.push_back(Link(particle(i, j), particle(i, j + 1), 1.0f));
			m_links.push_back(Link(particle(j, i), particle(j + 1, i), 1.0f));
			k += 2;
		}
	}

	for (int j = 0; j < 9; ++j) {
		m_links.push_back(Link(particle(9, j), particle(9, j + 1), 1.0f));
		m_links.push_back(Link(particle(j, 9), particle(j + 1, 9), 1.0f));
		k += 2;
	}
	//std::cout << k << std::endl;
	k = 0;

	for (int j = 1; j < 9; ++j) {
		int i = 0;
		int j1 = j;
		while (j1 <  9) {
			m_links.push_back(Link(particle(i, j1), particle(i + 1, j1 + 1), 1.0f));
			m_links.push_back(Link(particle(j1, i), particle(j1 + 1, i + 1), 1.0f));
			++j1;
			++i;
			k += 2;
		}
	}

	for (int i = 0; i < 9; ++i)
		m_links.push_back(Link(particle(i, i), particle(i + 1, i + 1), 1.0f));
	k += 9;
	//std::cout << k << std::endl;
	k = 0;

	for (int j = 1; j < 9; ++j) {
		int i = 0;
		int j1 = j;
		while (j1 > 0) {
			m_links.push_back(Link(particle(i, j1), particle(i + 1, j1 - 1), 1.0f));
			m_links.push_back(Link(particle(j1, i), particle(j1 - 1, i + 1), 1.0f));
			--j1;
			++i;
			k += 2;
		}
	}

	for (int i = 0; i < 9; ++i)
		m_links.push_back(Link(particle(i, 9 - i), particle(i + 1, 9 - i), .0f));
	k += 9;
	//std::cout << k << std::endl;

}

// =============================================================================

void System::push(const sf::Vector2f force) const noexcept
{
	for (auto i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j)
			m_particles[i][j]->move(force);
	}
}

// =============================================================================

void System::update() const noexcept
{
	for (auto i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j)
		{
			m_particles[i][j]->move(0.25f); //gravitetion

			if (m_particles[i][j]->position().y + m_particles[i][j]->radius() > m_max_point.y)
			{
				m_particles[i][j]->set_y(m_max_point.y - m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().y - m_particles[i][j]->radius() < m_min_point.y)
			{
				m_particles[i][j]->set_y(m_min_point.y + m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().x + m_particles[i][j]->radius() > m_max_point.x)
			{
				m_particles[i][j]->set_x(m_max_point.x - m_particles[i][j]->radius());
			}

			if (m_particles[i][j]->position().x - m_particles[i][j]->radius() < m_min_point.x)
			{
				m_particles[i][j]->set_x(m_min_point.x + m_particles[i][j]->radius());
			}
		}
	}

	for (auto i = 0U; i < m_links.size(); ++i)
	{
		m_links[i].update();
	}

}

// =============================================================================
