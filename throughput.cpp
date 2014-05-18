#include <throughput.h>
#include <math.h>
#include <QDebug>
#include <stdio.h>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QtGui/QApplication>
#include <QFileInfo>
#include <QTextCodec>
#include <QDateTime>


#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "QDebug"
#include <QtGui/QApplication>
#define LEN 9000
#define LENGTH_OF_OFDM 14404
#define connected
//#define TEST_SOCKET

#include "ui_throughput.h"
socklen_t size_chl2_3;
socklen_t size_chl2_4;
int cnt_3 = 0;
sockaddr_in addrSrv_chl1_3,addrrcv_chl1_3;
sockaddr_in addrSrv_chl1_4,addrrcv_chl1_4;
int sockser_chl1_3;
int sockser_chl1_4;
 double pilot4[1200][2] = {0};
  double pilot2[1200][2] = {0};
 double qam16_3[500][2] = {0};
char map1200_3[1200][8];
char map1200[1200][8];
double data1_3[1800][2] = {0};//complex number real and image
double data2_3[1800][2] = {0};
double data1[1800][2] = {0};
double new_star[1000][2] = {0};
int cnt_newstar = 0;
int err_3 = 0;
int bit_cnt_2_3 = 0;
char db_2[50] = {0};
int flag_bar = 0;int  cnt_pilot = 0;
int cnt_2 = 0;
double cnt_allstar1 = 0;
double cnt_allstar2 = 0;

double cnt_err_g1 = 0;
double cnt_err_g[8] ={0};

double comp_err_1 = 0;
double comp_err[10] = {0};
char str[500];
char str_im[500];
char s1_re[500];
char s1_im[500];
int err = 0;
int bit_cnt = 0;
double ber_rate = 0;

int err_0 = 0;
int bit_cnt_0 = 0;
double ber_rate_0 = 0;


double height2[9] = {0};
ThroughPut::ThroughPut(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::ThroughPut)
{

    static double ttt[1000] = {0};
    pdata2_3 = &ttt[0];

    ui->setupUi(this);
    translate = -6.0;
    xRot = yRot = zRot = 0.0;
    xRot += 0;
    yRot += 0;
    num_p = 300;

    InputManagement();
    size_chl2_3=sizeof(sockaddr_in);
    sockser_chl1_3=socket(AF_INET,SOCK_DGRAM,0);
    addrSrv_chl1_3.sin_addr.s_addr=htonl(INADDR_ANY);
    addrSrv_chl1_3.sin_family=AF_INET;
    addrSrv_chl1_3.sin_port=htons(7024);//server : receive port number
    bind(sockser_chl1_3,(sockaddr*)&addrSrv_chl1_3,sizeof(sockaddr));

    size_chl2_4=sizeof(sockaddr_in);
    sockser_chl1_4=socket(AF_INET,SOCK_DGRAM,0);
    addrSrv_chl1_4.sin_addr.s_addr=htonl(INADDR_ANY);
    addrSrv_chl1_4.sin_family=AF_INET;
    addrSrv_chl1_4.sin_port=htons(7021);//server : receive port number
    bind(sockser_chl1_4,(sockaddr*)&addrSrv_chl1_4,sizeof(sockaddr));


    id1 = startTimer(100);


}
ThroughPut::~ThroughPut(){
   delete ui;
}

