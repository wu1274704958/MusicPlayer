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
		void init() override;
		void updatePoint() override;
		virtual std::size_t getPointCount() const override;
		virtual sf::Vector2f getPoint(std::size_t index) const override;
		virtual ~FFT1_View() = default;
	};
}