#include "openglwidget.h"
#include <QDebug>
#include <QImage>
openGLWidget::openGLWidget()
    :m_vbo(QOpenGLBuffer::VertexBuffer),
     m_texture(QOpenGLTexture::Target2D)
{
    resize(800,480);

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

   GLfloat vertex[]={
       //vertex         //uv
    -0.5f,-0.5f,0.0f,   0.0f,0.0f,
    0.5f,-0.5f,0.0f,    1.0f,0.0f,
    0.5f,0.5f,0.0f,     1.0f,1.0f,
    -0.5f,0.5f,0.0f,    0.0f,1.0f
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

    m_texture.bind(GL_TEXTURE0);
    m_program.setAttributeValue("qt_Coord",GL_TEXTURE0);

    m_program.bind();
    m_vao.bind();
    glDrawArrays(GL_QUADS,0,4);
    m_program.release();
    m_vao.release();

}
