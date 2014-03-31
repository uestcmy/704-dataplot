#include <ber_bar.h>
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
#include "ui_ber_bar.h"
socklen_t size_chl2_2;
int cnt_2 = 0;
sockaddr_in addrSrv_chl1_2,addrrcv_chl1_2;
int sockser_chl1_2;
 double pilot3[1200][2] = {0};
 double qam16_2[500][2] = {0};
char map1200_2[1200][8];
double data1_2[1800][2] = {0};//complex number real and image
double data2_2[1800][2] = {0};
int err_2 = 0;
int bit_cnt_2_2 = 0;
char db[20] = {0};



BER_BAR::BER_BAR(QWidget *parent) :
    QGLWidget(parent),
    ui(new Ui::BER_BAR)
{

    static double ttt[1000] = {0};
    pdata2_2 = &ttt[0];

    ui->setupUi(this);
    translate = -6.0;
    xRot = yRot = zRot = 0.0;
    xRot += 0;
    yRot += 0;
    num_p = 300;

    InputManagement();
    size_chl2_2=sizeof(sockaddr_in);
    sockser_chl1_2=socket(AF_INET,SOCK_DGRAM,0);
    addrSrv_chl1_2.sin_addr.s_addr=htonl(INADDR_ANY);
    addrSrv_chl1_2.sin_family=AF_INET;
    addrSrv_chl1_2.sin_port=htons(7007);//server : receive port number
    bind(sockser_chl1_2,(sockaddr*)&addrSrv_chl1_2,sizeof(sockaddr));

    id1 = startTimer(100);


}
BER_BAR::~BER_BAR(){
   delete ui;
}

void BER_BAR::initializeGL()
{
    glClearColor(0.8,0.8,0.8,0);//canvas
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);


}
void BER_BAR::resizeGL(int w, int h)
{
    glViewport(-10, 0, (GLint)w, (GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //luPerspective(40.0, (GLfloat)w/(GLfloat)h, 2, 30.0);
    //gluPerspective(40,1.33, 2, 30.0);
    //glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 2.3* ( GLfloat ) w / ( GLfloat ) h, -2, 2, -15.0, 15.0);
     //glFrustum (-1.5* ( GLfloat ) w / ( GLfloat ) h, 2* ( GLfloat ) w / ( GLfloat ) h, -2, 1.3, 6, 10.0);
     glOrtho (-1.2 * ( GLfloat ) w / ( GLfloat ) h, 2.3* ( GLfloat ) w / ( GLfloat ) h, -2, 2, -10.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//
void BER_BAR::paintGL()
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

    if(bit_cnt_2_2 == 19500 ){
        sprintf(db,"%lf",err_2/20000.0);
    }
    renderText(0.3,-0.3,1,db);
}

void BER_BAR::Bar_ber(){

#ifdef connected
    double height = *(pdata2_2) * 2;
#endif
#ifdef standardalone
    double height = 1.5;
#endif

    //qDebug() <<  " height is :" << height;

     /*axis*/
    glLineWidth(3);
    //glBegin(GL_LINE_STRIP);
    glBegin(GL_LINE_LOOP);
    glColor4f(0.5,0,1,0.8);
    glVertex2f(-0.5,2.2); //left up
    glVertex2f(-0.5,0);// left,down
    glVertex2f(1.5,0);//right down
    glVertex2f(1.5,2.2);//right up
    glEnd();
    /*bar*/
    glBegin(GL_QUADS);
    glColor4f(1,0,0,0.5);
    glVertex2f(0,0);//x,y
    glVertex2f(0,height);
    glVertex2f(1,height);
    glVertex2f(1,0);
    glEnd();

    /*error == 1 */
    glColor3f(1,0,0);
    renderText(-0.8,2,1,"100%");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,2);
    glVertex2f(1.5,2);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-0.8,1.5,1,"75%");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,1.5);
    glVertex2f(1.5,1.5);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-0.8,1,1,"50%");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,1);
    glVertex2f(1.5,1);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-0.8,0.5,1,"25%");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(1.5,0.5);//x,y
    glEnd();



}
void BER_BAR::myDrawPoint(double x,double y,double z,double r){//x,y,z, and  radium

        glBegin(GL_QUADS);
        glColor4f(0.8, 0.8, 0,1);//red
        glVertex3f(x,y-r/2,z-r);
        glVertex3f(x,y-r/2,z+r);
        glVertex3f(x,y+r/2,z+r);
        glVertex3f(x,y+r/2,z-r);
        glEnd();
}

