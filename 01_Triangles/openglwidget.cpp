#include "openglwidget.h"
#include <QDebug>
openGLWidget::openGLWidget()
    :m_vbo(QOpenGLBuffer::VertexBuffer)
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
   m_vao.create();
   m_vao.bind();

   GLfloat vertex[]={
       //vertex             //color
       -0.5f,-0.5f,0.0f,   1.0f,0.0f,0.0f,
       0.0f,0.5f,0.0f,      0.0f,1.0f,0.0f,
       0.5f,-0.5f,0.0f,     0.0f,0.0f,1.0f
   };


   m_vbo.create();
   m_vbo.bind();
   m_vbo.allocate(vertex,sizeof(vertex));

   GLuint vertexLocation = m_program.attributeLocation("qt_Vertex");
   m_program.setAttributeBuffer(vertexLocation,GL_FLOAT,0,3,6*sizeof(GLfloat));
   m_program.enableAttributeArray(vertexLocation);

   GLuint colorLocation = m_program.attributeLocation("qt_Color");
   m_program.setAttributeBuffer(colorLocation,GL_FLOAT,3*sizeof(GLfloat),3,6*sizeof(GLfloat));
   m_program.enableAttributeArray(colorLocation);


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
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES,0,3);
    m_program.release();
    m_vao.release();

}
