#include "Pumper.h"
#include "GetFileName.h"
#include <ctime>
#include <stdio.h>



fv::Pumper::Pumper(std::stack<std::shared_ptr<std::vector<MMFile>>> &root, MusicPlayer &player) : m_root(root),m_player(player)
{
	m_mode = NONE;
	pump_dir = false;
	m_index = 0;

	
}


fv::Pumper::~Pumper()
{
	
}

void fv::Pumper::pump()
{
	switch (m_mode)
	{
	case NONE:
		//none();
		return;
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

void fv::Pumper::rand()
{
	if (!m_root.top()->empty())
	{
		srand((unsigned)time(NULL));
		m_index = ::rand() % m_root.top()->size();

		if (m_root.top()->at(m_index).getType() == 16 && pump_dir && (bool)fill_music_func)
		{
			std::shared_ptr<std::vector<MMFile>> temp = std::make_shared<std::vector<MMFile>>();
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

void fv::Pumper::loop()
{
	if (!m_root.top()->empty())
	{
		if (m_index >= m_root.top()->size())
		{
			m_index = 0;
		}
		if (m_root.top()->at(m_index).getType() == 16 && pump_dir && (bool)fill_music_func)
		{
			std::shared_ptr<std::vector<MMFile>> temp = std::make_shared<std::vector<MMFile>>();
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

void fv::Pumper::none()
{

}
