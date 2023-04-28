#include "openglwidget.h"
#include <QDebug>
#include <QImage>
#include <qmath.h>
#include <QTime>
#define M_PI 3.1415926
openGLWidget::openGLWidget()
    :m_vbo(QOpenGLBuffer::VertexBuffer),
      m_texture(QOpenGLTexture::Target2D),
      m_timer(new QTimer(this)),
      m_cameraPos(0.0, 0.0, 3.0),
      m_cameraUp(0.0, 1.0, 0.0),
      m_timerId(0),
      m_moveSpeed(1.0),
      m_yaw(-90.0),
      m_pitch(0.0),
      m_sensitivity(0.01),
      m_fov(45.0)
{
    resize(800,480);
    m_timer->start(100);
    connect(m_timer,&QTimer::timeout,this,[=](){
        angle+=5;
        if(angle >= 360){
            angle = 0;
        }
        scale-=0.1;
        if(scale <= 0.1){
            scale = 1.0f;
        }
        update();
    });
    //计算谁相机的方向，指向的是坐标系的-z轴 终点减去起点

    m_cameraDirection = {cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch)};
    m_cameraDirection.normalize();
    setMouseTracking(true);

}

void openGLWidget::resizeGL(int width, int height)
{
    glViewport(0,0,width,height);
}
/**
 * @brief openGLWidget::initializeGL 初始化openGL函数指针，类似于glad库
 */
void openGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //在绘制3d图形时一定要开启深度检测
    glEnable(GL_DEPTH_TEST);
    bool result = true;
    result =  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,"./vertex.vert");
    if(!result){
        qDebug()<<m_program.log()<<endl;
    }
    result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,"./fragment.frag");
    if(!result){
        qDebug()<<m_program.log()<<endl;
    }
    result = m_program.link();
    if(!result){
        qDebug()<<m_program.log()<<endl;
    }
    //create vao buffer
    m_vao.create();
    m_vao.bind();
    //一个立方体，有六个面，需要定义出立方体的每一个面
    GLfloat vertex[]={
        //vertex         //uv
        //里边的面
        -0.5f,-0.5f,-0.5f,  0.0f,0.0f,
        0.5f,-0.5f,-0.5f,   1.0f,0.0f,
        0.5f,0.5f,-0.5f,     1.0f,1.0f,
        -0.5f,0.5f,-0.5f,    0.0f,1.0f,
        //外边的面
        -0.5f,-0.5f,0.5f,  0.0f,0.0f,
        0.5f,-0.5f,0.5f,   1.0f,0.0f,
        0.5f,0.5f,0.5f,     1.0f,1.0f,
        -0.5f,0.5f,0.5f,    0.0f,1.0f,
        //底面
        -0.5f,-0.5f,-0.5f,    0.0f,0.0f,
        0.5f,-0.5f,-0.5f,     1.0f,0.0f,
        0.5f,-0.5f,0.5f,       1.0f,1.0f,
        -0.5f,-0.5f,0.5f,      0.0f,1.0f,
        //顶面
        -0.5f,0.5f,-0.5f,      0.0f,0.0f,
        0.5f,0.5f,-0.5f,       1.0f,0.0f,
        0.5f,0.5f,0.5f,         1.0f,1.0f,
        -0.5f,0.5f,0.5f,        0.0f,1.0f,

        //左面
        -0.5f,-0.5f,-0.5f,     0.0f,0.0f,
        -0.5f,-0.5f,0.5f,      1.0f,0.0f,
        -0.5f,0.5f,0.5f,        1.0f,1.0f,
        -0.5f,0.5f,-0.5f,       0.0f,1.0f,
        //右面
        0.5f,-0.5f,-0.5f,    0.0f,0.0f,
        0.5f,-0.5f,0.5f,     1.0f,0.0f,
        0.5f,0.5f,0.5f,       1.0f,1.0f,
        0.5f,0.5f,-0.5f,      0.0f,1.0f,



    };
    //create vbo buffer and bind data
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertex,sizeof(vertex));

    //create texture buffer and bind data
    m_texture.create();
    m_texture.bind();
    m_texture.setData(QImage("./container").mirrored());
    m_texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    m_texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);
    m_texture.setMagnificationFilter(QOpenGLTexture::Nearest);
    m_texture.setMinificationFilter(QOpenGLTexture::Linear);

    //operate shader

    m_program.setAttributeBuffer("qt_Vertex",GL_FLOAT,0,3,5*sizeof(GLfloat));
    m_program.enableAttributeArray("qt_Vertex");

    m_program.setAttributeBuffer("qt_Coord",GL_FLOAT,3*sizeof(GLfloat),2,5*sizeof(GLfloat));
    m_program.enableAttributeArray("qt_Coord");

    //after operate we should enable attribute
    m_vao.release();

    m_lastMousePos = this->geometry().center();
    QCursor::setPos(mapToGlobal(m_lastMousePos));//鼠标在中间
    this->setCursor(Qt::BlankCursor);//鼠标不可见
    setFocus();

}
/**
 * @brief openGLWidget::paintGL 发生变化或手动调用repaint和update时调用
 */
