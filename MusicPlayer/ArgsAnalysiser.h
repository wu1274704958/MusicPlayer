#pragma once
#include <tuple>


namespace ArgsAnalysiser {
	auto analysis(int argc, char **argv)
	{
		std::tuple<int, int, const char *,bool> tup(800, 600, "D:\\Music\\Big",false);//
		int w, h;
		if (argc >= 2)
		{
			w = atoi(argv[1]);
			if (w > 50)
			{
				std::get<0>(tup) = w;
			}
		}
		if (argc >= 3)
		{
			h = atoi(argv[2]);
			if (h > 50)
			{
				std::get<1>(tup) = h;
			}
		}

		if (argc >= 4)
		{
			std::get<2>(tup) = argv[3];
		}

		if (argc >= 5)
		{
			if (strcmp("true", argv[4]) == 0)
			{
				std::get<3>(tup) = true;
			}
			else
			{
				std::get<3>(tup) = false;
			}
		}
		
		return tup;
	}
}
