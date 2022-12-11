#include "console.h"
#include <QtWidgets/QScrollBar>
#include <QMenu>
#include <QClipboard>
#include <QFileDialog>

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
}

void Console::onCopySelected()
{
    copy();
}

void Console::onCopyAll()
{
    QGuiApplication::clipboard()->setText(toPlainText());
}

void Console::onSelectAll()
{
    this->selectAll();
}

void Console::onClrAll()
{
    clear();
}

void Console::onSaveSelected()
{
    QString fname = QFileDialog::getSaveFileName(nullptr, "Save selected text", ".", "Log files (*.log);;Any files (*)" );
    if (fname.length())
    {
        QFile file(fname);

        if (file.open(QIODevice::WriteOnly | QFile::Text))
        {
            file.write(textCursor().selectedText().toUtf8());
            file.close();
        }
    }
}

void Console::onSaveAll()
{
    QString fname = QFileDialog::getSaveFileName(nullptr, "Save full log", ".", "Log files (*.log);;Any files (*)" );
    if (fname.length())
    {
        QFile file(fname);

        if (file.open(QIODevice::WriteOnly | QFile::Text))
        {
            file.write(toPlainText().toUtf8());
            file.close();
        }
    }
}

void Console::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contMenu(this);

    QAction A("Copy selected (Ctl + C)", &contMenu);
    contMenu.addAction(&A);
    connect(&A, &QAction::triggered, this, &Console::onCopySelected);

    QAction B("Copy all", &contMenu);
    contMenu.addAction(&B);
    connect(&B, &QAction::triggered, this, &Console::onCopyAll);

    QAction C("Select all (Ctl + A)",&contMenu);
    contMenu.addAction(&C);
    connect(&C, &QAction::triggered, this, &Console::onSelectAll);

    QAction D("Clear all", &contMenu);
    contMenu.addAction(&D);
    connect(&D, &QAction::triggered, this, &Console::onClrAll);

    QAction E("Save selected as..", &contMenu);
    contMenu.addAction(&E);
    connect(&E, &QAction::triggered, this, &Console::onSaveSelected);

    QAction F("Save full log..", &contMenu);
    contMenu.addAction(&F);
    connect(&F, &QAction::triggered, this, &Console::onSaveAll);

    contMenu.exec(event->globalPos());
}

// Сюда должны приходить только полноценные строки с '\n'
void Console::printColorized(QString s)
{
    const QString decPattern = "\033\[?[0-9]+(;[0-9]+)*m"; /// Паттерн команды декоратора шрифта

    for (int startPos = s.indexOf(QRegExp(decPattern)); startPos >= 0; startPos = s.indexOf(QRegExp(decPattern)))
    {
        textCursor().insertText(s.mid(0, startPos), colorOutCurr);
        s = s.mid(startPos);

        int mPos = s.indexOf("m");

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
                    /// @todo логировать непонятные коды декораторов
                    break;
                }
            }
        }

        s = s.mid(mPos + 1);
    }
    textCursor().insertText(s, colorOutCurr);

    scrollDown();
}

void Console::print(QString s)
{
    bufRegExp += s;

    // Накапливаем строку, потому что иначе регулярное выражение теряет смысл
    for(int pos = bufRegExp.indexOf(QRegExp("(\r|\n)"));
        pos >= 0;
        pos = bufRegExp.indexOf(QRegExp("(\r|\n)")))
    {
        QString line = bufRegExp.mid(0, pos + 1);
        if (line.contains(allowRegExp))
        {
            printColorized(emit printPreamble() + line);
        }
        bufRegExp = bufRegExp.mid(pos + 1);
    }
}

void Console::printEcho(QString s)
{
    printColorized("\033[32m" + emit printPreamble(true) + s + "\033[0m");
}

void Console::scrollDown()
{
    QScrollBar *vbar = verticalScrollBar();
    vbar->setValue(vbar->maximum());
}
