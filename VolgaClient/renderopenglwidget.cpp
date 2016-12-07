#include "renderopenglwidget.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

using namespace RadeonRays;

void RenderOpenGLWidget::checkGLError(const char* file, int32_t line)
{
    GLint error = glGetError();
    if(error)
    {
        const char* errorString = 0;
        switch(error)
        {
        case GL_INVALID_ENUM:
            errorString = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_INVALID_VALUE:
            errorString = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorString = "GL_INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            errorString = "GL_OUT_OF_MEMORY";
            break;
        default:
            errorString = "unknown error";
            break;
        }
        qCritical("GL error: %s, line %d: %s\n", file, line, errorString);
        error = 0; // nice place to hang a breakpoint in compiler... :)
    }
}

#ifdef _DEBUG
#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)
#else
#define CHECK_GL_ERROR()
#endif

RenderOpenGLWidget::RenderOpenGLWidget(QWidget *parent)
    : QGLWidget(parent)

{
    QGLFormat format;
    format.setVersion(4, 1);
    format.setProfile(QGLFormat::CoreProfile);
//    format.setProfile(QGLFormat::CompatibilityProfile);
//    format.setRenderableType(QSurfaceFormat::OpenGL);
//    format.setProfile(QSurfaceFormat::CoreProfile);
#ifdef GL_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    QGLWidget::setFormat(format);

    connect(&m_renderTimer, SIGNAL(timeout()), this, SLOT(updateGL()));


}

RenderOpenGLWidget::~RenderOpenGLWidget()
{
}

