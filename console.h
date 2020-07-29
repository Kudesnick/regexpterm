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
    void setConnectState(bool isOk);
    static QStringList history;
protected:
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *);
private:
    const int historyMax = 10;
    static bool connectIsOk;

    QString prompt;
    int historyPos;
    QTextCharFormat colorOutDef, colorCmdOk, colorCmdErr, colorOutCurr, colorCmdCurr;
    QRegExp allowRegExp;

    void onEnter();
    QString removePromt(void);
    void insertPrompt(bool insertNewBlock = true, QString cmd = "");
    void historyAdd(QString);
    void historyGet(int);

    void onClr();
signals:
    void onCommand(QString);
    void onChange(QString);
};

#endif // CONSOLE_H
