

#include <QGLWidget>
#include <QKeyEvent>
#include <GL/glut.h>


namespace Ui {
class ThroughPut;
}

class ThroughPut : public QGLWidget
{

public:
    explicit ThroughPut(QWidget *parent = 0);
        ~ThroughPut();
    double *pdata;
        double *pdata2_3;
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
    void sys_function();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent *);
private:
    GLfloat translate, xRot, yRot, zRot,rMove;
private:
    Ui::ThroughPut *ui;
signals:

public slots:

};



