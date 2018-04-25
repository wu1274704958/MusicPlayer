//
// Created by wws on 17-7-19.
//

#include "GetFileName.h"
#include <Windows.h>
#include "SupportFile.h"

void GetFileName::getFileNameA(std::vector<MMFile> &v, const char *pChar)
{
	WIN32_FIND_DATAW ffd;
	HANDLE han = nullptr;

	if (!pChar) return ;
	wchar_t *pszBuf = NULL;
	int needWChar = MultiByteToWideChar(CP_ACP, 0, pChar, -1, NULL, 0);
	if (needWChar > 0) {
		pszBuf = new wchar_t[needWChar + 1];
		ZeroMemory(pszBuf, (needWChar + 1) * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, pChar, -1, pszBuf, needWChar);
	}

	getFileNameW(v, pszBuf);
    
	if (pszBuf)
		delete[] pszBuf;
}

void GetFileName::getFileNameW(std::vector<MMFile>& v, const wchar_t * d)
{
	WIN32_FIND_DATAW ffd;
	HANDLE han = nullptr;

	std::wstring dir(d);

	dir += L"\\*";

	if ((han = FindFirstFile(dir.c_str(), &ffd)) != nullptr)
	{
		while (FindNextFile(han, &ffd))
		{
			if (lstrcmpW(ffd.cFileName, L".") == 0 || lstrcmpW(ffd.cFileName, L"..") == 0)
				continue;
			//std::wcout << ffd.dwFileAttributes << "  " << ffd.cFileName << std::endl;
			MMFile tempFile(ffd.dwFileAttributes == 16 ? MMFile::TYPE_DIR : MMFile::TYPE_FILE, ffd.cFileName, d);
			if (SupportFile::isSupport(tempFile.getSuffix()) || tempFile.getType() == MMFile::TYPE_DIR)
				v.push_back(std::move(tempFile));
		}
	}

}
