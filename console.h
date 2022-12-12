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
    void printEcho(QString);

protected:
    void contextMenuEvent(QContextMenuEvent *);

private:
    QString bufRegExp;
    QTextCharFormat colorOutDef, colorOutCurr;
    QRegExp allowRegExp;
    void printColorized(QString s);
    void insertTextToEnd(const QString &text, const QTextCharFormat &format);

    void onGotoSelected();
    void onCopySelected();
    void onCopyAll();
    void onSelectAll();
    void onClrAll();
    void onSaveSelected();
    void onSaveAll();

signals:
    QString printPreamble(bool echo = false);
};

#endif // CONSOLE_H
