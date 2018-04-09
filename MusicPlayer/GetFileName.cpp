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

	std::wstring dir(pszBuf);

	

	dir += L"\\*";
    
	if ((han = FindFirstFile(dir.c_str(), &ffd)) != nullptr)
	{
		while(FindNextFile(han, &ffd))
		{
			if (lstrcmpW(ffd.cFileName, L".") == 0 || lstrcmpW(ffd.cFileName, L"..") == 0)
				continue;
			std::cout << ffd.dwFileAttributes << "  " << ffd.cFileName <<  std::endl;
			MMFile tempFile(ffd.dwFileAttributes, ffd.cFileName, pszBuf);
			if (SupportFile::isSupport(tempFile.getSuffix()) || tempFile.getType() == 16)
				v.push_back(tempFile);
			
		}
	}
    
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
			std::wcout << ffd.dwFileAttributes << "  " << ffd.cFileName << std::endl;
			MMFile tempFile(ffd.dwFileAttributes, ffd.cFileName, d);
			if (SupportFile::isSupport(tempFile.getSuffix()) || tempFile.getType() == 16)
				v.push_back(tempFile);
		}
	}

}
