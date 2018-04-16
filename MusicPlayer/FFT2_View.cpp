#include "FFT2_View.h"



fv::FFT2_View::FFT2_View(float * pdata, int size,float w,float h) : FFT_View(pdata, size)
{
	m_w = w;
	m_h = h;
	m_unit_w =  w / size;
	m_x = -(w / 2.f);
	m_y = h + 200.f;
	init();
}

void fv::FFT2_View::init()
{

	this->setFillColor(sf::Color(0, 0, 0, 0));
	this->setOutlineColor(sf::Color(0, 255, 255));
	this->setOutlineThickness(3.0);

	for (int i = 0; i < m_fft_size; i++)
	{
		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w) , m_y));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w) , m_y));

		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w) + m_unit_w, m_y));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w)  + m_unit_w, m_y));
	}
	m_ps.push_back(sf::Vector2f(m_x + m_fft_size * m_unit_w , m_y + m_h));
	m_ps.push_back(sf::Vector2f(m_x, m_y + m_h));
}

void fv::FFT2_View::updatePoint(float l, float h)
{
	for (int i = 0; i < m_fft_size; i++)
	{
		sf::Transform trans;
		//printf("%f", sqrtf(pdata[i]));
		trans.scale(1.0f , 1.0f-(sqrtf(m_pdata[i]) * 4.20f) );

		m_ps[i * 2] = trans.transformPoint(m_ps_copy[i * 2]);
		m_ps[i * 2 + 1] = trans.transformPoint(m_ps_copy[i * 2 + 1]);
	}
	update();
}




