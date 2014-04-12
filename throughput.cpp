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
#include "chol_inv_4.cpp"
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
int flag_bar = 0;
int  cnt_pilot = 0;

double cnt_allstar1 = 0;
double cnt_allstar2 = 0;

double cnt_err_g1 = 0;
double cnt_err_g[8] ={0};

double comp_err_1 = 0;
double comp_err[10] = {0};

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
    addrSrv_chl1_3.sin_port=htons(7008);//server : receive port number
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
     glOrtho (-1.7* ( GLfloat ) w / ( GLfloat ) h, 1.5* ( GLfloat ) w / ( GLfloat ) h, -2, 2, -12.0, 15.0);
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
        height2[0] =  -1 + (1-*(pdata2_3))*2;
        comp_err[0] =  *(pdata2_3);

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
    glVertex2f(2.8,-1);//right down
    glVertex2f(2.8,2);//right up  [-3 2.8 2 -1]
    glEnd();
    /*bar*/
   double pos = 0;

    for( int i = 0 ; i < 5 ; i++ ){
    glBegin(GL_QUADS);
    glColor4f(1,i/9.0,0,0.5);
    glVertex2f(-1.8+pos,-1);//x,y
    glVertex2f(-1.8+pos,height2[i]);
    glVertex2f(-2.5+pos,height2[i]);
    glVertex2f(-2.5+pos,-1);
     glEnd();
     pos  = (i+1)*1.005;
    }
    /*error == 1 */

    glColor3f(1,0,0);
    renderText(-3.5,2.2,1,"bps/Hz");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,1);
    glVertex2f(2.8,1);//x,y
    glEnd();
    glColor3f(1,0,0);
    renderText(-3.5,1,1,"4");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,0.5);
    glVertex2f(2.8,0.5);//x,y
    glEnd();
    glColor3f(1,0,0);
    renderText(-3.5,0.5,1,"3");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,0);
    glVertex2f(2.8,0);//x,y
    glEnd();
    glColor3f(1,0,0);
    renderText(-3.5,0,1,"2");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-3,-0.5);
    glVertex2f(2.8,-0.5);//x,y
    glEnd();

    glColor3f(1,0,0);
    renderText(-3.5,-0.5,1,"1");
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    glColor4f(0,0,1,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(1.5,0.5);//x,y
    glEnd();


    if(bit_cnt_2_3 == 19500 ){
        sprintf(db_2,"%.3lf",4*(1-err_3/20000.0));
    }
    char title[9][50];
    sprintf(title[0],"16QAM");
    sprintf(title[1],"Rx1");
    sprintf(title[2],"Rx2");
    sprintf(title[3],"Rx3");
    sprintf(title[4],"Rx4");
    sprintf(title[5],"16QAM");
    sprintf(title[6],"16QAM");
    sprintf(title[7],"16QAM");
    sprintf(title[8],"16QAM");

    for( int i = 1 ; i < 5 ; i++ ){
        comp_err[i] = comp_err[i+4];
        height2[i] = height2[i+4];
    }

    int scale = 2;
    for( int i = 0 ; i < 5 ; i ++ ){
        renderText(-2.5+ i * 1.05 ,2.2,1.1,title[i]);
        if( i == 0 ){
            sprintf(db_2,"%.3lf",(1-comp_err[i])*4);
        }else{
            sprintf(db_2,"%.3lf",(1-comp_err[i])*2);
        }
        renderText(-2.5+ i *1.05,-1.3,1.1,db_2);
        //comp_err[i] = 0;
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

 #ifdef connected
    recvfrom(sockser_chl1_3,&buff,14404*3+10,0,(struct sockaddr *)&addrrcv_chl1_3,(socklen_t*)&size_chl2_3);//port :7005.3


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

    double pilot4H11[200][2] = {0};
    int cnt_2H11 = 0;
    for( int i = 0 ; i < 1200 ; i++){
        if(i % 6 == 0){//position for the pilot4 H11
            pilot4H11[cnt_2H11][0] = data1_3[i][0];
            pilot4H11[cnt_2H11][1] = data1_3[i][1];
            pilot42[cnt_2H11].value = sqrt(data1_3[i][0]*data1_3[i][0]+data1_3[i][1]*data1_3[i][1]);
            cnt_2H11 += 1;
        }//if
    }
    for( int i = 0 ; i < 200 ; i++){
        for( int j = i+1 ; j < 200 ; j++){
            if( pilot42[i].value - pilot42[j].value  < 1e-7 ){
                struct pilot4id  tmp = pilot42[i];
                pilot42[i] = pilot42[j];
                pilot42[j] = tmp;
            }
        }
    }


    double H11[200][2] = {0};
    double absH11[200] = {0};
    double absH11_6[1200][2] = {0};


    //qDebug() << "  ----  ";
    for( int i = 0 ; i < 200 ; i++ ){

        H11[i][0] = (pilot4H11[i][0]*pilot4[i*6][0]+pilot4H11[i][1]*pilot4[i*6][1])/(pilot4[i*6][0]*pilot4[i*6][0]+pilot4[i*6][1]*pilot4[i*6][1]);
        H11[i][1] = (pilot4H11[i][1]*pilot4[i*6][0]-pilot4H11[i][0]*pilot4[i*6][1])/(pilot4[i*6][0]*pilot4[i*6][0]+pilot4[i*6][1]*pilot4[i*6][1]);
        //qDebug() << H11[i][0] << H11[i][1]  << "  angle :  "<<atan(H11[i][1]/H11[i][0])<<"pilot4"<<pilot4[i*6][0]<<"|"<<pilot4[i*6][1];

        //qDebug() << "  angle :  "<<atan(H11[i][1]/H11[i][0]);
        absH11[i] = sqrt(H11[i][0]*H11[i][0] + H11[i][1]*H11[i][1]);

    }//for icnt_err_g

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
            guess[cnt_2g][0] = (data1_3[i][0]*absH11_6[i][0]+data1_3[i][1]*absH11_6[i][1])/(absH11_6[i][0]*absH11_6[i][0]+absH11_6[i][1]*absH11_6[i][1]);
            //guess[cnt_2g][0] /= 15000;

            guess[cnt_2g][1] = (data1_3[i][1]*absH11_6[i][0]-data1_3[i][0]*absH11_6[i][1])/(absH11_6[i][0]*absH11_6[i][0]+absH11_6[i][1]*absH11_6[i][1]);
            //guess[cnt_2g][1]  /= 15000;
            cnt_2g++;
        }//if
    }//for
    pdata = &guess[0][0];




    //*(pdata) = 2*sin(cnt_2_update);




    ber();

    recvfrom(sockser_chl1_4,&buff,LENGTH_OF_OFDM*3+10,0,(struct sockaddr *)&addrrcv_chl1_4,(socklen_t*)&size_chl2_4);//port :7021


   // qDebug() << "Counter is " << cnt++ << endl;

   // qDebug() << buff << endl;
    for( int i = 0 ; i < 2400 ; i ++){
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

    position = 12; // avoid the header a0aa 3c20 cccc
    for( int i = 0 ; i < 1200 ; i++){
        for( int j = 0 ; j <8 ;) {
            map1200[i][j++] = buff[position++];
            map1200[i][j++] = buff[position++];
            position++;//avoid the comma
       }//for j
    }//for i
    for( int i = 0 ; i < 1200 ; i++){
        data1[i][0]=hex2int(map1200[i][0],map1200[i][1],map1200[i][2],map1200[i][3]);
        data1[i][1]=hex2int(map1200[i][4],map1200[i][5],map1200[i][6],map1200[i][7]);
       // qDebug() << data1[i][0] << data1[i][1] ;//<<data2[i][0] <<data2[i][1] <<endl;
    }//for i

    sys_function();//data >>   function >> HWS

#endif
    sys_function();//data >>   function >> HWS

    updateGL();


}




