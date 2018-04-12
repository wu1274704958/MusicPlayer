#pragma once
#include "FFT_view.h"

namespace fv{
	class FFT1_View : public FFT_View 
	{
	private:
		float a;
		float rx;
		float ry;
		float o;
		float x0;
		float y0;
		static const float pi;

	public:
		FFT1_View(float *pdata, int size, float radius = 164.f) : FFT_View(pdata,size)
		{
			this->rx = radius;
			this->ry = radius;
			this->o = radius / 180.0f;
			this->a = 360.0f;

			x0 = 0.f;
			y0 = 0.f;

			init();
		}
		virtual void init() override;
		virtual void updatePoint(float l, float h) override;
		virtual ~FFT1_View() = default;
	};
}