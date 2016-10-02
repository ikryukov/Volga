#ifndef RENDEROPENGLWIDGET_H
#define RENDEROPENGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class RenderOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    RenderOpenGLWidget(QWidget *parent = 0);
    ~RenderOpenGLWidget();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

 private:
    GLuint g_vertex_buffer;
    GLuint g_index_buffer;
    GLuint g_texture;
};

#endif // RENDEROPENGLWIDGET_H
