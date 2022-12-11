#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class Settings;

class ListSett
{
private:
    Settings *parent;
    const QString name;
public:
    ListSett(Settings *, const QString);
    bool read(QStringList&);
    void write(QStringList&);
};

class Settings: private QSettings
{
public:
    Settings(const QString filename);
    bool readList(const QString group, QStringList&);
    void writeList(const QString group, QStringList&);
    class ListSett cmds, tabs, patterns;
};

#endif // SETTINGS_H
