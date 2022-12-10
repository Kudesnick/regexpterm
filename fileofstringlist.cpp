#include "fileofstringlist.h"

#include <QFile>

FileOfStringList::FileOfStringList(const QString fileName):
    fname(fileName)
{

};

bool FileOfStringList::read(QStringList &strList)
{
    QFile file(fname);

    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        while(!file.atEnd())
        {
            strList.append(QString(file.readLine()).trimmed());
        }
        file.close();
    }

    return (strList.count() != 0);

};

void FileOfStringList::write(QStringList &strList)
{
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly | QFile::Text))
    {
        for (auto &i : strList)
        {
            file.write(i.toUtf8() + '\n');
        }
        file.close();
    }
};
