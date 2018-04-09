#pragma once
#include <SFML\Graphics.hpp>

namespace fv{
	class FFT1_View : public sf::Shape 
	{
		float *pdata;
		int fft_size;
		float m_radius;
		static const float pi;

		std::vector<sf::Vector2f> m_ps_copy;
		std::vector<sf::Vector2f> m_ps;
		
		

	public:
		FFT1_View(float *pdata, int size, float radius = 164.f) 
		{
			this->pdata = pdata;
			this->fft_size = size;
			this->m_radius = radius;

			for (int i = 0; i < fft_size; i++)
			{
				float angle = i * 2 * pi / fft_size - pi / 2;
				float x = std::cos(angle) * m_radius;
				float y = std::sin(angle) * m_radius;

				m_ps_copy.push_back(sf::Vector2f(x,y));
				m_ps.push_back(sf::Vector2f(x, y));
			}
		}
		void updatePoint();
		virtual std::size_t getPointCount() const;
		virtual sf::Vector2f getPoint(std::size_t index) const;
	};
}