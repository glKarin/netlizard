#include "itemscene.h"

#include <QDebug>
#include <QByteArray>
#include <QFileInfo>

#include "qdef.h"
#include "simplecameraactor.h"
#include "netlizardmapmodelrenderer.h"
#include "simplecontrolcomponent.h"
#include "nlsceneorthocamera.h"
#include "nlscenecamera.h"
#include "settings.h"

ItemScene::ItemScene(QWidget *parent)
    : NLScene(parent),
      m_model(0),
      m_renderer(0),
      m_control(0)
{
    setObjectName("ItemScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("RENDER/fps", 0));

    SimpleCameraActor *camera = new SimpleCameraActor(NLProperties("camera_z_is_up", true));
    AddActor(camera);
    m_control = static_cast<SimpleControlComponent *>(camera->Control());
    NLActor *actor = new NLActor;
    AddActor(actor);
    m_renderer = new NETLizardMapModelRenderer;
    actor->SetRenderable(m_renderer);
    SetCurrentCamera(camera->Camera());

    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

ItemScene::~ItemScene()
{
    DEBUG_DESTROY_Q;
}

void ItemScene::Init()
{
    NLScene::Init();
}

void ItemScene::Update(float delta)
{
    //qDebug() << delta;
    NLScene::Update(delta);
}

void ItemScene::paintGL()
{
    if(!m_model)
        return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //glDisable(GL_CULL_FACE);
    NLScene::paintGL();

    glFlush();
}

void ItemScene::Deinit()
{
    Reset();
    NLScene::Deinit();
}

bool ItemScene::LoadFile(const QString &file, const QString &resourcePath, int game, int index)
{
    Reset();
    QByteArray ba1 = file.toLocal8Bit();
    const char *path = ba1.data();
    QByteArray ba2 = resourcePath.toLocal8Bit();
    const char *resc_path = ba2.data();
    m_model = (GL_NETLizard_3D_Model *)malloc(sizeof(GL_NETLizard_3D_Model));
    memset(m_model, 0, sizeof(GL_NETLizard_3D_Model));
    GLboolean b = GL_FALSE;
    qDebug() << "Load game: " << path << resc_path;
    switch(game)
    {
    case NL_CONTR_TERRORISM_3D:
        b = NETLizard_ReadGLCT3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_2:
        b = NETLizard_ReadGLCT3DEp2ItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_ARMY_RANGER_3D:
        b = NETLizard_ReadGLSpecnaz3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_SHADOW_OF_EGYPT_3D:
        b = NETLizard_ReadGLEgypt3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_CLONE_3D:
        b = NETLizard_ReadGLClone3DItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_CONTR_TERRORISM_3D_EPISODE_3:
        b = NETLizard_ReadGLCT3DEp3ItemModelFile(path, index, resc_path, m_model);
        break;
    case NL_RACING_EVOLUTION_3D:
    {
        QFileInfo info(path);
        QRegExp regexp("car(\\d+)");
        int carIndex = index;
        if(carIndex == -1)
        {
            int pos = regexp.indexIn(info.fileName());
            if(pos != -1)
            {
                carIndex = regexp.cap(1).toInt();
            }
        }
        QString texPath = QString("car%1_tex.png").arg(carIndex);
        qDebug() << "Car tex: " << texPath;
        QByteArray ba3 = texPath.toLocal8Bit();
        const char *tex_path = ba3.data();
        b = NETLizard_ReadGLRE3DCarModelFile(path, tex_path, resc_path, m_model);
    }
        break;
    default:
        qDebug() << "Unsupport game";
        break;
    }
    qDebug() << "Load item model result: " << b;
    if(!b)
    {
        free(m_model);
        m_model = 0;
        return false;
    }

    m_renderer->SetModel(m_model);

    if(game == NL_RACING_EVOLUTION_3D)
    {
        CurrentCamera()->SetZIsUp(false);
        SimpleCameraActor *cameraActor = GetActor_T<SimpleCameraActor>(0);
        SimpleControlComponent *control = cameraActor->GetComponent_T<SimpleControlComponent>(1);
        control->SetMoveSens(control->MoveSens() / 50);
        vector3_t startPos = VECTOR3(0, 0.5, 5);
        cameraActor->SetPosition(startPos);
        cameraActor->UpdateCamera();
    }
    else
    {
        CurrentCamera()->SetZIsUp(true);
        vector3_t startPos = VECTOR3(0, 150, 1000);
        SimpleCameraActor *camera = GetActor_T<SimpleCameraActor>(0);
        camera->SetPosition(startPos);
        camera->UpdateCamera();
    }

    //GrabMouseCursor(true);

    return true;
}

void ItemScene::Reset()
{
    m_renderer->SetModel(0);
    if(m_model)
    {
        delete_GL_NETLizard_3D_Model(m_model);
        free(m_model);
        m_model = 0;
    }

    NLScene::Reset();
}

void ItemScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "RENDER/fps")
        SetFPS(value.toInt());
    else if(name == "CONTROL_3D/move_sens")
        m_control->SetMoveSens(value.toInt());
    else if(name == "CONTROL_3D/turn_sens")
        m_control->SetTurnSens(value.toInt());
    else if(name == "CONTROL_3D/freelook_sens")
        m_control->SetFreelookSens(value.toFloat());
    else if(name == "CONTROL_3D/fovy_sens")
        m_control->SetFovySens(value.toFloat());
}
