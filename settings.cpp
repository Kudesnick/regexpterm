#include "settings.h"

#include <QCoreApplication>

// See https://stackoverflow.com/questions/2452893/save-qlistint-to-qsettings

ListSett::ListSett(Settings *_prnt, const QString _name):
    parent(_prnt),
    name(_name)
{
}

bool ListSett::read(QStringList &strList)
{
    return parent->readList(name, strList);
}

void ListSett::write(QStringList &strList)
{
    parent->writeList(name, strList);
}


Settings::Settings(const QString filename)
    : QSettings(QCoreApplication::applicationDirPath().append("\\" + filename), QSettings::IniFormat)
    , cmds(this, "COMMANDS")
    , tabs(this, "TABS")
    , patterns(this, "PATTERNS")
{
}

bool Settings::readList(const QString group, QStringList &strList)
{
    strList.clear();

    int cnt = this->beginReadArray(group);

    for (int i = 0; i < cnt; i++)
    {
        this->setArrayIndex(i);
        strList.append(this->value(group).toString());
    }

    this->endArray();

    return (bool)cnt;
};

void Settings::writeList(const QString group, QStringList &strList)
{
    this->beginWriteArray(group);
    for (int i = 0; i < strList.count(); i++)
    {
        this->setArrayIndex(i);
        this->setValue(group, (QString)strList.takeAt(i).toUtf8());
    }
    this->endArray();
    this->sync();
}
