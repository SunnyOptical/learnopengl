#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
/**
 * @brief The openGLWidget class:创建openGL窗体
 */
class openGLWidget:public QOpenGLWidget,protected QOpenGLFunctions_3_3_Core
{
public:
    openGLWidget();
protected:
    void resizeGL(int width,int height);
    void initializeGL();
    void paintGL();
private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram m_program;
};

#endif // OPENGLWIDGET_H
