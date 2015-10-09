#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QWidget>
#include "ui_tutorial.h"
#include "mainwindow.h"

namespace Ui {
class Tutorial;
}

class Tutorial : public QWidget
{
    Q_OBJECT

public:
    explicit Tutorial(QWidget *parent = 0);
    ~Tutorial();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Tutorial *ui;
};

#endif // TUTORIAL_H
