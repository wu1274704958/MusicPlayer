#pragma once
#include <bass.h>
#include"MMFile.h"

namespace sf {
	class Event;
}

namespace fv {
	class MusicPlayer
	{

	public:
		MusicPlayer(bool enable3d = false);
		~MusicPlayer();

		void playStream(const  MMFile& file, bool loop = false);
		void play(bool loop = false);
		void pause();
		void stop();
		void getData(void *p, int size);
		DWORD getLevel();
		QWORD getPosition(int flag);
		void setPosition(QWORD pos, int flag);
		int getActive();
		void cleanup();
		QWORD getLength();
		bool isOff();


		HSAMPLE getChan();
		bool isEnable3D();
		bool isSupport3D();

		void handleEvent(sf::Event& e);
	private:
		void cleanFX();

	private:
		HSTREAM chan = 0;
		QWORD chan_max_len = 0;
		bool IsEnable3D = false;
		bool IsSupport3D = false;
		HFX fx[10];
	};

}
