#ifndef CONSOLE_H
#define CONSOLE_H

#include <QtGui>
#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr, QString pattern = ".*");
    void output(QString);
    void scrollDown();
    void setRegExp(QString);
protected:
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
private:
    QString prompt;
    bool isLocked;
    QStringList *history;
    int historyPos;
    QTextCharFormat colorOutDef, colorCmd, colorOutCurr;
    QRegExp allowRegExp;

    void onEnter();
    void insertPrompt(bool insertNewBlock = true, QString cmd = "");
    void historyAdd(QString);
    void historyBack();
    void historyForward();
signals:
    void onCommand(QString);
    void onChange(QString);
};

#endif // CONSOLE_H
