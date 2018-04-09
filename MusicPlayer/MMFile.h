//
// Created by wws on 17-7-19.
//

#ifndef FIRST_MMFILE_H
#define FIRST_MMFILE_H

#include <iostream>
#include <string>

class MMFile {
private:

    unsigned char type;
    std::wstring *name;
    std::wstring *path;
    std::wstring *suffix;
    std::wstring *absolutePath;
public:

    MMFile();
    MMFile(int type,const wchar_t *name,const wchar_t *path);
    MMFile(const MMFile &mmf);
    MMFile(MMFile &&mmf);

    int getType() const;
    void setType( unsigned char t);

	const wchar_t *getName() const;
    void setName(const wchar_t* n);

    const wchar_t *getPath() const;
    void setPath(const wchar_t *p);

	const wchar_t *getSuffix() const;
	const wchar_t *getAbsolutePath() const;

	bool nameIsLike(const wchar_t *str) const;

    virtual  ~MMFile();


private:
    void analysisSuffix();
    void analysisAbsolutePath();
};


#endif //FIRST_MMFILE_H
