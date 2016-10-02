#ifndef RENDEROPENGLWIDGET_H
#define RENDEROPENGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>

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

private slots:
    void onMessageLogged(QOpenGLDebugMessage message);

private:
    QOpenGLDebugLogger* m_logger;
    GLuint g_vertex_buffer;
    GLuint g_index_buffer;
    GLuint g_texture;

};

#endif // RENDEROPENGLWIDGET_H