void  BER_BAR::R_Adjust(double *tmp){
    *tmp -= rMove;
}
void  BER_BAR::L_Adjust(double *tmp){
    *tmp += 0.4;
}
void  BER_BAR::D_Adjust(double *tmp){
    *tmp -= 0.3;
}
void BER_BAR::wallplot(){
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

void BER_BAR::keyPressEvent(QKeyEvent *event)
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



struct pilot3id{
    int id;
    double value;
};

void BER_BAR::timerEvent(QTimerEvent *event){
    char buff[14404*3+10];
    struct pilot3id pilot32[1200];
    for( int i = 0 ; i < 200 ; i++ ){
        pilot32[i].id = i;
        pilot32[i].value = 0;
    }

 #ifdef connected
    recvfrom(sockser_chl1_2,&buff,14404*3+10,0,(struct sockaddr *)&addrrcv_chl1_2,(socklen_t*)&size_chl2_2);//port :7005.3


    //qDebug() << "Counter is " << cnt_2++ << endl;
    /*
    qDebug() << buff << endl;
*/
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
            map1200_2[i][j++] = buff[position++];
            map1200_2[i][j++] = buff[position++];
            position++;//avoid the comma
       }//for j
    }//for i
    for( int i = 0 ; i < 1200 ; i++){
        data1_2[i][0]=hex2int(map1200_2[i][0],map1200_2[i][1],map1200_2[i][2],map1200_2[i][3]);
        data1_2[i][1]=hex2int(map1200_2[i][4],map1200_2[i][5],map1200_2[i][6],map1200_2[i][7]);
        //qDebug() << data1_2[i][0] << data1_2[i][1] <<data2_2[i][0] <<data2_2[i][1] <<endl;
    }//for i

    double pilot3H11[200][2] = {0};
    int cnt_2H11 = 0;
    for( int i = 0 ; i < 1200 ; i++){
        if(i % 6 == 0){//position for the pilot3 H11
            pilot3H11[cnt_2H11][0] = data1_2[i][0];
            pilot3H11[cnt_2H11][1] = data1_2[i][1];
            pilot32[cnt_2H11].value = sqrt(data1_2[i][0]*data1_2[i][0]+data1_2[i][1]*data1_2[i][1]);
            cnt_2H11 += 1;
        }//if
    }
    for( int i = 0 ; i < 200 ; i++){
        for( int j = i+1 ; j < 200 ; j++){
            if( pilot32[i].value - pilot32[j].value  < 1e-7 ){
                struct pilot3id  tmp = pilot32[i];
                pilot32[i] = pilot32[j];
                pilot32[j] = tmp;
            }
        }
    }


    double H11[200][2] = {0};
    double absH11[200] = {0};
    double absH11_6[1200][2] = {0};


    //qDebug() << "  ----  ";
    for( int i = 0 ; i < 200 ; i++ ){

        H11[i][0] = (pilot3H11[i][0]*pilot3[i*6][0]+pilot3H11[i][1]*pilot3[i*6][1])/(pilot3[i*6][0]*pilot3[i*6][0]+pilot3[i*6][1]*pilot3[i*6][1]);
        H11[i][1] = (pilot3H11[i][1]*pilot3[i*6][0]-pilot3H11[i][0]*pilot3[i*6][1])/(pilot3[i*6][0]*pilot3[i*6][0]+pilot3[i*6][1]*pilot3[i*6][1]);
        //qDebug() << H11[i][0] << H11[i][1]  << "  angle :  "<<atan(H11[i][1]/H11[i][0])<<"pilot3"<<pilot3[i*6][0]<<"|"<<pilot3[i*6][1];

        //qDebug() << "  angle :  "<<atan(H11[i][1]/H11[i][0]);
        absH11[i] = sqrt(H11[i][0]*H11[i][0] + H11[i][1]*H11[i][1]);

    }//for i

    //interp :edited by hh
    for(int i=0;i<200;i++){
        absH11_6[i*6][0] =  H11[i][0];
        absH11_6[i*6 + 1 ][0] =  H11[i][0];
        absH11_6[i*6 + 2 ][0] =  H11[i][0];
        absH11_6[i*6 + 3][0]=  H11[i][0];
        absH11_6[i*6 + 4 ][0] =  H11[i][0];
        absH11_6[i*6 + 5 ][0] =  H11[i][0];

        absH11_6[i*6][1] =  H11[i][1];
        absH11_6[i*6 + 1 ][1] =  H11[i][1];
        absH11_6[i*6 + 2 ][1] =  H11[i][1];
        absH11_6[i*6 + 3][1]=  H11[i][1];
        absH11_6[i*6 + 4 ][1] =  H11[i][1];
        absH11_6[i*6 + 5 ][1] =  H11[i][1];
        absH11_6[i*6 + 6 ][1] =  H11[i][1];


    }


    // qDebug() << "  H : ---- ";
    int cnt_2g = 0;
    double guess[1000][2] = {0};
    for( int i = 0 ; i < 1200 ; i++){
        if(i%6 != 0){
            // qDebug() << "  H :  "<<absH11_6[i][0];
            guess[cnt_2g][0] = (data1_2[i][0]*absH11_6[i][0]+data1_2[i][1]*absH11_6[i][1])/(absH11_6[i][0]*absH11_6[i][0]+absH11_6[i][1]*absH11_6[i][1]);
            //guess[cnt_2g][0] /= 15000;

            guess[cnt_2g][1] = (data1_2[i][1]*absH11_6[i][0]-data1_2[i][0]*absH11_6[i][1])/(absH11_6[i][0]*absH11_6[i][0]+absH11_6[i][1]*absH11_6[i][1]);
            //guess[cnt_2g][1]  /= 15000;
            cnt_2g++;
        }//if
    }//for
    pdata = &guess[0][0];




    //*(pdata) = 2*sin(cnt_2_update);




    ber();

