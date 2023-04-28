#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QTimer>
#include <QEvent>
#include <QWheelEvent>
#include <qevent.h>
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
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLTexture m_texture;
    QOpenGLShaderProgram m_program;
    quint32 angle=10;
    qreal scale = 1.0f;
    QTimer *m_timer;
    //定义模型视图投影矩阵
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;
    //openGL中本身没有摄像机，是根据视图坐标与世界坐标转换虚拟出来的摄像机
    //摄像机位置
    QVector3D m_cameraPos;
    //摄像机方向
    QVector3D m_cameraDirection;
    //上轴
    QVector3D m_cameraUp;

    //interact
    //key
    QSet<int> m_pressedKeys;
    int m_timerId;
    float m_deltaTime;//当前帧与上一帧的时间差
    float m_lastFrame;//上一帧的时间
    float m_moveSpeed;

    float m_yaw;//偏航角
    float m_pitch;//俯视角
    float m_sensitivity;//鼠标灵敏度

    float m_fov;

    QPoint m_lastMousePos;
};

#endif // OPENGLWIDGET_H