void ThroughPut::initializeGL()
{
    glClearColor(0.8,0.8,0.8,0);//canvas
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);


}
void ThroughPut::resizeGL(int w, int h)
{
    glViewport(-10, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //luPerspective(40.0, (GLfloat)w/(GLfloat)h, 2, 30.0);
    //gluPerspective(40,1.33, 2, 30.0);
    //glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 2.3* ( GLfloat ) w / ( GLfloat ) h, -2, 2, -15.0, 15.0);
     //glFrustum (-1.5* ( GLfloat ) w / ( GLfloat ) h, 2* ( GLfloat ) w / ( GLfloat ) h, -2, 1.3, 6, 10.0);
     glOrtho (-3.2* ( GLfloat ) w / ( GLfloat ) h, 0.6* ( GLfloat ) w / ( GLfloat ) h, -2, 2, -10.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//
void ThroughPut::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glTranslatef(0.0, 0.0, translate);// translate is included in <QKeyEvent>
    glTranslatef(0.0, -0.5, 0.3);// translate is included in <QKeyEvent>
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);

    //Draw_point();
   // Draw_line();

    Bar_ber();


}

void ThroughPut::Bar_ber(){

#ifdef connected
    if( flag_bar == 0 ){
        height2[0] = 0;
    }
    if(flag_bar == 1 || *(pdata2_3) - 0 > 1e-7 ){

        comp_err[0] =  *(pdata2_3);
        if( comp_err[0] >  0.75  ){
            comp_err[0] = 1;
        }
        height2[0] =  -1 + (1-comp_err[0])*2;

          flag_bar = 1;
    }
#endif
#ifdef standardalone
    double height2[0] = 1.5;
#endif

    //qDebug() <<  " height is :" << height;

     /*axis*/
    glLineWidth(3);
    //glBegin(GL_LINE_STRIP);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.5,0,1,0.8);
    glVertex2f(-3,2); //left up
    glVertex2f(-3,-1);// left,down
    glVertex2f(0,-1);//right down
    glVertex2f(0,2);//right up  [-3 2.8 2 -1]
    glEnd();
    /*bar*/
   double pos = 0;

   if( ber_rate  >0.4 ){
      ber_rate = 1;
   }
   if( ber_rate_0  >0.4 ){
      ber_rate_0 = 1;
   }

   height2[1] = 1-ber_rate-1;
   height2[0] = 1-ber_rate_0-1;
   comp_err[3] = comp_err[7];
   comp_err[4] = comp_err[8];



    for( int i = 0 ; i < 2 ; i++ ){
    glBegin(GL_QUADS);
    glColor4f(1,i/9.0,0,0.5);
    glVertex2f(-1.8+pos,-1);//x,y
    glVertex2f(-1.8+pos,height2[i]);
    glVertex2f(-2.5+pos,height2[i]);
    glVertex2f(-2.5+pos,-1);
     glEnd();
     pos  = (i+1)*1.5;
    }
    /*error == 1 */

    glColor3f(1,0,0);
    renderText(-3.5,2.2,1,"bps/Hz");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,1);
    glVertex2f(0,1);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-3.5,1,1,"4");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,0.5);
    glVertex2f(0,0.5);//x,y
    glEnd();
    glColor3f(1,0,0);
    renderText(-3.5,0.5,1,"3");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,0);
    glVertex2f(0,0);//x,y
    glEnd();
    glColor3f(1,0,0);
    renderText(-3.5,0,1,"2");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,-0.5);
    glVertex2f(0,-0.5);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-3.5,-0.5,1,"1");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(0,0.5);//x,y
    glEnd();

    char title[9][50];
    sprintf(title[0],"16QAM");
    sprintf(title[1],"UE1 Rx1");
    sprintf(title[2],"UE1 Rx2");
    sprintf(title[3],"UE2 Rx1");
    sprintf(title[4],"UE2 Rx2");
    sprintf(title[5],"16QAM");
    sprintf(title[6],"16QAM");
    sprintf(title[7],"16QAM");
    sprintf(title[8],"16QAM");

    if(1){
        //sprintf(db_2,"%.3lf",4*(1-err_3/5000.0));

        for( int i = 0 ; i < 2 ; i ++ ){
            //renderText(-2.5+ i * 1.05 ,2.2,1.1,title[i]);
            if( i == 0 ){
                sprintf(db_2,"ThPt:%.3lf",1+(height2[i]));
            }else{
                sprintf(db_2,"%.3lf",1+(height2[i]));
            }
           renderText(-2.5+ i *1.6,-1.3,1.1,db_2);
            //comp_err[i] = 0;
        }
       // qDebug()<< "   comp_err:  "   << comp_err[0] << comp_err[1] << comp_err[2] << comp_err[3] << comp_err[4];

    }





}
void ThroughPut::myDrawPoint(double x,double y,double z,double r){//x,y,z, and  radium

        glBegin(GL_QUADS);
        glColor4f(0.8, 0.8, 0,1);//red
        glVertex3f(x,y-r/2,z-r);
        glVertex3f(x,y-r/2,z+r);
        glVertex3f(x,y+r/2,z+r);
        glVertex3f(x,y+r/2,z-r);
        glEnd();
}