#endif
    updateGL();


}

int BER_BAR::hex2int(char a,char b,char c,char d)
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
void BER_BAR::InputManagement(){
    //QFile file("datain.txt");

    //QFile file("./pilot3_send_data/pilot3_1200_re.txt");
    QFile file("./16qam_data_new/data_im.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
        qDebug() << "Last sentence is in BER_BAR Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];
    char str[100];
    for( int i = 0 ; i < 1200 ; i++ ){
        file.readLine(str,100);
        double crr = 0;
        crr = char2int(str);
        //qDebug() << "pilot3[ "<< i <<"][0] is :"<<crr;
        pilot3[i][0] = crr;
    }

    //qDebug() << "last one is :"<< BER_BAR[9][0] ;
    file.close();

    //QFile file2("./pilot3_send_data/pilot3_1200_im.txt");
    QFile file2("./16qam_data_new/data_re.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file2.errorString();
        qDebug() << "Last sentence is in BER_BAR Inputmanagement." << endl;
    }
    //qDebug() << QObject::tr("file text:") << endl << file2.readAll();
    //char str[100];
    for( int i = 0 ; i < 1200 ; i++ ){
        file2.readLine(str,100);
        double crr = 0;
        crr = char2int(str);
        //qDebug() << "pilot3[ "<< i <<"][1] is :"<<crr;
        pilot3[i][1] = crr;
    }


    //qDebug() <<"apsk[0][0] is :"<< *(pdata) << endl;
    file2.close();


   // QFile file3("./16qam_data/data_re.txt");
    QFile file3("./16qam_data_new/data_re.txt");
    if (!file3.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file3.errorString();
        qDebug() << "Last sentence is in BER_BAR Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];
    int cnt_2_500=0;
    for( int i = 0 ; i < 600 ; i++ ){
        file3.readLine(str,100);
        double crr = 0;
        if(i%6!=0){
        crr = char2double(str);
       // qDebug() << "re[ "<< i <<"][0] is :"<<crr;
        qam16_2[cnt_2_500][0] = crr;
        cnt_2_500++;
        }
    }

    //qDebug() << "last one is :"<< BER_BAR[9][0] ;
    file3.close();

   // QFile file4("./16qam_data/data_im.txt");
    QFile file4("./16qam_data_new/data_im.txt");
    if (!file4.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file4.errorString();
        qDebug() << "Last sentence is in BER_BAR Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];
    int cnt_2_500_2;
    for( int i = 0 ; i < 600 ; i++ ){
        file4.readLine(str,100);
        double crr = 0;
        if(i%6!=0){
        crr = char2double(str);
        //qDebug() << "im[ "<< i <<"][0] is :"<<crr;
        qam16_2[cnt_2_500_2][1] = crr;
        cnt_2_500_2++;
        }
    }

    //qDebug() << "last one is :"<< BER_BAR[9][0] ;
    file4.close();

}

double BER_BAR::char2double(char *str){
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
double BER_BAR::char2int(char *str){
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
void BER_BAR::myDrawStars(){

    for( int i = 0 ; i < 1000 ; i++ ){
        double z = (*(pdata+i*2+0));
        double y = *(pdata+i*2+1);


        //qDebug()<< "in my DrawStars, x ,y is " << z<<" , "<<y<<endl;
        myDrawPoint(-0.2,y+0.5,z*2,0.01);
    }


}
int BER_BAR::ber(){

    //qDebug() << "start";
    for( int i = 0 ; i < 500 ; i++ ){
        double y = (*(pdata+i*2+0));
        double z = *(pdata+i*2+1);
        double diff = sqrt( (z-qam16_2[i][0])*(z-qam16_2[i][0])+(y - qam16_2[i][1])*(y - qam16_2[i][1])  );

        //qDebug() << z << qam16_2[i][0] << "  |  " <<y<<qam16_2[i][1]<<"|"<<(diff)<<"|"<<1/sqrt(10);
        if(diff> 1/sqrt(10)){
              err_2++;
        }
        bit_cnt_2_2++;

        if(bit_cnt_2_2 == 20000){
            qDebug() << "BER :" <<err_2/20000.0 ;
            for( int i = num_p-1 ; i >= 1 ; i-- ){
                *(pdata2_2+i) = *( pdata2_2+i-1);
            }
            if(err_2 -0 <1e-7){
                 *(pdata2_2) = 0 ;
            }else{
                 *(pdata2_2) =   err_2/20000.0  ;
            }
             err_2 = 0;
             bit_cnt_2_2 = 0;
        }
       // qDebug()<< "in my DrawStars, x ,y is " << z<<" , "<<y<<endl;

    }
}

void  BER_BAR::Draw_line(){
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
        glVertex3f(1+*(pdata2_2+i)/5.0, point2[crr][1]+0.1  , point2[crr][2]);
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
