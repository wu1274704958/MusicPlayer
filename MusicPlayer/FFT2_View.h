#pragma once

#include "FFT_view.h"

namespace fv {
	class FFT2_View : public FFT_View
	{
	private:

	public:
		FFT2_View(float *pdata,int size);
		virtual ~FFT2_View() = default;
	};
}