void openGLWidget::paintGL()
{
    float curTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_deltaTime = curTime - m_lastFrame;
    m_lastFrame = curTime;
    glClearColor(0.5f,0.2f,0.8f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();
    m_model.setToIdentity();
    m_view.setToIdentity();
    m_projection.setToIdentity();

    m_projection.perspective(m_fov, 1.0 * width() / height(), 0.1, 200.0);
    m_view.lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_cameraUp);
    m_program.setUniformValue("model",m_model);
    m_program.setUniformValue("view",m_view);
    m_program.setUniformValue("projection",m_projection);

    m_texture.bind(GL_TEXTURE0);
    m_program.setAttributeValue("qt_Coord",GL_TEXTURE0);

    m_vao.bind();
    glDrawArrays(GL_QUADS,0,24);
    m_program.release();
    m_vao.release();
    m_texture.release();

}

void openGLWidget::keyPressEvent(QKeyEvent *event)
{
    m_pressedKeys.insert(event->key());
    //启动定时器
    if(!event->isAutoRepeat() && m_timerId == 0){
        m_timerId = startTimer(1);
    }
}

void openGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
    //当没有按键按下且定时器正在运行，才关闭定时器
    if(!event->isAutoRepeat() && m_timerId != 0 && m_pressedKeys.empty()){
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void openGLWidget::timerEvent(QTimerEvent *event)
{
    float speed = m_deltaTime * m_moveSpeed;
    if(m_pressedKeys.contains(Qt::Key_W)){
        m_cameraPos += speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(Qt::Key_S)){
        m_cameraPos -= speed * m_cameraDirection;
    }
    if(m_pressedKeys.contains(Qt::Key_A)){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos -= speed * cameraRight;
    }
    if(m_pressedKeys.contains(Qt::Key_D)){
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos += speed * cameraRight;
    }
    qDebug() << m_deltaTime << ", " << m_moveSpeed;
    update();
}

void openGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float offsetX = event->x() - m_lastMousePos.x();
    float offsetY = m_lastMousePos.y() - event->y();//注意坐标方向
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    //更新lastpos
    m_lastMousePos = event->pos();
    //更新欧拉角
    m_yaw += offsetX;
    m_pitch += offsetY;
    if(m_pitch >= M_PI / 2.0){
        m_pitch = M_PI / 2.0 - 0.1;
    }
    else if(m_pitch <= -M_PI / 2.0){
        m_pitch = -M_PI / 2.0 + 0.1;
    }

    m_cameraDirection = {cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch)};
    m_cameraDirection.normalize();

    update();
}

void openGLWidget::wheelEvent(QWheelEvent *event)
{
    auto degree = event->angleDelta();
    double factor = degree.y() / 360.0;

    m_fov += factor;
    if(m_fov >= 45.0){
        m_fov = 45.0;
    }
    else if(m_fov <= 1.0){
        m_fov = 1.0;
    }

    update();
}

