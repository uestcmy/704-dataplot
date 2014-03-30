

#include <QApplication>

#include <qpsk.h>
#include <ber_bar.h>
#include <ber.h>
#include <GL/glut.h>




#include <qfile.h>

#include <QDebug>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <GL/glut.h>
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
/*
    QPSK w3;
    w3.resize(600,200);
    w3.show();
    w3.setWindowTitle("Ber");

    BER w24;
    w24.resize(600,450);
    w24.show();
    w24.setWindowTitle("BER");
*/
    BER_BAR w4;
    w4.resize(600,450);
    w4.show();
    w4.setWindowTitle("BER");
    return app.exec();
    //edit in fedora
}



