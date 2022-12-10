#include "console.h"
#include <QtWidgets/QScrollBar>
#include <QMenu>
#include <QClipboard>

QStringList Console::history;
bool Console::connectIsOk = false;

Console::Console(QWidget *parent, QString pattern)
    : QPlainTextEdit(parent)
    , allowRegExp(pattern)
{
    QFont f("monospace");
    f.setStyleHint(QFont::Monospace);
    setFont(f);

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

    setConnectState(connectIsOk);
}

void Console::setConnectState(bool isOk)
{
    connectIsOk = isOk;
}

void Console::onClr()
{
    clear();
}

void Console::onCopy()
{
    QGuiApplication::clipboard()->setText(toPlainText());
}

void Console::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *contMenu = new QMenu(this);

    QAction *A  = new QAction(contMenu);
    A->setText(tr("Clear"));
    contMenu->addAction(A);
    connect(A, &QAction::triggered, this, &Console::onClr);

    QAction *B  = new QAction(contMenu);
    B->setText(tr("Copy"));
    contMenu->addAction(B);
    connect(B, &QAction::triggered, this, &Console::onCopy);

    contMenu->exec(event->globalPos());

    delete A;
    delete B;
    delete contMenu;
}

void Console::print(QString s)
{
    // RegExp match
    if (!s.contains(allowRegExp))
    {
        return;
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
        for (auto &i : lst)
        {
            bool res = false;
            int cmd = i.toInt(&res);
            if (res)
            {
                const Qt::GlobalColor colors[] =
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
    textCursor().insertText(s + '\n', colorOutCurr);

    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}
