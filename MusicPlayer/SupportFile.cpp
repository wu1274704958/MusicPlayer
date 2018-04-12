#include "SupportFile.h"
#include <cstring>
#include <Windows.h>

const wchar_t  * const SupportFile::supportList[SupportFile::supportNum] = {
	L".mp3",
	L".flac",
	L".wav"
};

bool SupportFile::isSupport(const wchar_t *other)
{
	for (int i = 0; i < supportNum; i++)
	{
		if (lstrcmpW(supportList[i],other) == 0)
			return true;
	}
	return false;
	
}
