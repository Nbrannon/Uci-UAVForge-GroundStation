/* This is the header file for the MainWindow class. For information about
 * this class and its functions, see comments on the top of mainwindow.cpp.
 *
 * File added sometime prior to April 30 2015.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include <QShortcut>
#include <QPixmap>
#include <QDebug>
#include <iostream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Constructor and destructor
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //ui - used to do window layout
    Ui::MainWindow *ui;
    //animation - added by GUI team... don't know what it's for yet
    QPropertyAnimation *animation;

private slots:
    //button actions
    void openTutorial();
    void missionPlanningClicked();
    void missionExecutionClicked();
    void missionRecapClicked();

    //quit command
    void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
