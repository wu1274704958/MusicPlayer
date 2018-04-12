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
		
		virtual void updatePoint() = 0;
		virtual void init() = 0;
	protected:
		float *m_pdata;
		int m_fft_size;

		std::vector<sf::Vector2f> m_ps_copy;
		std::vector<sf::Vector2f> m_ps;
	
	};
}
