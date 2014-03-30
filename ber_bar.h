#ifndef BER_BAR_H
#define BER_BAR_H


#include <QGLWidget>
#include <QKeyEvent>
#include <GL/glut.h>


namespace Ui {
class BER_BAR;
}

class BER_BAR : public QGLWidget
{

public:
    explicit BER_BAR(QWidget *parent = 0);
        ~BER_BAR();
    double *pdata;
        double *pdata2_2;
    void wallplot();
    int num_p;

    void timerEvent(QTimerEvent *event);
    int id1;
    void InputManagement();
    void myDrawCircle(double x, double y, double z, double r);
    void myDrawPoint(double x, double y, double z, double r);

    void myDrawStars();
    int hex2int(char a, char b, char c, char d);
    double char2double(char *str);
    double char2int(char *str);
    int ber();
    void R_Adjust(double *tmp);
    void L_Adjust(double *tmp);
    void D_Adjust(double *tmp);
    void Draw_line();
    void Bar_ber();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent *);
private:
    GLfloat translate, xRot, yRot, zRot,rMove;
private:
    Ui::BER_BAR *ui;
signals:

public slots:

};


#endif // BER_BAR_H