void ThroughPut::sys_function(){
    // (2+i) *(3-i)
    /*
    double re,im;
    mult(2,1,3,-1,&re, &im);
    qDebug() << " re , im : " << re  << im;
*/


    double mat48_1_re[4][8];
    double mat48_1_im[4][8];

    double mat48_2_re[4][8];
    double mat48_2_im[4][8];

    double mat48_tmp_re[4][8];
    double mat48_tmp_im[4][8];

    double mat84_tmp_re[8][4];
    double mat84_tmp_im[8][4];

    double mat44_tmp_re[4][4] = {0};
    double mat44_tmp_im[4][4] = {0};

    double mat44_inv_re[4][4];
    double mat44_inv_im[4][4];

    double w84_re[8][4];
    double w84_im[8][4];

    double hw44_re[4][4];
    double hw44_im[4][4];
/*
    FILE *fp1,*fp2;
    fp2 = fopen("data.txt","r");
     fp1 = fopen("data2out.txt","w");
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 8 ; j++ ){
            fscanf(fp2,"%lf",&mat48_1_re[i][j]);
        }
    }
    for( int i = 0 ; i < 4 ; i++){
        for( int j = 0 ; j < 8 ; j++ ){
            fscanf(fp2,"%lf",&mat48_1_im[i][j]);
        }
    }
    fclose(fp2);
    */


    for( int t = 0 ; t < 3 ; t++){//time
        for(int f = 0 ; f < 4 ; f ++){//freq
            // get data t1
            for (int i = 0;i<4;i++){
                for(int j=0;j<8;j++){
                    mat48_1_re[i][j]=data1[i*256+j + 64*t + 8*f ][0];
                    mat48_1_im[i][j]=data1[i*256+j + 64*t + 8*f  ][1];
                }
            }
            // get data t2
            for (int i = 0;i<4;i++){
                for(int j=0;j<8;j++){
                    mat48_2_re[i][j]=data1[i*256+j + 64*t+ 64 + 8*f ][0];
                    mat48_2_im[i][j]=data1[i*256+j + 64*t  + 64 + 8*f  ][1];
                }
            }
            hermitian( 4,8,mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im );
            Matrix_mult484(mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im, mat44_tmp_re,mat44_tmp_im);
            chol_inv(mat44_tmp_re,mat44_tmp_im,mat44_inv_re,mat44_inv_im);
            Matrix_mult844(mat84_tmp_re,mat84_tmp_im, mat44_inv_re,mat44_inv_im, w84_re,w84_im);
            Matrix_mult484(mat48_2_re,mat48_2_im,w84_re,w84_im,hw44_re,hw44_im);
            // jiu xiangpian
            double alpha=0;

            for (int i=0;i<4;i++){
                alpha += 0.25 * atan(hw44_im[i][i]/hw44_re[i][i]);

            }
            double hw2_44_re[4][4];
            double hw2_44_im[4][4];
            for(int i = 0 ; i < 4 ; i++ ){
                for( int j = 0 ; j < 4 ; j++ ){
                    mult(cos(alpha),-sin(alpha),hw44_re[i][j],hw44_im[i][j],&hw2_44_re[i][j],&hw2_44_im[i][j]);
                }
            }
            //get x
            double x_re[4][1];
            double x_im[4][1];
            double y41_re[4][1];
            double y41_im[4][1];
            for( int ip = 0 ; ip < 4 ; ip++ ){
                x_re[ip][0] = pilot2[cnt_pilot][0];
                x_im[ip][0] = pilot2[cnt_pilot++][1];
                if(cnt_pilot == 1200){
                    cnt_pilot = 0;
                }
            }
            //y = hw*x
            Matrix_mult441(hw2_44_re,hw2_44_im,x_re,x_im,y41_re,y41_im);
/*
            for( int i = 0 ; i < 4 ; i++ ){
                qDebug() << hw2_44_re[i][0]  << hw2_44_re[i][1]  << hw2_44_re[i][2]  << hw2_44_re[i][3]  ;
            }qDebug() << "\n";
*/

           cnt_allstar1 ++;
           if( ( (y41_re[0][0] -x_re[0][0] )*(y41_re[0][0] -x_re[0][0]) + (y41_im[0][0] -x_im[0][0])*(y41_im[0][0] -x_im[0][0]) ) >0.5 ){
                cnt_err_g1++;
           }

           for( int i = 0 ; i < 4 ; i ++ ){
               if( ( (y41_re[i][0] -x_re[i][0] )*(y41_re[i][0] -x_re[i][0]) + (y41_im[i][0] -x_im[i][0])*(y41_im[i][0] -x_im[i][0]) ) >0.5 ){
                    cnt_err_g[i]++;
               }
           }

           if( cnt_allstar1 == 500){
               for( int i = 1 ; i <= 4 ; i++ ){
                   //comp_err_1 = cnt_err_g[i] / cnt_allstar1;
                   comp_err[i] = cnt_err_g[i-1] / cnt_allstar1;

                   height2[i] = -1+ (1-comp_err[i]) ;
                   //height2[i] = -1+ i*0.3 ;

               }
               //height2[0] =  -1 + ( 1 - cnt_err_g1/cnt_allstar1 );
                   comp_err[0] = 1-  cnt_err_g1/cnt_allstar1;
                   comp_err[5]  = comp_err[1];
                   comp_err[6]  = comp_err[2];
               height2[5] = height2[1] ;
               height2[6] = height2[2] ;


                 qDebug() << " comp err : " << cnt_err_g1;
                   qDebug() << " yre : " << y41_re[0][0]   << " yim : " << y41_im[0][0]  ;
                   qDebug() << " xre : " << x_re[0][0]   << " xim : " << x_im[0][0]  ;
                 cnt_allstar1 = 0;
                 cnt_err_g1 = 0;
                 cnt_err_g[0] = 0;cnt_err_g[1] =1;cnt_err_g[2] = 0;cnt_err_g[2] = 0;cnt_err_g[3] = 0;

           }

            new_star[cnt_newstar][0] = y41_re[0][0];
            new_star[cnt_newstar++][1] = y41_im[0][0];

            if(cnt_newstar == 120){
                cnt_newstar = 0;
            }
        }
    }




    for( int t = 0 ; t < 3 ; t++){//time
        for(int f = 0 ; f < 4 ; f ++){//freq
            // get data t1
            for (int i = 0;i<4;i++){
                for(int j=0;j<8;j++){
                    mat48_1_re[i][j]=data1[i*256+j + 64*t + 8*f  +32][0];//s1 ue2
                    mat48_1_im[i][j]=data1[i*256+j + 64*t + 8*f  +32][1];
                }
            }
            // get data t2
            for (int i = 0;i<4;i++){
                for(int j=0;j<8;j++){
                    mat48_2_re[i][j]=data1[i*256+j + 64*t+ 64 + 8*f  +32][0];
                    mat48_2_im[i][j]=data1[i*256+j + 64*t  + 64 + 8*f  +32][1];
                }
            }
            hermitian( 4,8,mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im );
            Matrix_mult484(mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im, mat44_tmp_re,mat44_tmp_im);
            chol_inv(mat44_tmp_re,mat44_tmp_im,mat44_inv_re,mat44_inv_im);
            Matrix_mult844(mat84_tmp_re,mat84_tmp_im, mat44_inv_re,mat44_inv_im, w84_re,w84_im);
            Matrix_mult484(mat48_2_re,mat48_2_im,w84_re,w84_im,hw44_re,hw44_im);
            // jiu xiangpian
            double alpha=0;

            for (int i=0;i<4;i++){
                alpha += 0.25 * atan(hw44_im[i][i]/hw44_re[i][i]);

            }
            double hw2_44_re[4][4];
            double hw2_44_im[4][4];
            for(int i = 0 ; i < 4 ; i++ ){
                for( int j = 0 ; j < 4 ; j++ ){
                    mult(cos(alpha),-sin(alpha),hw44_re[i][j],hw44_im[i][j],&hw2_44_re[i][j],&hw2_44_im[i][j]);
                }
            }
            //get x
            double x_re[4][1];
            double x_im[4][1];
            double y41_re[4][1];
            double y41_im[4][1];
            for( int ip = 0 ; ip < 4 ; ip++ ){
                x_re[ip][0] = pilot2[cnt_pilot][0];
                x_im[ip][0] = pilot2[cnt_pilot++][1];
                if(cnt_pilot == 1200){
                    cnt_pilot = 0;
                }
            }
            //y = hw*x
            Matrix_mult441(hw2_44_re,hw2_44_im,x_re,x_im,y41_re,y41_im);

            new_star[cnt_newstar][0] = y41_re[2][0];
            new_star[cnt_newstar++][1] = y41_im[2][0];

            if(cnt_newstar == 120){
                cnt_newstar = 0;
            }

            cnt_allstar2 ++;
            if( ( (y41_re[0][0] -x_re[0][0] )*(y41_re[0][0] -x_re[0][0]) + (y41_im[0][0] -x_im[0][0])*(y41_im[0][0] -x_im[0][0]) ) >0.5 ){
                 cnt_err_g1++;
            }

            for( int i = 2 ; i < 4 ; i ++ ){
                if( ( (y41_re[i][0] -x_re[i][0] )*(y41_re[i][0] -x_re[i][0]) + (y41_im[i][0] -x_im[i][0])*(y41_im[i][0] -x_im[i][0]) ) >0.5 ){
                     cnt_err_g[i+4]++;
                }
            }

            if( cnt_allstar2 == 500){
                for( int i = 7 ; i < 9 ; i++ ){
                    //comp_err_1 = cnt_err_g[i] / cnt_allstar2;
                    comp_err[i] = cnt_err_g[i-1] / cnt_allstar2;

                    height2[i] = -1+ (1-comp_err[i]) ;


                   // height2[i] = -1+ i*0.3 ;

                }



                  qDebug() << " comp err : " << cnt_err_g1;
                    qDebug() << " yre : " << y41_re[0][0]   << " yim : " << y41_im[0][0]  ;
                    qDebug() << " xre : " << x_re[0][0]   << " xim : " << x_im[0][0]  ;
                  cnt_allstar1 = 0;
                  cnt_err_g1 = 0;
                  cnt_err_g[0] = 0;cnt_err_g[1] = 1;

            }


        }
    }


   #ifdef onetime
    for (int i = 0;i<4;i++){
        for(int j=0;j<8;j++){
            mat48_1_re[i][j]=data1[i*256+j][0];
            mat48_1_im[i][j]=data1[i*256+j][1];
        }
    }

    for (int i = 0;i<4;i++){
        for(int j=0;j<8;j++){
            mat48_2_re[i][j]=data1[i*256+j+64][0];
            mat48_2_im[i][j]=data1[i*256+j+64][1];
        }
    }
    hermitian( 4,8,mat48_1_re,mat48_1_im,
                    mat84_tmp_re,mat84_tmp_im );

    Matrix_mult484(mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im, mat44_tmp_re,mat44_tmp_im);
    chol_inv(mat44_tmp_re,mat44_tmp_im,mat44_inv_re,mat44_inv_im);
    Matrix_mult844(mat84_tmp_re,mat84_tmp_im, mat44_inv_re,mat44_inv_im, w84_re,w84_im);
    Matrix_mult484(mat48_2_re,mat48_2_im,w84_re,w84_im,hw44_re,hw44_im);
    double x_re[4][1];
    double x_im[4][1];
    double y41_re[4][1];
    double y41_im[4][1];

    for( int i = 0 ; i < 4 ; i++ ){
        x_re[i][0] = pilot2[i][0];
        x_im[i][0] = pilot2[i][1];
    }



    double alpha=0;

    for (int i=0;i<4;i++){
        alpha += 0.25 * atan(hw44_im[i][i]/hw44_re[i][i]);

    }
    double hw2_44_re[4][4];
    double hw2_44_im[4][4];
    for(int i = 0 ; i < 4 ; i++ ){
        for( int j = 0 ; j < 4 ; j++ ){
            mult(cos(alpha),-sin(alpha),hw44_re[i][j],hw44_im[i][j],&hw2_44_re[i][j],&hw2_44_im[i][j]);
        }
    }
    Matrix_mult441(hw2_44_re,hw2_44_im,x_re,x_im,y41_re,y41_im);

/*
    for( int i = 0 ; i < 4; i++){
        for( int j = 0 ; j < 4 ; j++ ){
            fprintf(fp1,"%lf\t",hw44_re[i][j]);
        }fprintf(fp1,"\n");
    }
    for( int i = 0 ; i < 4; i++){
        for( int j = 0 ; j < 4 ; j++ ){
            fprintf(fp1,"%lf\t",hw44_im[i][j]);
        }fprintf(fp1,"\n");
    }
    */
    for( int i = 0 ; i < 4; i++){
        qDebug() << hw44_re[i][0] << hw44_re[i][1] << hw44_re[i][2] << hw44_re[i][3];// << mat48_1_re[i][4] << mat48_1_re[i][5] << mat48_1_re[i][6] << mat48_1_re[i][7];
    }qDebug()<<"\n";
    for( int i = 0 ; i < 4; i++){
        qDebug() << hw44_im[i][0] << hw44_im[i][1] << hw44_im[i][2] << hw44_im[i][3] ;//<< hw44_im[i][4] << mat48_2_re[i][5] << mat48_2_re[i][6] << mat48_2_re[i][7];
        }qDebug()<<"\n";

    qDebug() << "y  re is "<< y41_re[0][0] <<  y41_re[1][0] <<  y41_re[2][0] <<  y41_re[3][0] ;
    qDebug() << "y im is "<< y41_im[0][0] <<  y41_im[1][0] <<  y41_im[2][0] <<  y41_im[3][0] ;
    qDebug() << "x  re is "<< pilot2[0][0] <<  pilot2[1][0] <<  pilot2[2][0] <<  pilot2[3][0] ;
    qDebug() << "x im is "<< pilot2[0][1] <<  pilot2[1][1] <<  pilot2[2][1] <<  pilot2[3][1] ;

    new_star[cnt_newstar][0] = y41_re[1][0];
    new_star[cnt_newstar++][1] = y41_im[1][0];

    new_star[cnt_newstar][0] = y41_re[0][0];
    new_star[cnt_newstar++][1] = y41_im[0][0];
    new_star[cnt_newstar][0] = y41_re[2][0];
    new_star[cnt_newstar++][1] = y41_im[2][0];
    new_star[cnt_newstar][0] = y41_re[3][0];
    new_star[cnt_newstar++][1] = y41_im[3][0];
    if(cnt_newstar == 60){
        cnt_newstar = 0;
    }
#endif
/*
    for( int i = 0 ; i < 4; i++){
        qDebug() << mat48_1_re[i][0] << mat48_1_re[i][1] << mat48_1_re[i][2] << mat48_1_re[i][3] << mat48_1_re[i][4] << mat48_1_re[i][5] << mat48_1_re[i][6] << mat48_1_re[i][7];
    }qDebug()<<"\n";
    for( int i = 0 ; i < 4; i++){
        qDebug() << mat48_2_re[i][0] << mat48_2_re[i][1] << mat48_2_re[i][2] << mat48_2_re[i][3] << mat48_2_re[i][4] << mat48_2_re[i][5] << mat48_2_re[i][6] << mat48_2_re[i][7];
        }qDebug()<<"\n";

*/

   //Matrix_mult484(mat48_1_re,mat48_1_im, mat84_tmp_re,mat84_tmp_im, mat44_tmp_re,mat44_tmp_im);
/*
    for( int i = 0 ; i < 4 ; i++){
         for( int j = 0 ; j < 4 ; j++){
             mat44_tmp_re[i][1] = 1;
         }
    }
    */







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
    //QFile file("datain.txt");
    QFile file0("./Pilot_send_data/pilot_1200_im.txt");
    if (!file0.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file0.errorString();
        qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];
    char str6[100];
    for( int i = 0 ; i < 1200 ; i++ ){
        file0.readLine(str6,100);
        double crr = 0;
        crr = char2int(str6);
        //qDebug() << "pilot2[ "<< i <<"][0] is :"<<crr;
        pilot2[i][1] = crr;
    }
  file0.close();

  QFile file5("./Pilot_send_data/pilot_1200_re.txt");
  if (!file5.open(QIODevice::ReadOnly | QIODevice::Text)){
      qDebug() << file5.errorString();
      qDebug() << "Last sentence is in QPSK Inputmanagement." << endl;

  }
  //qDebug() << QObject::tr("file text:") << endl << file.readAll();
  //char str[100];

  for( int i = 0 ; i < 1200 ; i++ ){
      file5.readLine(str6,100);
      double crr = 0;
      crr = char2int(str6);
      //qDebug() << "pilot2[ "<< i <<"][0] is :"<<crr;
      pilot2[i][0] = crr;
  }
file5.close();

    //QFile file("./pilot4_send_data/pilot4_1200_re.txt");
    QFile file("./16qam_data_new/data_im.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
        qDebug() << "Last sentence is in ThroughPut Inputmanagement." << endl;

    }
    //qDebug() << QObject::tr("file text:") << endl << file.readAll();
    //char str[100];
    char str[100];
    for( int i = 0 ; i < 1200 ; i++ ){
        file.readLine(str,100);
        double crr = 0;
        crr = char2int(str);
        //qDebug() << "pilot4[ "<< i <<"][0] is :"<<crr;
        pilot4[i][0] = crr;
    }

    //qDebug() << "last one is :"<< ThroughPut[9][0] ;
    file.close();

    //QFile file2("./pilot4_send_data/pilot4_1200_im.txt");
    QFile file2("./16qam_data_new/data_re.txt");
    if (!file2.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file2.errorString();
        qDebug() << "Last sentence is in ThroughPut Inputmanagement." << endl;
    }
    //qDebug() << QObject::tr("file text:") << endl << file2.readAll();
    //char str[100];
    for( int i = 0 ; i < 1200 ; i++ ){
        file2.readLine(str,100);
        double crr = 0;
        crr = char2int(str);
        //qDebug() << "pilot4[ "<< i <<"][1] is :"<<crr;
        pilot4[i][1] = crr;
    }


    //qDebug() <<"apsk[0][0] is :"<< *(pdata) << endl;
    file2.close();


   // QFile file3("./16qam_data/data_re.txt");
    QFile file3("./16qam_data_new/data_re.txt");
    if (!file3.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file3.errorString();
        qDebug() << "Last sentence is in ThroughPut Inputmanagement." << endl;

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
        qam16_3[cnt_2_500][0] = crr;
        cnt_2_500++;
        }
    }

    //qDebug() << "last one is :"<< ThroughPut[9][0] ;
    file3.close();

   // QFile file4("./16qam_data/data_im.txt");
    QFile file4("./16qam_data_new/data_im.txt");
    if (!file4.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file4.errorString();
        qDebug() << "Last sentence is in ThroughPut Inputmanagement." << endl;

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
        qam16_3[cnt_2_500_2][1] = crr;
        cnt_2_500_2++;
        }
    }

    //qDebug() << "last one is :"<< ThroughPut[9][0] ;
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
    for( int i = 0 ; i < 500 ; i++ ){
        double y = (*(pdata+i*2+0));
        double z = *(pdata+i*2+1);
        double diff = sqrt( (z-qam16_3[i][0])*(z-qam16_3[i][0])+(y - qam16_3[i][1])*(y - qam16_3[i][1])  );

        //qDebug() << z << qam16_3[i][0] << "  |  " <<y<<qam16_3[i][1]<<"|"<<(diff)<<"|"<<1/sqrt(10);
        if(diff> 1/sqrt(10)){
              err_3++;
        }
        bit_cnt_2_3++;

        if(bit_cnt_2_3 == 20000){
            qDebug() << "BER :" <<err_3/20000.0 ;
            for( int i = num_p-1 ; i >= 1 ; i-- ){
                *(pdata2_3+i) = *( pdata2_3+i-1);
            }
            if(err_3 -0 <1e-7){
                 *(pdata2_3) = 0 ;
            }else{
                 *(pdata2_3) =   err_3/20000.0  ;
            }
             err_3 = 0;
             bit_cnt_2_3 = 0;
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
