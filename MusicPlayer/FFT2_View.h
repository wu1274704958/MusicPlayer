#pragma once

#include "FFT_view.h"

namespace fv {
	class FFT2_View : public FFT_View
	{
	private:
		float m_w;
		float m_unit_w;
		float m_x;
		float m_y;
		float m_h;

	public:
		FFT2_View(float *pdata,int size,float w = 700.f,float h = 20.0f);
		virtual ~FFT2_View() = default;

		virtual void updatePoint(float l, float h) override;
		virtual void init() override;
	};
}

