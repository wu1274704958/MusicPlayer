#include "FFT2_View.h"



fv::FFT2_View::FFT2_View(float * pdata, int size,float w,float h) : FFT_View(pdata, size)
{
	m_w = w;
	m_h = h;
	m_unit_w =  w / size;
	m_x = -(w / 2.f);
	m_y = h / 2;
	init();
}

void fv::FFT2_View::init()
{

	this->setFillColor(sf::Color(0, 0, 0, 0));
	this->setOutlineColor(sf::Color(0, 255, 255));
	this->setOutlineThickness(1.6);

	for (int i = 0; i < m_fft_size; ++i)
	{
		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w) , m_y));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w) , m_y));

		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w) + m_unit_w, m_y));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w)  + m_unit_w, m_y));
	}

	for (int i = m_fft_size - 1; i >= 0; --i)
	{
		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w) + m_unit_w, m_y - m_h));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w) + m_unit_w, m_y - m_h));
		
		m_ps.push_back(sf::Vector2f(m_x + (i * m_unit_w), m_y - m_h));
		m_ps_copy.push_back(sf::Vector2f(m_x + (i * m_unit_w), m_y - m_h));
	}
	update();
}

void fv::FFT2_View::updatePoint(float l, float h)
{
	int s = m_fft_size * 4 - 1;
	for (int i = 0; i < m_fft_size; i++)
	{
		sf::Transform trans;
		//printf("%f\n", -(sqrtf(m_pdata[i]) * h * 100.0f));
		trans.scale(1.0f , (sqrtf(m_pdata[i]) * h * 30.f) );

		m_ps[i * 2] = trans.transformPoint(m_ps_copy[i * 2]);
		m_ps[i * 2 + 1] = trans.transformPoint(m_ps_copy[i * 2 + 1]);

		sf::Transform trans2;
		//printf("%f\n", (sqrtf(m_pdata[i]) * l * 100.0f) );
		trans2.scale(1.0f, (sqrtf(m_pdata[i]) * l * 30.0f) );

		m_ps[s] = trans2.transformPoint(m_ps_copy[s]);
		m_ps[s - 1] = trans2.transformPoint(m_ps_copy[s - 1]);

		s -= 2;
	}
	update();
}




