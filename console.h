#ifndef CONSOLE_H
#define CONSOLE_H

#include <QtGui>
#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr, QString pattern = ".*");
    void print(QString);
    void scrollDown();
    void setConnectState(bool isOk);
    static QStringList history;
protected:
    void contextMenuEvent(QContextMenuEvent *);
private:
    const int historyMax = 10;
    static bool connectIsOk;

    QTextCharFormat colorOutDef, colorOutCurr;
    QRegExp allowRegExp;

    /// @todo add "copy selected", save as file
    void onClr();
    void onCopy();
};

#endif // CONSOLE_H
