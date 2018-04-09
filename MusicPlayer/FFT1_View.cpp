#include "FFT1_View.h"
#include <stdio.h>

const float fv::FFT1_View::pi = 3.141592654f;

void fv::FFT1_View::updatePoint()
{
	
	
	for (int i = 0; i < fft_size; i++)
	{
		sf::Transform trans;
		//printf("%f", sqrtf(pdata[i]));
		trans.scale(1.0f + sqrtf(pdata[i]), 1.0f + sqrtf(pdata[i]) );
		m_ps[i] = trans.transformPoint(m_ps_copy[i]);
	}
	update();
}

std::size_t fv::FFT1_View::getPointCount() const
{
	return m_ps.size();
}

sf::Vector2f fv::FFT1_View::getPoint(std::size_t index) const
{
	return m_ps[index];
}
