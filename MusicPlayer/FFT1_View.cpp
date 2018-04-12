#include "FFT1_View.h"
#include <stdio.h>

const float fv::FFT1_View::pi = 3.141592654f;

void fv::FFT1_View::init()
{

	this->setFillColor(sf::Color(11, 23, 70));
	this->setOutlineColor(sf::Color(0, 199, 140));
	this->setOutlineThickness(3.0);

	float angle = (360.0f - a) / 2.0f;
	float ang_zl = a / m_fft_size;
	for (int i = 0; i < m_fft_size; i++)
	{
		float x = x0 + rx * sinf(angle *  pi / 180);
		float y = y0 + ry * cosf(angle * pi / 180);

		angle += ang_zl;

		m_ps_copy.push_back(sf::Vector2f(x, -y));
		m_ps.push_back(sf::Vector2f(x, -y));
	}
}

void fv::FFT1_View::updatePoint(float l, float h)
{
	this->setScale(l, h);
	for (int i = 0; i < m_fft_size; i++)
	{
		sf::Transform trans;
		//printf("%f", sqrtf(pdata[i]));
		trans.scale(1.0f + sqrtf(m_pdata[i]), 1.0f + sqrtf(m_pdata[i]) );
		m_ps[i] = trans.transformPoint(m_ps_copy[i]);
	}
	update();
}

