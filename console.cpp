#include "console.h"
#include <QtWidgets/QScrollBar>

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
    colorCmd.setForeground(Qt::green);
    colorCmd.setBackground(Qt::black);

    history = new QStringList;
    historyPos = 0;
    insertPrompt(false);
    isLocked = false;
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if(isLocked)
	return;
    if(event->key() >= 0x20 && event->key() <= 0x7e
       && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier))
	QPlainTextEdit::keyPressEvent(event);
    if(event->key() == Qt::Key_Backspace
       && event->modifiers() == Qt::NoModifier
       && textCursor().positionInBlock() > prompt.length())
	QPlainTextEdit::keyPressEvent(event);
    if(event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
	onEnter();
    if(event->key() == Qt::Key_Up && event->modifiers() == Qt::NoModifier)
	historyBack();
    if(event->key() == Qt::Key_Down && event->modifiers() == Qt::NoModifier)
	historyForward();
    QString cmd = textCursor().block().text().mid(prompt.length());
    emit onChange(cmd);
}

void Console::mousePressEvent(QMouseEvent *)
{
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *){}

void Console::contextMenuEvent(QContextMenuEvent *){}

void Console::onEnter()
{
    if(textCursor().positionInBlock() == prompt.length())
    {
	insertPrompt();
	return;
    }
    QString cmd = textCursor().block().text().mid(prompt.length());
    isLocked = true;
    historyAdd(cmd);
    emit onCommand(cmd);
}

void Console::output(QString s)
{
    // RegExp match
    if (!s.contains(allowRegExp))
        return;

    QString cmd = "";
    if (isLocked == false)
    {
        cmd = textCursor().block().text().mid(prompt.length());
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
        setTextCursor(cursor);
    }
    else
    {
        textCursor().insertBlock();
    }
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
    isLocked = false;
}

void Console::insertPrompt(bool insertNewBlock, QString cmd)
{
    if(insertNewBlock)
    textCursor().insertBlock();
    textCursor().insertText(prompt + cmd, colorCmd);
    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}

void Console::historyAdd(QString cmd)
{
    history->append(cmd);
    historyPos = history->length();
}

void Console::historyBack()
{
    if(!historyPos)
	return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.insertText(prompt + history->at(historyPos - 1));
    setTextCursor(cursor);
    historyPos--;
}

void Console::historyForward()
{
    if(historyPos == history->length())
	return;
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    if(historyPos == history->length() - 1)
	cursor.insertText(prompt);
    else
	cursor.insertText(prompt + history->at(historyPos + 1));
    setTextCursor(cursor);
    historyPos++;
}
