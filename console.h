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

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    QString bufRegExp;
    QString bufColorPattern;
    QTextCharFormat colorOutDef, colorOutCurr;
    QRegExp allowRegExp;
    void printColorized(QString s);
    void printRegexp(QString s);
    void scrollDown();

    void onCopySelected();
    void onCopyAll();
    void onSelectAll();
    void onClrAll();
    void onSaveSelected();
    void onSaveAll();
};

#endif // CONSOLE_H
