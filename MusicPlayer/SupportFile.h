#pragma once

class SupportFile {
	static const int supportNum = 3;
	static const  wchar_t * const supportList[supportNum];
	
public:
	static bool isSupport(const wchar_t *);
};