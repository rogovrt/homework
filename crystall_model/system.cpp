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
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 9; ++j) {
			m_links.push_back(Link(particle(i, j), particle(i, j + 1), 1.0f));
			m_links.push_back(Link(particle(j, i), particle(j, i + 1), 1.0f));
		}
	}
	for (int i = 1; i < 9; ++i) {
		int i1 = i;
		int j = 0;
		while (i1 <  10) {
			m_links.push_back(Link(particle(i1, j), particle(i1 + 1, j + 1), 1.0f));
			m_links.push_back(Link(particle(j, i1), particle(j + 1, i1 + 1), 1.0f));
			++j;
			++i1;
		}
	}
	for (int i = 0; i < 9; ++i)
		m_links.push_back(Link(particle(i, i), particle(i + 1, i + 1), 1.0f));
	for (int i = 1; i < 9; ++i) {
		int i1 = i;
		int j = 0;
		while (i1 > -1) {
			m_links.push_back(Link(particle(i1, j), particle(i1 - 1, j + 1), 1.0f));
			--i1;
			++j;
		}
		int i2 = i;
		j = 9;
		while (i2 < 10) {
			m_links.push_back(Link(particle(j, i2), particle(j - 1, i2 + 1), 1.0f));
			++i2;
			--j;
		}
	}
}

// =============================================================================

void System::push(const sf::Vector2f force) const noexcept
{
	for (auto i = 0U; i < m_particles.size(); ++i) {
		for (int j = 0; j < 10; ++j)
			m_particles[i][j]->move(force);
	}
}

// =============================================================================

void System::update() const noexcept
{
	for (auto i = 0U; i < m_particles.size(); ++i) {
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
