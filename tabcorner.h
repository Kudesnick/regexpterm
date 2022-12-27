#ifndef TABCORNER_H
#define TABCORNER_H

#include <QWidget>

namespace Ui {
class TabCorner;
}

class TabCorner : public QWidget
{
    Q_OBJECT

public:
    explicit TabCorner(QWidget *parent = nullptr);
    ~TabCorner();

private slots:
    void on_btnNew_pressed();

    void on_btnMenu_pressed();

    void on_btnClose_pressed();

private:
    Ui::TabCorner *ui;

signals:
    void pressNew();
    void pressMenu();
    void pressClose();
};

#endif // TABCORNER_H