void  ThroughPut::R_Adjust(double *tmp){
    *tmp -= rMove;
}
void  ThroughPut::L_Adjust(double *tmp){
    *tmp += 0.4;
}
void  ThroughPut::D_Adjust(double *tmp){
    *tmp -= 0.3;
}
void ThroughPut::wallplot(){
    double point2[8][3] = {{2,2,2},{-2,2,2},{-2,2,-2},{2,2,-2},{2,-1,6},{-2,-1,6},{-2,-1,-4},{2,-1,-4}};
    for(int i = 0 ; i < 4 ; i++){
        R_Adjust(&point2[i+4][2]);
        L_Adjust(&point2[i+4][2]);
        D_Adjust(&point2[i+4][0]);
    }

    glColor4f(0, 0, 0,0.2);
    glVertex3f(point2[4][0], point2[4][1], point2[4][2]);
    glVertex3f(point2[5][0]+2, point2[5][1], point2[5][2]);
    glVertex3f(point2[6][0]+2, point2[6][1], point2[6][2]);
    glVertex3f(point2[7][0], point2[7][1], point2[7][2]);
}

void ThroughPut::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up :
        xRot += 10;
        break;
    case Qt::Key_Left :
        yRot += 10;
        break;
    case Qt::Key_Right :
        zRot += 10;
        break;
    case Qt::Key_Down :
        translate -= 1;
        if (translate <= -20)
            translate = -1;
        break;
    }
    updateGL();
    QGLWidget::keyPressEvent(event);
}



struct pilot4id{
    int id;
    double value;
};

void ThroughPut::timerEvent(QTimerEvent *event){
    char buff[14404*3+10];
    struct pilot4id pilot42[1200];
    for( int i = 0 ; i < 200 ; i++ ){
        pilot42[i].id = i;
        pilot42[i].value = 0;
    }


    recvfrom(sockser_chl1_3,&buff,14404*3+10,0,(struct sockaddr *)&addrrcv_chl1_3,(socklen_t*)&size_chl2_3);//port :7005.3


    //qDebug() << "Counter is " << cnt_2++ << endl;

   //qDebug() << buff << endl;

    // inverse
    for( int i = 0 ; i < 2405 ; i ++){
       int position = i * 6;
       char tmp;
       //swap
       tmp = buff[position];
       buff[position] = buff[position+3];
       buff[position+3] = tmp;
       //swap
       position ++;
       tmp = buff[position];
       buff[position] = buff[position+3];
       buff[position+3] = tmp;
    }//for
    //qDebug() << buff << endl;

    int position = 12; // avoid the header a0aa 3c20 cccc
    for( int i = 0 ; i < 1200 ; i++){
        for( int j = 0 ; j <8 ;) {
            map1200_3[i][j++] = buff[position++];
            map1200_3[i][j++] = buff[position++];
            position++;//avoid the comma
       }//for j
    }//for i
    for( int i = 0 ; i < 1200 ; i++){
        data1_3[i][0]=hex2int(map1200_3[i][0],map1200_3[i][1],map1200_3[i][2],map1200_3[i][3]);
        data1_3[i][1]=hex2int(map1200_3[i][4],map1200_3[i][5],map1200_3[i][6],map1200_3[i][7]);
        //qDebug() << data1_3[i][0] << data1_3[i][1] <<data2_3[i][0] <<data2_3[i][1] <<endl;
    }//for i
    pdata = &data1_3[0][0];



    ber();



    updateGL();


}




