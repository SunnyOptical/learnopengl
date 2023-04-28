#include <QApplication>
#include "openglwidget.h"
int main(int argc,char** argv){
    QApplication a(argc,argv);
    openGLWidget w;
    w.show();
    return a.exec();
}
