#ifndef RENDEROPENGLWIDGET_H
#define RENDEROPENGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <thread>

#include "OpenImageIO/imageio.h"

#include "CLW.h"

#include "math/mathutils.h"

#include "tiny_obj_loader.h"
#include "perspective_camera.h"
#include "shader_manager.h"
#include "Scene/scene.h"
#include "PT/ptrenderer.h"
#include "AO/aorenderer.h"
#include "CLW/clwoutput.h"
#include "config_manager.h"

class RenderOpenGLWidget : public QGLWidget, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

    void checkGLError(const char *file, int32_t line);
public:
    struct OutputData
    {
        Baikal::ClwOutput* output;
        std::vector<RadeonRays::float3> fdata;
        std::vector<unsigned char> udata;
        CLWBuffer<RadeonRays::float3> copybuffer;
    };

    struct ControlData
    {
        std::atomic<int> clear;
        std::atomic<int> stop;
        std::atomic<int> newdata;
        std::mutex datamutex;
        int idx;
    };
public:
    RenderOpenGLWidget(QWidget *parent = 0);
    virtual ~RenderOpenGLWidget();
    void RenderThread(ControlData &cd);


protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void initShaders();

    QOpenGLShaderProgram* mFullscreenProgram;
    GLint mFullscreenTextureUniform;
    GLuint VertexArrayID;
private slots:
    void onMessageLogged(QOpenGLDebugMessage message);

public slots:
    void initCL();
    void InitData(QString fileName);

public:
    void StartRenderThreads();

    QTimer m_renderTimer;

    QOpenGLDebugLogger* m_logger;
    GLuint g_vertex_buffer;
    GLuint g_index_buffer;
    GLuint g_texture;

    std::unique_ptr<Baikal::Scene> g_scene;
    ConfigManager::Mode g_mode = ConfigManager::Mode::kUseSingleGpu;
    bool g_interop = true;
    std::vector<ConfigManager::Config> g_cfgs;
    int g_num_bounces = 5;
    std::vector<OutputData> g_outputs;
    std::unique_ptr<ControlData[]> g_ctrl;
    int g_primary = -1;
    CLWImage2D g_cl_interop_image;
    std::vector<std::thread> g_renderthreads;
    std::unique_ptr<ShaderManager>    g_shader_manager;

    RadeonRays::float3 g_camera_pos = RadeonRays::float3(0.f, 1.f, 4.f);
    RadeonRays::float3 g_camera_at = RadeonRays::float3(0.f, 1.f, 0.f);
    RadeonRays::float3 g_camera_up = RadeonRays::float3(0.f, 1.f, 0.f);

    RadeonRays::float2 g_camera_sensor_size = RadeonRays::float2(0.036f, 0.024f);  // default full frame sensor 36x24 mm
    RadeonRays::float2 g_camera_zcap = RadeonRays::float2(0.0f, 100000.f);
    float g_camera_focal_length = 0.035f; // 35mm lens
    float g_camera_focus_distance = 0.f;
    float g_camera_aperture = 0.f;

    int g_window_width = 800;
    int g_window_height = 600;

    bool     g_is_left_pressed = false;
    bool     g_is_right_pressed = false;
    bool     g_is_fwd_pressed = false;
    bool     g_is_back_pressed = false;
    bool     g_is_home_pressed = false;
    bool     g_is_end_pressed = false;
    bool     g_is_mouse_tracking = false;
    RadeonRays::float2   g_mouse_pos = RadeonRays::float2(0, 0);
    RadeonRays::float2   g_mouse_delta = RadeonRays::float2(0, 0);
    float g_cspeed = 100.25f;
    float g_envmapmul = 1.f;
    int g_num_samples = -1;
    int g_samplecount = 0;
    bool g_benchmark = false;
    void Update();
    bool isLoaded = false;
    void Render();
};

#endif // RENDEROPENGLWIDGET_H
