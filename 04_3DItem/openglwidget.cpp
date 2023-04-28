#include "openglwidget.h"
#include <QDebug>
#include <QImage>
openGLWidget::openGLWidget()
    :m_vbo(QOpenGLBuffer::VertexBuffer),
      m_texture(QOpenGLTexture::Target2D),
      m_timer(new QTimer(this))
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

}
/**
 * @brief openGLWidget::paintGL 发生变化或手动调用repaint和update时调用
 */
void openGLWidget::paintGL()
{
    glClearColor(0.5f,0.2f,0.8f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_program.bind();
    QMatrix4x4 model;
    model.setToIdentity();
    //设置沿着z轴旋转,并缩放一定角度
    model.rotate(angle,0,1,0);

    QMatrix4x4 view;
    view.setToIdentity();
    view.translate({0.0, 0.0, -3.0});
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    m_program.setUniformValue("model",model);
    m_program.setUniformValue("view",view);
    m_program.setUniformValue("projection",projection);

    m_texture.bind(GL_TEXTURE0);
    m_program.setAttributeValue("qt_Coord",GL_TEXTURE0);

    m_vao.bind();
    glDrawArrays(GL_QUADS,0,24);
    m_program.release();
    m_vao.release();
    m_texture.release();

}