int ThroughPut::hex2int(char a,char b,char c,char d)
{
    /*
      *   eg. ff to 1111 1111
      */

    //qDebug() << "abcd is : " << a << b << c << d << endl;
    int deca = 0,decb = 0,decc = 0,decd = 0;
    if( a >= 'a' && a <= 'f' ){
        deca  = 10 + a -'a';
    }else{
        deca = a - '0';
    }//else

    if( b >= 'a' && b <= 'f' ){
       decb  = 10 + b -'a';
    }else{
        decb = b - '0';
    }//else

    if( c >= 'a' && c <= 'f' ){
        decc  = 10 + c -'a';
    }else{
        decc = c - '0';
    }//else

    if( d >= 'a' && d <= 'f' ){
       decd  = 10 + d -'a';
    }else{
        decd = d - '0';
    }//else

  //qDebug() << "dec abcd is : " << deca << decb << decc << decd << endl;

   int num[16] = {0};
   num[12] = deca % 2;
   deca /= 2;
   num[13] = deca % 2;
   deca /= 2;
   num[14] = deca % 2;
   deca /= 2;
   num[15] = deca % 2;
   deca /= 2;

   num[8] = decb % 2;
   decb /= 2;
   num[9] = decb % 2;
   decb /= 2;
   num[10] = decb % 2;
   decb /= 2;
   num[11] = decb % 2;
   decb /= 2;

   num[4] = decc % 2;
   decc /= 2;
   num[5] = decc % 2;
   decc /= 2;
   num[6] = decc % 2;
   decc /= 2;
   num[7] = decc % 2;
   decc /= 2;

   num[0] = decd % 2;
   decd /= 2;
   num[1] = decd % 2;
   decd /= 2;
   num[2] = decd % 2;
   decd /= 2;
   num[3] = decd % 2;
   decd /= 2;

   int flag = 1;
   if(num[15] == 1){
        flag = -1;
        for( int i = 0 ; i < 15 ; i++ ){
            num[i] = 1 - num[i];
        }
        num[0] ++;
        for( int i = 0 ; i <15 ; i++ ){
            if( num[i] > 1){
                num[i] = 0;
                num[i+1]++;
            }//if
        }
   }
   int res = 0;
   int base = 1;
   for( int i = 0 ; i < 15 ; i++ ){
       res += num[i] *base;
       base *= 2;
   }


   //for i
  // qDebug() << "16 bit is : "  << num[15] << num[14] << num[13] << num[12] << num[11] << num[10] << num[9] << num[8]<< num[7] << num[6]<< num[5] << num[4]<< num[3] << num[2]<< num[1] << num[0] << endl;
   res *= flag;
   //qDebug() << res << endl;
    return res;
}
void ThroughPut::InputManagement(){
    QFile file("./stream2_re.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
        qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];


     file.readLine(str,301);


    //qDebug() << "last one is :"<< qpsk[9][0] ;
    file.close();

    //QFile file2("./Pilot_send_data/pilot_1200_im.txt");
    QFile file2("./stream2_im.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file2.errorString();
        qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;
    }
    //qDebug() << QObject::tr("file text:") << endl << file2.readAll();
    //char str[100];


    file2.readLine(str_im,301);



    qDebug() <<"str is :"<< str <<  strlen(str) <<endl;
    qDebug() <<"str_im is :"<< str_im <<strlen(str_im) <<endl;


    file2.close();

    QFile file3("./stream1_re.txt");
    if (!file3.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file3.errorString();
        qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;
    }
    file3.readLine(s1_re,301);
    file3.close();

    QFile file4("./stream1_im.txt");
    if (!file4.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file4.errorString();
        qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;
    }
    file4.readLine(s1_im,301);
    file4.close();

}

