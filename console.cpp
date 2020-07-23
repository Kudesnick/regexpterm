#include "console.h"
#include <QtWidgets/QScrollBar>
#include <QMenu>

QStringList Console::history;

Console::Console(QWidget *parent, QString pattern)
    : QPlainTextEdit(parent)
    , allowRegExp(pattern)
{
    prompt = "rtt$ ";

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    setPalette(p);

    colorOutDef.setForeground(Qt::white);
    colorOutDef.setBackground(Qt::black);
    colorOutCurr = colorOutDef;
    colorCmdOk.setForeground(Qt::green);
    colorCmdOk.setBackground(Qt::black);
    colorCmdErr.setForeground(Qt::red);
    colorCmdErr.setBackground(Qt::black);
    colorCmdCurr = colorCmdErr;

    historyPos = -1;
    insertPrompt(false);
}

void Console::setConnectState(bool isOk)
{
    colorCmdCurr = (isOk) ? colorCmdOk : colorCmdErr;
    insertPrompt(false, removePromt());
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (true
        && event->key() >= 0x20
        && event->key() <= 0x7e
        && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier)
       )
        QPlainTextEdit::keyPressEvent(event);
    if (true
        && event->key() == Qt::Key_Backspace
        && event->modifiers() == Qt::NoModifier
        && textCursor().positionInBlock() > prompt.length()
       )
        QPlainTextEdit::keyPressEvent(event);
    if(event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
        onEnter();
    if(event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
        historyGet(-1);
    if(event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
        historyGet(1);
    QString cmd = textCursor().block().text().mid(prompt.length());
    emit onChange(cmd);
}

void Console::mousePressEvent(QMouseEvent *)
{
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *){}

void Console::onClr()
{
    clear();
    insertPrompt(false);
}

void Console::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *contMenu = new QMenu(this);
    QAction *A  = new QAction(contMenu);

    A->setText(tr("Clear"));
    contMenu->addAction(A);
    connect(A, &QAction::triggered, this, &Console::onClr);

    contMenu->exec(event->globalPos());
    delete contMenu;
}

void Console::onEnter()
{
    QString cmd = textCursor().block().text().mid(prompt.length());
    historyAdd(cmd);
    insertPrompt();
    emit onCommand(cmd);
}

void Console::output(QString s)
{
    // RegExp match
    if (!s.contains(allowRegExp))
        return;

    QString cmd = removePromt();

    // Colorized console
    for (int startPos = s.indexOf("\033["); startPos >= 0; startPos = s.indexOf("\033["))
    {
        textCursor().insertText(s.mid(0, startPos), colorOutCurr);
        s = s.mid(startPos);

        int mPos = s.indexOf("m");
        if (mPos < 1)
            break;
        QStringList lst = s.mid(2, mPos - 2).split(QLatin1Char(';'));
        for (auto i : lst)
        {
            bool res = false;
            int cmd = i.toInt(&res);
            if (res)
            {
                Qt::GlobalColor colors[] =
                {
                    Qt::black  ,
                    Qt::red    ,
                    Qt::green  ,
                    Qt::yellow ,
                    Qt::blue   ,
                    Qt::magenta,
                    Qt::cyan   ,
                    Qt::white  ,
                };

                switch(cmd)
                {
                    // Спецэффекты
                    case 0:	// Сброс к начальным значениям
                        colorOutCurr = colorOutDef;
                    break;
                    case 1:	// Жирный
                        colorOutCurr.setFontWeight(QFont::Bold);
                    break;
                    case 2:	// Блёклый
                        colorOutCurr.setFontWeight(QFont::Light);
                    break;
                    case 3:	// Курсив
                        colorOutCurr.setFontItalic(true);
                    break;
                    case 4:	// Подчёркнутый
                        colorOutCurr.setFontUnderline(true);
                    break;
                    case 5:	// Редкое мигание
                        colorOutCurr.setFontWeight(QFont::Bold);
                    break;
                    case 6:	// Частое мигание
                        colorOutCurr.setFontWeight(QFont::Black);
                    break;
                    case 7:	// Смена цвета фона с цветом текста
                    {
                        QBrush tmp = colorOutCurr.foreground();
                        colorOutCurr.setForeground(colorOutCurr.background());
                        colorOutCurr.setBackground(tmp);
                    }
                    break;

                    // Цвет текста
                    case 30: // Чёрный
                    case 31: // Красный
                    case 32: // Зелёный
                    case 33: // Жёлтый
                    case 34: // Синий
                    case 35: // Фиолетовый
                    case 36: // Бирюзовый
                    case 37: // Белый
                        colorOutCurr.setForeground(colors[cmd - 30]);
                     break;

                    // Цвет фона
                    case 40: // Чёрный
                    case 41: // Красный
                    case 42: // Зелёный
                    case 43: // Жёлтый
                    case 44: // Синий
                    case 45: // Фиолетовый
                    case 46: // Бирюзовый
                    case 47: // Белый
                        colorOutCurr.setBackground(colors[cmd - 40]);
                    break;

                    default:
                        break;
                }
            }
        }

        s = s.mid(mPos + 1);
    }
    textCursor().insertText(s, colorOutCurr);

    insertPrompt(true, cmd);
}

QString Console::removePromt(void)
{
    QString cmd = "";
    cmd = textCursor().block().text().mid(prompt.length());
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.removeSelectedText();
    setTextCursor(cursor);

    return cmd;
}

void Console::insertPrompt(bool insertNewBlock, QString cmd)
{
    if(insertNewBlock)
        textCursor().insertBlock();
    textCursor().insertText(prompt + cmd, colorCmdCurr);
    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void Console::historyAdd(QString cmd)
{
    if (history.contains(cmd))
    {
        history.removeAt(history.indexOf(cmd));
    }
    history.append(cmd);

    while (history.length() > historyMax)
        history.removeFirst();
    historyPos = history.length();
}

void Console::historyGet(int _inc)
{
    if (historyPos < 0)
    {
        historyPos = history.length();
    }

    historyPos += _inc;
    if(historyPos < 0)
    {
        historyPos = 0;
        return;
    }
    else if (historyPos >= history.length())
    {
        historyPos = history.length() - 1;
        return;
    }

    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(prompt + history.at(historyPos), colorCmdCurr);
    setTextCursor(cursor);
}
