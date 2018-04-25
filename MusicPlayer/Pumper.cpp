#include "Pumper.h"
#include "GetFileName.h"
#include <ctime>
#include <stdio.h>



fv::Pumper::Pumper(std::stack<std::shared_ptr<std::vector<MMFile>>> &root, MusicPlayer &player) : m_root(root),m_player(player)
{
	m_mode = NONE;
	pump_dir = false;
	m_index = 0;
	srand((unsigned)time(NULL));
	next_music = nullptr;
}

void fv::Pumper::setNextMusic(const MMFile *nm)
{
	cleanup();
	next_music = new MMFile(*nm);
}

fv::Pumper::~Pumper()
{
	cleanup();
}

void fv::Pumper::pump()
{
	switch (m_mode)
	{
	case NONE:
		none();
		break;
	case LOOP:
		loop();
		break;
	case RAND:
		rand();
		break;
	default:
		none();
		break;
	}
}

fv::Pumper::PUMP_MODE fv::Pumper::getMode()
{
	return m_mode;
}

void fv::Pumper::setMode(PUMP_MODE mo)
{
	m_mode = mo;
	if(mo == RAND)
		srand((unsigned)time(NULL));
}

int fv::Pumper::getIndex()
{
	return m_index;
}

void fv::Pumper::setIndex(int ind)
{
	m_index = ind;
}

bool fv::Pumper::getPumpDir()
{
	return pump_dir;
}

void fv::Pumper::setPumpDir(bool pump_dir)
{
	this->pump_dir = pump_dir;
}

void fv::Pumper::setFillMusicFunc(std::function<void()> f)
{
	fill_music_func = f;
}

template<size_t N>
void fv::Pumper::all_templet()
{
	if (next_music)
	{
		m_player.playStream(*next_music);
		delete next_music;
		next_music = nullptr;
		return;
	}
	if constexpr(N == NONE)
	{
		return;
	}
	if (!m_root.top()->empty())
	{
		if constexpr(N == RAND)
		{
			m_index = ::rand() % m_root.top()->size();
		}
		else if constexpr(N == LOOP)
		{
			if (m_index >= m_root.top()->size())
			{
				m_index = 0;
			}
		}
		if (m_root.top()->at(m_index).getType() == MMFile::TYPE_DIR && pump_dir && (bool)fill_music_func)
		{
			std::shared_ptr<std::vector<MMFile>> temp = std::make_shared<std::vector<MMFile>>();
			temp->reserve(5);
			GetFileName::getFileNameW(*temp, m_root.top()->at(m_index).getAbsolutePath());
			m_root.push(temp);
			fill_music_func();
			m_index = 0;
		}
		else {
			m_player.playStream(m_root.top()->at(m_index));
			++m_index;
		}
	}
}

void fv::Pumper::rand()
{
	all_templet<RAND>();
}

void fv::Pumper::loop()
{
	all_templet<LOOP>();
}

void fv::Pumper::none()
{
	all_templet<NONE>();
}

void fv::Pumper::cleanup()
{
	if (next_music)
	{
		delete next_music;
		next_music = nullptr;
	}
}
