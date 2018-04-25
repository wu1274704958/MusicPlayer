//
// Created by wws on 17-7-19.
//

#include "MMFile.h"
#include <memory>

MMFile::MMFile()
{
    this->type = TYPE_FILE;
    this->name = nullptr;
    this->path = nullptr;
    this->suffix = nullptr;
    this->absolutePath = nullptr;
}

MMFile::MMFile(FILE_TYPE type,const wchar_t *name, const wchar_t *path)
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
	if (mmf.name)
		this->name = new std::wstring(*(mmf.name));
	if (mmf.path)
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

MMFile & MMFile::operator=(const MMFile & mmf)
{
	cleanup();
	this->type = mmf.type;
	this->type = mmf.type;
	if (mmf.name)
		this->name = new std::wstring(*(mmf.name));
	if (mmf.path)
		this->path = new std::wstring(*(mmf.path));
	if (mmf.suffix != nullptr)
		this->suffix = new std::wstring(*(mmf.suffix));
	else
		analysisSuffix();
	if (mmf.absolutePath != nullptr)
		this->absolutePath = new std::wstring(*(mmf.absolutePath));
	else
		analysisAbsolutePath();

	return *this;
}

MMFile & MMFile::operator=(MMFile && mmf)
{
#ifdef _DEBUG
	std::cout << "operator=(MMFile && mmf) " << this->name << std::endl;
#endif // DEBUG
	cleanup();
	this->type = mmf.type;

	this->name = mmf.name;
	this->path = mmf.path;
	this->suffix = mmf.suffix;
	this->absolutePath = mmf.absolutePath;
	mmf.name = nullptr;
	mmf.path = nullptr;
	mmf.suffix = nullptr;
	mmf.absolutePath = nullptr;

	return *this;
}

MMFile::~MMFile()
{
#ifdef _DEBUG
	std::cout << "~MMFile() " <<(size_t)(this->name) << std::endl;
#endif // DEBUG
	cleanup();
}

MMFile::FILE_TYPE MMFile::getType() const
{
        return this->type;
}

void MMFile::setType(FILE_TYPE t)
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

void MMFile::cleanup()
{
	if (this->name)
	{
		delete this->name;
		this->name = nullptr;
	}
	if (this->path)
	{
		delete this->path;
		this->path = nullptr;
	}
	if (this->suffix)
	{
		delete this->suffix;
		this->suffix = nullptr;
	}
	if (this->absolutePath)
	{
		delete this->absolutePath;
		this->absolutePath = nullptr;
	}
		
}

void MMFile::analysisSuffix()
{
    //printf("%s\t",name->c_str());
	if (this->name)
	{
		int i = 0;
		for (i = name->size() - 1; i >= 0; i--)
		{
			if (*(name->data() + i) == '.')
				break;
		}
		if (i > 0)
		{
			//printf("%d  %s\n", i,temp.c_str());
			suffix = new std::wstring(name->substr(i));
		}
		else {
			suffix = new std::wstring();
		}
	}
}


const wchar_t * MMFile::getAbsolutePath() const
{
    return absolutePath->c_str();
}

void MMFile::analysisAbsolutePath()
{
	if (this->name && this->path)
	{
		absolutePath = new std::wstring();
		absolutePath->operator+=(path->c_str());
		absolutePath->operator+=(L"\\");
		absolutePath->operator+=(name->c_str());
	}
}


bool MMFile::nameIsLike(const wchar_t * str) const
{
	return  name->find(str) != std::wstring::npos;
}
