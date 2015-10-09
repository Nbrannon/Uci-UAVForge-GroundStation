#include "gsclient.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QQueue>

#include "jsonobject.h"

using namespace std;
//using namespace net;
using namespace rapidjson;

#define BUFSIZE 256

GsClient::GsClient()
    :sendport(30005){
    for (int i = 0; i < 4; ++i){
        addressPartsInt[i] = 0;
    }
    //std::cout << "Please dont devour my mortal soul" << std::endl;
}


void GsClient::set_list(QList<QPair<double , double > > cl){
    cor_list = cl;
}


void GsClient::getCoordinates(QList<QPair<double, double> > cl){

    QQueue<Coordinates> cor_queue;
    jsonObj coordJ;

    Coordinates tempC;
    for(QPair<double,double> pair : cl){
        tempC.latitude = pair.first;
        tempC.longitude = pair.second;
        cor_queue.enqueue(tempC);
    }

    while(!cor_queue.empty())
        coordJ.addCoordinate(cor_queue.dequeue());

    std::string coords = coordJ.coordinatesExtractString();
    size_t len = coords.length();
    char ccoords[4096];
    u_int i;
    for (i = 0; i < len; i++)
        ccoords[i] = coords[i];
    ccoords[i] = '\0';
    const char* jsonStrC = ccoords;
    //std::cout << ccoords << std::endl;

    Document documentC;
    documentC.Parse(jsonStrC);
    PrettyWriter<StringBuffer> writerC(bufferC);
    documentC.Accept(writerC);

    //std::cout << "rapidJson version:" << std::endl;
    //std::cout << bufferC.GetString() << std::endl;
}



int GsClient::gsc_connect_start(){
    addressPartsInt[0] = 127;
    addressPartsInt[1] = 0;
    addressPartsInt[2] = 0;
    addressPartsInt[3] = 1;
    sendport = 3491;

    // initialize socket layer

    if (!net::InitializeSockets())
    {
        std::cout << "failed to initialize sockets" << std::endl;
        return 1;
    }

    // create socket

    int port = 3490;

    std::cout << "creating socket on port " <<  port << std::endl;

    if (!my_socket.Open(port))
    {
        std::cout << "failed to create socket!" << std::endl;
        return 1;
    }

    return 0;

}

void GsClient::gsc_send_message(){
    getCoordinates(cor_list);
//    char data[4096];
//    strcpy(data, "Mortal soul devaoured");

//   // std::cin.getline(data , BUFSIZ);
//    net::GS_Address myAddress = net::GS_Address(addressPartsInt[0], addressPartsInt[1],
//            addressPartsInt[2], addressPartsInt[3], sendport);

//    my_socket.Send(myAddress , data, sizeof(data));

    string to_send = bufferC.GetString();
    char to_send_c[BUFSIZE];
    size_t sendLen = to_send.length();

    int packets = sendLen / BUFSIZE + 1;
    std::cout << packets << " packets required to send."<< std::endl;

    for(int p = 0; p <packets; p++){
        int first = p * (BUFSIZE-1);
        int last = (p+1) * (BUFSIZE-1) - 1;
        for (int i = first; i < last; i++){
            to_send_c[i%(BUFSIZE-1)] = to_send[i];
        }
        to_send_c[BUFSIZE-1] = '\0';

        //std::cout << "Packet: " << p + 1 << std::endl;
        //std::cout << "Sending:" <<to_send_c << std::endl;
        std::cout << "Sending bytes " << first << " to "<< last<< std::endl;
        //std::cout << "Total size: " << sizeof(to_send_c) << std::endl;
        char buffer[BUFSIZE];
       // int recvBytes = 0;



        //for(int i = 0; i < 100; i++)
        {
            //char data[256];

            //cin.getline(data, 256);
            net::GS_Address addra = net::GS_Address(addressPartsInt[0], addressPartsInt[1], addressPartsInt[2], addressPartsInt[3], sendport);
           // Address sender;
            strcpy(buffer, "Mortal soul devoured.");
        //	recvBytes = socket.Receive(sender, buffer, sizeof(buffer));  //receive currently not working
            //std::cout << to_send_c;
            my_socket.Send(addra, to_send_c, sizeof(to_send_c));

        }
    }
    std::cout<<std::endl;

    std::cout << "WORKING JUST FINE" << std::endl;

}

/*void GsClient::sendStopMessage(){
    bufferC << "Empty String";
    qDebug() << bufferC;
    string to_send = bufferC.GetString();
    char to_send_c[BUFSIZ];
    int sendLen = to_send.length();
    int i;
    for (i = 0; i < sendLen; i++)
        to_send_c[i] = to_send[i];
    to_send_c[i] = '\0';
    std::cout << to_send_c << std::endl;
    char buffer[BUFSIZ];
    //int recvBytes = 0;
    for(int i = 0; i < 100; i++) {
        net::GS_Address addra = net::GS_Address(addressPartsInt[0], addressPartsInt[1], addressPartsInt[2], addressPartsInt[3], sendport);
        // Address sender;
        strcpy(buffer, "Mortal soul devoured.");
        my_socket.Send(addra, to_send_c, sizeof(to_send_c));
    }
}*/

void GsClient::gsc_close_connection(){
    net::ShutdownSockets();
}

