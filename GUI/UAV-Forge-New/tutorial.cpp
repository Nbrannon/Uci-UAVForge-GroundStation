#include "tutorial.h"

Tutorial::Tutorial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tutorial)
{
    ui->setupUi(this);
//    QPixmap picture("UAVTutorialTest.png");
//    ui->label_picture->setPixmap(picture);
}

Tutorial::~Tutorial()
{
    delete ui;
}

void Tutorial::on_pushButton_2_clicked()
{

    MainWindow *mainwindow = new MainWindow();
    this -> close();
    mainwindow->showFullScreen();

}
