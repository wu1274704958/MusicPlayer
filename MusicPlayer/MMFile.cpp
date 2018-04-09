//
// Created by wws on 17-7-19.
//

#include "MMFile.h"
#include <memory>

MMFile::MMFile()
{
    this->type = -1;
    this->name = nullptr;
    this->path = nullptr;
    this->suffix = nullptr;
    this->absolutePath = nullptr;
}

MMFile::MMFile(int type,const wchar_t *name, const wchar_t *path)
{
    this->type = type;
    this->name = new std::wstring(name);
    this->path = new std::wstring(path);
    analysisSuffix();
    analysisAbsolutePath();
}

MMFile::MMFile(const MMFile &mmf)
{
    this->type = mmf.type;
    this->name = new std::wstring(*(mmf.name));
    this->path = new std::wstring(*(mmf.path));
    if(mmf.suffix != nullptr)
        this->suffix = new std::wstring(*(mmf.suffix));
    else
        analysisSuffix();
    if(mmf.absolutePath != nullptr)
        this->absolutePath = new std::wstring(*(mmf.absolutePath));
    else
        analysisAbsolutePath();
}

MMFile::MMFile(MMFile &&mmf)
{
    this->type = mmf.type;

    this->name = mmf.name;
    this->path = mmf.path;
    this->suffix = mmf.suffix;
    this->absolutePath = mmf.absolutePath;
    mmf.name = nullptr;
    mmf.path = nullptr;
    mmf.suffix = nullptr;
    mmf.absolutePath = nullptr;
}

MMFile::~MMFile()
{
    if(this->name != nullptr)
        delete this->name;
    if(this->path != nullptr)
        delete this->path;
    if(this->suffix != nullptr)
        delete this->suffix;
    if(this->absolutePath != nullptr)
        delete this->absolutePath;
}

int MMFile::getType() const
{
        return this->type;
}

void MMFile::setType(unsigned char t)
{
    type = t;
}

const wchar_t* MMFile::getName() const
{
    return name->data();
}

void MMFile::setName(const wchar_t *n)
{
    *(name) = n;
}

const wchar_t * MMFile::getPath() const
{
    return path->data();
}

void MMFile::setPath(const wchar_t *p)
{
    *(path) = p;
}


const wchar_t *MMFile::getSuffix() const
{
    return suffix->data();
}

void MMFile::analysisSuffix()
{
    //printf("%s\t",name->c_str());
    int i = 0;
    for(i = name->size() - 1;i >= 0;i--)
    {
        if(*(name->data() + i) == '.')
            break;
    }
    if(i > 0)
    {
        //printf("%d  %s\n", i,temp.c_str());
        suffix = new std::wstring(name->substr(i));
    }else{
        suffix = new std::wstring();
    }
}


const wchar_t * MMFile::getAbsolutePath() const
{
    return absolutePath->c_str();
}

void MMFile::analysisAbsolutePath()
{
    absolutePath = new std::wstring();
    absolutePath->operator+=(path->c_str());
    absolutePath->operator+=(L"\\");
    absolutePath->operator+=(name->c_str());
}


bool MMFile::nameIsLike(const wchar_t * str) const
{
	return  name->find(str) != std::wstring::npos;
}
