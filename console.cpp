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

    static QString buf;
    s = buf + s;
    buf.clear();

    // Colorized console
    /// @todo при асинхронной вычитке ломаются последовательности. Реализовать в виде конечного автомата!
    for (int startPos = s.indexOf("\033"); startPos >= 0; startPos = s.indexOf("\033"))
    {
        textCursor().insertText(s.mid(0, startPos), colorOutCurr);
        s = s.mid(startPos);

        int mPos = s.indexOf("m");
        /* Защита от фрагментирования. Если обнаружено начало управляющей последовательности,
         * но не обнаружено конца - сохраняем фрагмент в буфер и ждем продолжения.*/
        /// @todo защититься от сломанных последовательностей и зависаний по таймауту (недозагруженных последовательностей)
        if (s.indexOf("\033[") >= mPos)
        {
            // Вероятно, управляющая последовательность еще недозагружена
            buf = s;
            scrollDown();
            return;
        }
        QStringList lst = s.mid(2, mPos - 2).split(QLatin1Char(';'));
        for (auto &i : lst)
        {
            bool res = false;
            int cmd = i.toInt(&res);
            if (res)
            {
                const Qt::GlobalColor colors[] =
                {
                    Qt::black      ,
                    Qt::darkRed    ,
                    Qt::darkGreen  ,
                    Qt::darkYellow ,
                    Qt::darkBlue   ,
                    Qt::darkMagenta,
                    Qt::darkCyan   ,
                    Qt::lightGray  ,
                };

                const Qt::GlobalColor colors1[] =
                {
                    Qt::darkGray,
                    Qt::red     ,
                    Qt::green   ,
                    Qt::yellow  ,
                    Qt::blue    ,
                    Qt::magenta ,
                    Qt::cyan    ,
                    Qt::white   ,
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
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                    colorOutCurr.setForeground(colors[cmd - 30]);
                break;

                case 39:
                    colorOutCurr.setForeground(colorOutDef.foreground());
                break;

                case 90:
                case 91:
                case 92:
                case 93:
                case 94:
                case 95:
                case 96:
                case 97:
                    colorOutCurr.setForeground(colors1[cmd - 90]);
                break;

                // Цвет фона
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                    colorOutCurr.setBackground(colors[cmd - 40]);
                break;

                case 49:
                    colorOutCurr.setBackground(colorOutDef.background());
                break;

                case 100:
                case 101:
                case 102:
                case 103:
                case 104:
                case 105:
                case 106:
                case 107:
                    colorOutCurr.setBackground(colors1[cmd - 100]);
                break;

                default:
                    break;
                }
            }
        }

        s = s.mid(mPos + 1);
    }
    textCursor().insertText(s, colorOutCurr);

    scrollDown();
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}
