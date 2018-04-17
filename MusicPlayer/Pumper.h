#pragma once

#include <memory>
#include <vector>
#include <stack>
#include <functional>
#include "MMFile.h"
#include "MusicPlayer.h"


namespace fv {
	class Pumper
	{
	public:
		enum PUMP_MODE
		{
			NONE = 1,
			LOOP,
			RAND
		};

		Pumper(std::stack<std::shared_ptr<std::vector<MMFile>>> &root, MusicPlayer &player);
		~Pumper();

		void pump();
		PUMP_MODE getMode();
		void setMode(PUMP_MODE mo);
		int getIndex();
		void setIndex(int ind);
		bool getPumpDir();
		void setPumpDir(bool pump_dir);
		void setFillMusicFunc(std::function<void()> f);
		

	private:
		void rand();
		void loop();
		void none();

		template<size_t N>
		void all_templet();

		std::function<void()> fill_music_func;

		PUMP_MODE m_mode;
		std::stack<std::shared_ptr<std::vector<MMFile>>> &m_root;
		MusicPlayer &m_player;
		int m_index;
		bool pump_dir;
	};
}
