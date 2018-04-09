//
// Created by wws on 17-7-19.
//

#ifndef FIRST_GETFILENAME_H
#define FIRST_GETFILENAME_H

#include <iostream>
#include <vector>
#include "MMFile.h"


class GetFileName { 

public:
    static void getFileNameA(std::vector<MMFile> &v, const char *d);
	static void getFileNameW(std::vector<MMFile> &v, const wchar_t *d);
};


#endif //FIRST_GETFILENAME_H
