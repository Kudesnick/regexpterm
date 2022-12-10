#ifndef FILEOFSTRINGLIST_H
#define FILEOFSTRINGLIST_H

#include <QString>

class FileOfStringList
{
private:
    const QString fname;
public:
    FileOfStringList(const QString fileName);
    bool read(QStringList &strList);
    void write(QStringList &strList);
};

#endif // FILEOFSTRINGLIST_H
