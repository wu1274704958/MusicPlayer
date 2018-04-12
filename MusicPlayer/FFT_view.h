#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

namespace fv {
	class FFT_View : public sf::Shape
	{
	public:
		FFT_View(float *pdata, int fft_size) {
			m_pdata = pdata;
			m_fft_size = fft_size;
		}
		virtual ~FFT_View()
		{

		}
		
		virtual void updatePoint(float l,float h) = 0;
		virtual void init() = 0;

		virtual std::size_t getPointCount() const override
		{
			return m_ps.size();
		}
		virtual sf::Vector2f getPoint(std::size_t index) const override
		{
			return m_ps[index];
		}

	protected:
		float *m_pdata;
		int m_fft_size;

		std::vector<sf::Vector2f> m_ps_copy;
		std::vector<sf::Vector2f> m_ps;
	
	};
}
