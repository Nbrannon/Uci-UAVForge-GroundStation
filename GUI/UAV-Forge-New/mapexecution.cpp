#include "MapExecution.h"
#include "ui_MapExecution.h"
#include "missionrecap.h"
#include "mainwindow.h"
#include "gsclient.h"
#include "gsserver.h"
#include <QWebFrame>
#include <QPair>
#include <QList>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>

MapExecution::MapExecution(QList<QString> strings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapExecution)
{
    //myServer = new GsServer();
    ui->setupUi(this);
    mapStrings = strings;
    connect(ui->webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(addClickListener()));
    connect(ui->pushBtnStop,SIGNAL(clicked()),this,SLOT(stopClicked()));
    ui->webView->load(QUrl("qrc:/res/html/mapsExecution.html"));

    QList <QPair<double, double > > h;
    h << QPair<double, double >(32, 32);

    myServer.openServer();
    connect(&myServer.networkListener,SIGNAL(sendCoordinates()),this,SLOT(sendFlightPlan()));
    connect(&myServer.networkListener,SIGNAL(logTelemetry(QString)),this,SLOT(newTelemCoord(QString)));

}

MapExecution::MapExecution(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapExecution)
{
    ui->setupUi(this);
    connect(ui->webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(addClickListener()));
    ui->webView->load(QUrl("qrc:/res/html/mapsExecution.html"));

}

MapExecution::~MapExecution()
{
    delete ui;
}

void MapExecution::finishClicked()
{
    MissionRecap *missionRecap = new MissionRecap();
    this->close();
    missionRecap->showFullScreen();
}


void MapExecution::newTelemCoord(QString msgString){
    double lat = msgString.split(',').at(0).toDouble();
    double lng = msgString.split(',').at(1).toDouble();
    long time = msgString.split(',').at(2).toLong();
    std::cout << time << std::endl;
    //plotPosition(lat,lng);
}

void MapExecution::sendFlightPlan(){
    std::cout << "Sending..." << std::endl;
    QList<QPair<double,double> > coords = getDoublePairs(mapStrings);
    myServer.sendMessage((char*)std::to_string(coords.length()).c_str(), 10, 11, myServer.uav_fd);
    char msgBuffer[4096];
    myServer.formatCoordinatesToSend(msgBuffer, 4096, coords);
    myServer.sendMessage(msgBuffer,strlen(msgBuffer),myServer.maxPackSize,myServer.uav_fd);
    std::cout << "done!" << std::endl;
}


void MapExecution::stopClicked() {
    /* Sends the point (999.99,999.99) to the UAV. Used as a code for stop.
    Function added by Jordan Dickson March 9th 2015. */
    QList <QPair<double, double > > h;
    h << QPair<double, double >(999.99,999.99);
    //myClient.set_list(h);
    //myClient.gsc_send_message();
}

void MapExecution::returnHomeClicked() {
    /* would not return back to home, it is now fixed. Arash */
    MainWindow *mainwindow = new MainWindow();
    mainwindow->showFullScreen();
    this -> close();
}

void MapExecution::cancelClicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->showFullScreen();
    this -> close();
}

void MapExecution::on_pushButton_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    this -> close();
    mainwindow->showFullScreen();
}

void MapExecution::setMap(QList<QString> list) {
    /* Sends a request for the map to clear itself, causing the JavaScript page
    to reload itself. This function then cycles through each entry in the string
    list and sends it to the addPoint(QString) function to be added to the map.
    Function added by Jordan Dickson Feb 21st 2015. */

    //Sends clearMap request.
    ui->webView->page()->mainFrame()->evaluateJavaScript("clearMap()");

    //Loops through list entries
    for(int i = 0; i < list.length(); i++){
        addPoint(list[i]);
    }
}

QList<QPair<double,double> > MapExecution::getDoublePairs(QList<QString> strings){
    /* Takes a list of strings, each string formatted (Action,Longitude,LongDirection,
    Latitude,LatDirection,Behavior), and returns a new list of double pairs formatted
    <latitude,longitude>. Function added by Jordan Dickson Feb 21st 2015. */

    QList<QPair<double,double> > returnList;
    for(QString string: strings){
        QList<QString> comps = string.split(",");

        //Converts West and South coordinates to negative numbers.
        double lat = comps[3].toDouble();
        double lng = comps[1].toDouble();
        if(comps[2] == "W") {
            lng *= -1.0;
        }
        if(comps[4] == "S") {
            lat *= -1.0;
        }
        returnList.append(QPair<double,double>(lat,lng));
    }

    return returnList;

}

void MapExecution::addPoint(QString string){
    /*  Takes a QString formated (Action,Longitude,LongDirection,Latitude,
    LatDirection,Behavior) and extracts the (lat,lng) pair needed to add
    a new point to the map. Function added by Jordan Dickson Feb 21st 2015. */

    //Split the string into a list of components separated by ','
    QList<QString> comps = string.split(",");

    //Converts West and South coordinates to negative numbers.
    double lat = comps[3].toDouble();
    double lng = comps[1].toDouble();
    if(comps[2] == "W"){
        lng *= -1.0;
    }
    if(comps[4] == "S"){
        lat *= -1.0;
    }

    //Sends the add point request with its parameters.
    ui->webView->page()->mainFrame()->evaluateJavaScript("addLatLngCoords("+QString::number(lat)+","+QString::number(lng)+")");
}

/*void MapExecution::push_new_point(QString string){
   QList<QString> points = string.split(",");
}*/

void MapExecution::addClickListener() {
    /* Since c++/JS bridges are broken when the JS page refreshes this slot
     is used to rebruild the bridge each time when triggered by a
     javaScriptWindowObjectCleared signal from the page frame. Function
     added by Jordan Dickson Feb 21st 2015. */

    //Creates the bridge called cbridge between the java script object and this class.
    ui->webView->page()->mainFrame()->addToJavaScriptWindowObject("cbridge",this);

}

void MapExecution::addNewMap(){
    /*  Function called by the JavaScript to add the map data from mission planning.
    This is necessary because data cannot be added until the html file is completely
    loaded. Jordan 2/21/2015 */
    setMap(mapStrings);

    //used to initiate the simulated imput -- delete later
    //ui->webView->page()->mainFrame()->evaluateJavaScript("simulateInput()");
}

void MapExecution::plotPosition(double lat, double lng){
    /*  Sends a (latitude,longitude) pair to the map to be plotted.
    Used for telemetry. Keeps track of every point sent by appending it
    to the flightPath list. Jordan 2/21/2015 */

    //Problem: Loss of accuracy for some reason.
    //      Seems to be caused by rounding done by the JS/C++ bridge
  
    flightPath << QPair<double, double>(lat,lng);
    ui->webView->page()->mainFrame()->evaluateJavaScript("addActualPath("+QString::number(lat)+","+QString::number(lng)+")");
}