double ThroughPut::char2double(char *str){
    double  ans = 0;

    if(str[0] == '-'){
        if(str[1] == '3')
            return -3/sqrt(10);
        else if(str[1] == '1')
            return -1/sqrt(10);
    }
    else{
        if(str[0] == '3')
            return 3/sqrt(10);
        else if(str[0] == '1')
            return 1/sqrt(10);
    }
    return ans;
}
double ThroughPut::char2int(char *str){
    /*
     *  eg. 2.789 -2.789
     */
    double ans = 0;

    if(str[0] == '-'){
        return -1.0/sqrt(2);
    }
    else{
       return  1.0/sqrt(2);
    }
    return ans;
}
void ThroughPut::myDrawStars(){

    for( int i = 0 ; i < 1000 ; i++ ){
        double z = (*(pdata+i*2+0));
        double y = *(pdata+i*2+1);


        //qDebug()<< "in my DrawStars, x ,y is " << z<<" , "<<y<<endl;
        myDrawPoint(-0.2,y+0.5,z*2,0.01);
    }


}
int ThroughPut::ber(){

    //qDebug() << "start";
    for( int i = 0 ; i < 300  ; i++ ){
        double y = (*(pdata+i*2+0));
        double z = *(pdata+i*2+1);

        //qDebug() << z << qam16[i][0] << "  |  " <<y<<qam16[i][1]<<"|"<<(diff)<<"|"<<1/sqrt(10);
        if( ( y>0 && s1_re[i]=='-' )  ||  ( y<0 && s1_re[i]=='+' ) || ( z>0 && s1_im[i]=='-' ) || ( z<0 && s1_im[i]=='+' ) ){
              err_0++;
        }
        bit_cnt_0++;
        if(bit_cnt_0 == 2000){
            qDebug() << "BER :" <<err_0/2000.0 ;
            ber_rate_0 = err_0/2000.0;
             err_0 = 0;
             bit_cnt_0 = 0;
        }

       // qDebug()<< "in my DrawStars, x ,y is " << z<<" , "<<y<<endl;

    }



    for( int i = 300 ; i < 600  ; i++ ){
        double y = (*(pdata+i*2+0));
        double z = *(pdata+i*2+1);

        //qDebug() << z << qam16[i][0] << "  |  " <<y<<qam16[i][1]<<"|"<<(diff)<<"|"<<1/sqrt(10);
        if( ( y>0 && str[i-300]=='-' )  ||  ( y<0 && str[i-300]=='+' ) || ( z>0 && str_im[i-300]=='-' ) || ( z<0 && str_im[i-300]=='+' ) ){
              err++;
        }
        bit_cnt++;
        if(bit_cnt == 2000){
            qDebug() << "BER :" <<err/2000.0 ;
            ber_rate = err/2000.0;
             err = 0;
             bit_cnt = 0;
        }

       // qDebug()<< "in my DrawStars, x ,y is " << z<<" , "<<y<<endl;

    }
}

void  ThroughPut::Draw_line(){
    double point2[8][3] = {{2,2,2},{-2,2,2},{-2,2,-2},{2,2,-2},{2,-1,6},{-2,-1,6},{-2,-1,-4},{2,-1,-4}};
    for(int i = 0 ; i < 4 ; i++){
        L_Adjust(&point2[i+4][2]);
        D_Adjust(&point2[i+4][0]);
    }
    glLineWidth(3);

       int crr = 6;
    double step = 10.0/num_p;
    double st = point2[crr][2];


    glBegin(GL_LINE_STRIP); // 用折线绘
    glColor4f(1, 143.0/255.0, 50.0/255.0,1);
    for(int i = 0 ; i < num_p/5.0 ; i++){
        glVertex3f(1+*(pdata2_3+i)/5.0, point2[crr][1]+0.1  , point2[crr][2]);
        //point2[crr][0] = 0.3*sin(point2[crr][2])+(qrand() % 10)/100.0;
        point2[crr][2] += step*5;
    }
    glEnd();



    glLineWidth(1);


    point2[crr][2] = st;
    glBegin(GL_LINE_STRIP); // 用折线绘
    glColor4f(0,0,1,1);
    for(int i = 0 ; i < num_p/5.0 ; i++){
        glVertex3f(point2[crr][0]+2.3, point2[crr][1]+0.1  , point2[crr][2]);
        //point2[crr][0] = 0.3*sin(point2[crr][2])+(qrand() % 10)/100.0;
        point2[crr][2] += step*5;
    }
    glEnd();
}