void RenderOpenGLWidget::initializeGL()
{
#ifdef GL_DEBUG
    m_logger = new QOpenGLDebugLogger( this );
    connect(m_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);
    if (m_logger->initialize())
    {
        m_logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
        m_logger->enableMessages();
    }
    else
    {
      qDebug() << "GL_DEBUG Debug Logger (NONE)\n";
    }
#endif

    bool res = initializeOpenGLFunctions();
    if (res)
    {
        qDebug("OpenGL initialized: version: %s GLSL: %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
    else
    {
        qCritical("Unable to initialize OpenGL");
        Q_ASSERT(false);
    }
//    g_shader_manager.reset(new ShaderManager());
    initShaders();
    CHECK_GL_ERROR();

//    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//    CHECK_GL_ERROR();
    glCullFace(GL_FRONT);
//    glCullFace(GL_FRONT_AND_BACK);
    CHECK_GL_ERROR();
//    glDisable(GL_DEPTH_TEST);
//    glEnable(GL_DEPTH_TEST);
//    CHECK_GL_ERROR();
//    glEnable(GL_TEXTURE_2D);
//    CHECK_GL_ERROR();

    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    CHECK_GL_ERROR();
    resizeGL(width(), height());
}

void RenderOpenGLWidget::Render()
{
    try
    {
        {
//            glDisable(GL_DEPTH_TEST);
//            CHECK_GL_ERROR();

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            CHECK_GL_ERROR();

            mFullscreenProgram->bind();
            CHECK_GL_ERROR();

            mFullscreenProgram->setUniformValue(mFullscreenTextureUniform, 0);
            CHECK_GL_ERROR();

            glActiveTexture(GL_TEXTURE0);
            CHECK_GL_ERROR();
            glBindTexture(GL_TEXTURE_2D, g_texture);
            CHECK_GL_ERROR();

//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//            CHECK_GL_ERROR();

            glDrawArrays(GL_TRIANGLES, 0, 3);
            CHECK_GL_ERROR();

            glBindTexture(GL_TEXTURE_2D, 0);
            CHECK_GL_ERROR();
            mFullscreenProgram->release();
            CHECK_GL_ERROR();
//            glFinish();
            CHECK_GL_ERROR();
        }
        CHECK_GL_ERROR();
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what();
        exit(-1);
    }
}

void RenderOpenGLWidget::paintGL()
{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (isLoaded)
        Update();
    Render();

}

void RenderOpenGLWidget::resizeGL(int width, int height)
{
    qreal retinaScale = devicePixelRatioF() / 8;
    g_window_width = width * retinaScale;
    g_window_height = height * retinaScale;
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, g_window_width, g_window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();

    glViewport(0, 0, g_window_width, g_window_height);
    CHECK_GL_ERROR();
}

void RenderOpenGLWidget::mousePressEvent(QMouseEvent *event)
{

}

void RenderOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void RenderOpenGLWidget::initShaders()
{
    bool status = false;
    mFullscreenProgram = new QOpenGLShaderProgram(this);
    status = mFullscreenProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/fullscreentriangle.vert");
    Q_ASSERT(status);
    status = mFullscreenProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fullscreentriangle.frag");
    Q_ASSERT(status);
    status = mFullscreenProgram->link();
    Q_ASSERT(status);

    mFullscreenTextureUniform = mFullscreenProgram->uniformLocation("gTexture");
    CHECK_GL_ERROR();

}

void RenderOpenGLWidget::onMessageLogged(QOpenGLDebugMessage message)
{
    qDebug() << message;
}

void RenderOpenGLWidget::initCL()
{
    bool force_disable_itnerop = false;

    try
    {
        ConfigManager::CreateConfigs(g_mode, g_interop, g_cfgs, g_num_bounces);
    }
    catch (CLWException &)
    {
        force_disable_itnerop = true;
        ConfigManager::CreateConfigs(g_mode, false, g_cfgs, g_num_bounces);
    }


    std::cout << "Running on devices: \n";

    for (int i = 0; i < g_cfgs.size(); ++i)
    {
        std::cout << i << ": " << g_cfgs[i].context.GetDevice(g_cfgs[i].devidx).GetName() << "\n";
    }

    g_interop = false;

    g_outputs.resize(g_cfgs.size());
    g_ctrl.reset(new ControlData[g_cfgs.size()]);

    for (int i = 0; i < g_cfgs.size(); ++i)
    {
        if (g_cfgs[i].type == ConfigManager::kPrimary)
        {
            g_primary = i;

            if (g_cfgs[i].caninterop)
            {
                g_cl_interop_image = g_cfgs[i].context.CreateImage2DFromGLTexture(g_texture);
                g_interop = true;
            }
        }

        g_ctrl[i].clear.store(1);
        g_ctrl[i].stop.store(0);
        g_ctrl[i].newdata.store(0);
        g_ctrl[i].idx = i;
    }

    if (force_disable_itnerop)
    {
        std::cout << "OpenGL interop is not supported, disabled, -interop flag is ignored\n";
    }
    else
    {
        if (g_interop)
        {
            std::cout << "OpenGL interop mode enabled\n";
        }
        else
        {
            std::cout << "OpenGL interop mode disabled\n";
        }
    }
}

void RenderOpenGLWidget::InitData(QString fileName)
{
    rand_init();

    // Load obj file
//    std::string basepath = g_path;
    std::string basepath = QFileInfo(fileName).dir().absolutePath().toStdString();
    basepath += "/";
//    std::string filename = basepath + g_modelname;
    std::string filename = fileName.toStdString();
    g_scene.reset(Baikal::Scene::LoadFromObj(filename, basepath));

    g_scene->camera_.reset(new PerspectiveCamera(
        g_camera_pos
        , g_camera_at
        , g_camera_up));

    // Adjust sensor size based on current aspect ratio
    float aspect = (float)g_window_width / g_window_height;
    g_camera_sensor_size.y = g_camera_sensor_size.x / aspect;

    g_scene->camera_->SetSensorSize(g_camera_sensor_size);
    g_scene->camera_->SetDepthRange(g_camera_zcap);
    g_scene->camera_->SetFocalLength(g_camera_focal_length);
    g_scene->camera_->SetFocusDistance(g_camera_focus_distance);
    g_scene->camera_->SetAperture(g_camera_aperture);

    std::cout << "Camera type: " << (g_scene->camera_->GetAperture() > 0.f ? "Physical" : "Pinhole") << "\n";
    std::cout << "Lens focal length: " << g_scene->camera_->GetFocalLength() * 1000.f << "mm\n";
    std::cout << "Lens focus distance: " << g_scene->camera_->GetFocusDistance() << "m\n";
    std::cout << "F-Stop: " << 1.f / (g_scene->camera_->GetAperture() * 10.f) << "\n";
    std::cout << "Sensor size: " << g_camera_sensor_size.x * 1000.f << "x" << g_camera_sensor_size.y * 1000.f << "mm\n";

    //g_scene->SetEnvironment("../Resources/Textures/studio015.hdr", "", g_envmapmul);
    g_scene->SetEnvironment(QFileInfo(fileName).dir().absoluteFilePath("i8.hdr").toStdString().c_str(), "", g_envmapmul);
#pragma omp parallel for
    for (int i = 0; i < g_cfgs.size(); ++i)
    {
        //g_cfgs[i].renderer->SetNumBounces(g_num_bounces);
        g_cfgs[i].renderer->Preprocess(*g_scene);

        g_outputs[i].output = (Baikal::ClwOutput*)g_cfgs[i].renderer->CreateOutput(g_window_width, g_window_height);

        g_cfgs[i].renderer->SetOutput(g_outputs[i].output);

        g_outputs[i].fdata.resize(g_window_width * g_window_height);
        g_outputs[i].udata.resize(g_window_width * g_window_height * 4);

        if (g_cfgs[i].type == ConfigManager::kPrimary)
        {
            g_outputs[i].copybuffer = g_cfgs[i].context.CreateBuffer<float3>(g_window_width * g_window_height, CL_MEM_READ_WRITE);
        }
    }

    g_cfgs[g_primary].renderer->Clear(float3(0, 0, 0), *g_outputs[g_primary].output);
    m_renderTimer.start(16);
}

void RenderOpenGLWidget::RenderThread(ControlData& cd)
{
    auto renderer = g_cfgs[cd.idx].renderer;
    auto output = g_outputs[cd.idx].output;

    auto updatetime = std::chrono::high_resolution_clock::now();

    while (!cd.stop.load())
    {
        int result = 1;
        bool update = false;

        if (std::atomic_compare_exchange_strong(&cd.clear, &result, 0))
        {
            renderer->Clear(float3(0, 0, 0), *output);
            update = true;
        }

        renderer->Render(*g_scene.get());

        auto now = std::chrono::high_resolution_clock::now();

        update = update || (std::chrono::duration_cast<std::chrono::seconds>(now - updatetime).count() > 1);

        if (update)
        {
            g_outputs[cd.idx].output->GetData(&g_outputs[cd.idx].fdata[0]);
            updatetime = now;
            cd.newdata.store(1);
        }

        g_cfgs[cd.idx].context.Finish(0);
    }
}


void RenderOpenGLWidget::StartRenderThreads()
{
    for (int i = 0; i < g_cfgs.size(); ++i)
    {
        if (i != g_primary)
        {
            g_renderthreads.push_back(std::thread(&RenderOpenGLWidget::RenderThread, this, std::ref(g_ctrl[i])));
            g_renderthreads.back().detach();
        }
    }

    std::cout << g_cfgs.size() << " OpenCL submission threads started\n";
}

void RenderOpenGLWidget::Update()
{
    static auto prevtime = std::chrono::high_resolution_clock::now();
    static auto updatetime = std::chrono::high_resolution_clock::now();

    static int numbnc = 1;
    static int framesperbnc = 2;
    auto time = std::chrono::high_resolution_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::duration<double>>(time - prevtime);
    prevtime = time;

    bool update = false;
    float camrotx = 0.f;
    float camroty = 0.f;

    const float kMouseSensitivity = 0.001125f;
    float2 delta = g_mouse_delta * float2(kMouseSensitivity, kMouseSensitivity);
    camrotx = -delta.x;
    camroty = -delta.y;

    if (std::abs(camroty) > 0.001f)
    {
        g_scene->camera_->Tilt(camroty);
        //g_scene->camera_->ArcballRotateVertically(float3(0, 0, 0), camroty);
        update = true;
    }

    if (std::abs(camrotx) > 0.001f)
    {

        g_scene->camera_->Rotate(camrotx);
        //g_scene->camera_->ArcballRotateHorizontally(float3(0, 0, 0), camrotx);
        update = true;
    }

    const float kMovementSpeed = g_cspeed;
    if (g_is_fwd_pressed)
    {
        g_scene->camera_->MoveForward((float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (g_is_back_pressed)
    {
        g_scene->camera_->MoveForward(-(float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (g_is_right_pressed)
    {
        g_scene->camera_->MoveRight((float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (g_is_left_pressed)
    {
        g_scene->camera_->MoveRight(-(float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (g_is_home_pressed)
    {
        g_scene->camera_->MoveUp((float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (g_is_end_pressed)
    {
        g_scene->camera_->MoveUp(-(float)dt.count() * kMovementSpeed);
        update = true;
    }

    if (update)
    {
        g_scene->set_dirty(Baikal::Scene::kCamera);

        if (g_num_samples > -1)
        {
            g_samplecount = 0;
        }

        for (int i = 0; i < g_cfgs.size(); ++i)
        {
            if (i == g_primary)
                g_cfgs[i].renderer->Clear(float3(0, 0, 0), *g_outputs[i].output);
            else
                g_ctrl[i].clear.store(true);
        }

        /*        numbnc = 1;
        for (int i = 0; i < g_cfgs.size(); ++i)
        {
        g_cfgs[i].renderer->SetNumBounces(numbnc);
        }*/
    }

    if (g_num_samples == -1 || g_samplecount++ < g_num_samples)
    {
        g_cfgs[g_primary].renderer->Render(*g_scene.get());
    }

    //if (std::chrono::duration_cast<std::chrono::seconds>(time - updatetime).count() > 1)
    //{
    for (int i = 0; i < g_cfgs.size(); ++i)
    {
        if (g_cfgs[i].type == ConfigManager::kPrimary)
            continue;

        int desired = 1;
        if (std::atomic_compare_exchange_strong(&g_ctrl[i].newdata, &desired, 0))
        {
            {
                //std::unique_lock<std::mutex> lock(g_ctrl[i].datamutex);
                //std::cout << "Start updating acc buffer\n"; std::cout.flush();
                g_cfgs[g_primary].context.WriteBuffer(0, g_outputs[g_primary].copybuffer, &g_outputs[i].fdata[0], g_window_width * g_window_height);
                //std::cout << "Finished updating acc buffer\n"; std::cout.flush();
            }

            CLWKernel acckernel = g_cfgs[g_primary].renderer->GetAccumulateKernel();

            int argc = 0;
            acckernel.SetArg(argc++, g_outputs[g_primary].copybuffer);
            acckernel.SetArg(argc++, g_window_width * g_window_width);
            acckernel.SetArg(argc++, g_outputs[g_primary].output->data());

            int globalsize = g_window_width * g_window_height;
            g_cfgs[g_primary].context.Launch1D(0, ((globalsize + 63) / 64) * 64, 64, acckernel);
        }
    }

    //updatetime = time;
    //}

    if (!g_interop)
    {
        g_outputs[g_primary].output->GetData(&g_outputs[g_primary].fdata[0]);

        float gamma = 2.2f;
        int fdataSize = g_outputs[g_primary].fdata.size();
        for (int i = 0; i < (int) fdataSize; ++i)
        {
            g_outputs[g_primary].udata[4 * i] = (unsigned char)clamp((unsigned) (clamp(pow(g_outputs[g_primary].fdata[i].x / g_outputs[g_primary].fdata[i].w, 1.f / gamma), 0.f, 1.f) * 255), (unsigned) 0, (unsigned) 255);
            g_outputs[g_primary].udata[4 * i + 1] = (unsigned char)clamp((unsigned) (clamp(pow(g_outputs[g_primary].fdata[i].y / g_outputs[g_primary].fdata[i].w, 1.f / gamma), 0.f, 1.f) * 255), (unsigned) 0, (unsigned) 255);
            g_outputs[g_primary].udata[4 * i + 2] = (unsigned char)clamp((unsigned) (clamp(pow(g_outputs[g_primary].fdata[i].z / g_outputs[g_primary].fdata[i].w, 1.f / gamma), 0.f, 1.f) * 255), (unsigned) 0, (unsigned) 255);
            g_outputs[g_primary].udata[4 * i + 3] = (unsigned char) 255;
        }


        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, g_texture);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, g_outputs[g_primary].output->width(), g_outputs[g_primary].output->height(), GL_RGBA, GL_UNSIGNED_BYTE, &g_outputs[g_primary].udata[0]);
        CHECK_GL_ERROR();

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::vector<cl_mem> objects;
        objects.push_back(g_cl_interop_image);
        g_cfgs[g_primary].context.AcquireGLObjects(0, objects);

        CLWKernel copykernel = g_cfgs[g_primary].renderer->GetCopyKernel();

        int argc = 0;
        copykernel.SetArg(argc++, g_outputs[g_primary].output->data());
        copykernel.SetArg(argc++, g_outputs[g_primary].output->width());
        copykernel.SetArg(argc++, g_outputs[g_primary].output->height());
        copykernel.SetArg(argc++, 2.2f);
        copykernel.SetArg(argc++, g_cl_interop_image);

        int globalsize = g_outputs[g_primary].output->width() * g_outputs[g_primary].output->height();
        g_cfgs[g_primary].context.Launch1D(0, ((globalsize + 63) / 64) * 64, 64, copykernel);

        g_cfgs[g_primary].context.ReleaseGLObjects(0, objects);
        g_cfgs[g_primary].context.Finish(0);
    }


    if (g_benchmark)
    {
        auto const kNumBenchmarkPasses = 100U;

        Baikal::Renderer::BenchmarkStats stats;
        g_cfgs[g_primary].renderer->RunBenchmark(*g_scene.get(), kNumBenchmarkPasses, stats);

        auto numrays = stats.resolution.x * stats.resolution.y;
        std::cout << "Baikal renderer benchmark\n";
        std::cout << "Number of primary rays: " << numrays << "\n";
        std::cout << "Primary rays: " << (float)(numrays / (stats.primary_rays_time_in_ms * 0.001f) * 0.000001f) << "mrays/s ( " << stats.primary_rays_time_in_ms << "ms )\n";
        std::cout << "Secondary rays: " << (float)(numrays / (stats.secondary_rays_time_in_ms * 0.001f) * 0.000001f) << "mrays/s ( " << stats.secondary_rays_time_in_ms << "ms )\n";
        std::cout << "Shadow rays: " << (float)(numrays / (stats.shadow_rays_time_in_ms * 0.001f) * 0.000001f) << "mrays/s ( " << stats.shadow_rays_time_in_ms << "ms )\n";
        g_benchmark = false;
    }

//    glutPostRedisplay();
}
