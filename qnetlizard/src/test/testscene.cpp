#include "testscene.h"

#include <QDebug>
#include <QByteArray>

#include "qdef.h"
#include "simplecameraactor.h"
#include "engine/nlmodelrenderer.h"
#include "engine/nlscript.h"
#include "engine/nlcomponent.h"
#include "settings.h"

TestScene::TestScene(QWidget *parent)
    : NLScene(parent)
{
    setObjectName("TestScene");
    Settings *settings = SINGLE_INSTANCE_OBJ(Settings);
    SetFPS(settings->GetSetting<int>("ENGINE/fps", 0));
    SetUpdateInterval(settings->GetSetting<int>("ENGINE/update_interval", 10));

    SimpleCameraActor *camera = new SimpleCameraActor;
    camera->setObjectName("Test_SimpleCameraActor");
    AddActor(camera);
    NLActor *actor = new NLActor;
    actor->setObjectName("Test_CoordinateRenderActor");
    AddActor(actor);
    actor->SetRenderable(new NLModelRenderer_coordinate(NLProperties("length", 9999)));
    SetCurrentCamera(camera->Camera());
    actor = new NLActor;
    actor->setObjectName("Test_ObjectRenderActor");
    AddActor(actor);

    connect(settings, SIGNAL(settingChanged(const QString &, const QVariant &, const QVariant &)), this, SLOT(OnSettingChanged(const QString &, const QVariant &, const QVariant &)));
}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
    NLActor *objectActor = GetActor(2);

    NLActor *plane = new NLActor;
    plane->setObjectName("Test_PlaneRender");
    NLModelRenderer *r = new NLModelRenderer_plane(NLProperties("length", 5000));
    plane->SetRenderable(r);
    r->Model()->Primitive(0).SetColor(QColor::fromRgbF(0.5, 0.5, 0.5));
    objectActor->AddChild(plane);

    NLActor *cube = new NLActor;
    cube->setObjectName("Test_CubeRender");
    r = new NLModelRenderer_cube(NLProperties("length", 200));
    cube->SetRenderable(r);
    objectActor->AddChild(cube);
    NLVector3 v = VECTOR3(-800, 600, -200);
    cube->SetPosition(v);
    NLVector3 vr = VECTOR3(45,0,0);
    cube->SetRotation(vr);

    NLActor *line = new NLActor;
    line->setObjectName("Test_LineRender");
    r = new NLModelRenderer_line(NLProperties("length", 3000));
    line->SetRenderable(r);
    r->Model()->Primitive(0).SetColor(QColor::fromRgbF(0.1, 0.5, 0.9));
    objectActor->AddChild(line);
    NLVector3 v2 = VECTOR3(600, 800, -200);
    line->SetPosition(v2);
    NLVector3 vr2 = VECTOR3(45,45,45);
    line->SetRotation(vr2);

    const char source[] =
            "globalvar_a = 123;\n"
            "global_source = \"globalvar_b = globalvar_b + nl_Delta; return globalvar_b\";\n"
            "\n"
            "function Init()\n"
            "globalvar_b = 0.456;\n"
            "return true;\n"
            "end;\n"
            "\n"
            "function Update(delta)\n"
            "if(nl_Scene:KeyPressed(\"V\")) then;\n"
            "ccc = 'abc';\n"
            "end;\n"
            "\n"
            "if(nl_Scene:KeyPressed(\"B\")) then;\n"
            "ccc = nil;\n"
            "end;\n"
            "\n"
            "if(nl_Scene:KeyPressed(\"F\")) then;\n"
            "local f = _G.load(global_source);\n"
            "print(\"_G.load(\" .. global_source .. \")\", f);\n"
            "if type(f) == \"function\" then;\n"
            "print(\"exec\", f());\n"
            "else\n"
            "print(\"not function\");\n"
            "end;\n"
            "end;\n"
            "\n"
            "if(nl_Scene:KeyPressed(\"P\")) then;\n"
            "print_G();\n"
            "end;\n"
            "\n"
            "end;\n"
            "\n"
            "function print_G()\n"
                "for i, v in pairs(_G) do;\n"
                    "local t = type(v);\n"
                    "if t ~= \"table\" and t ~= \"function\" and t ~= \"thread\" and i ~= \"_VERSION\" then;\n"
                    "print(i, v);\n"
                    "end\n"
                "end\n"
            "end\n"
            ;
    NLScript *script = new NLScript(NLProperties("objectName", "scrrr")("scriptSource", source));
    GetActor(0)->AddScript(script);

    NLComponent *comp = new NLComponent(NLProperties("name", "rrrrrr"));
    objectActor->AddComponent(comp);

    SetFPS(SINGLE_INSTANCE_OBJ(Settings)->GetSetting<int>("ENGINE/fps", 0));

    NLScene::Init();
}

void TestScene::Update(float delta)
{
    NLScene::Update(delta);
}

void TestScene::paintGL()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLineWidth(10);
    NLScene::paintGL();
    glLineWidth(1);

    glFlush();
}

void TestScene::Deinit()
{
    NLScene::Deinit();
    Reset();
}

void TestScene::Reset()
{
}

void TestScene::OnSettingChanged(const QString &name, const QVariant &value, const QVariant &oldValue)
{
    if(name == "ENGINE/fps")
        SetFPS(value.toInt());
    else if(name == "ENGINE/update_interval")
        SetUpdateInterval(value.toInt());
}
