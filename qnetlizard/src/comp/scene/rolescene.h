#ifndef _KARIN_ROLESCENE_H
#define _KARIN_ROLESCENE_H

#include "nlscene.h"
#include "gl/nl_gl.h"

class NETLizardAnimationModelRenderer;

NLSCENE(RoleScene)
class RoleScene : public NLScene
{
    Q_OBJECT
public:
    RoleScene(QWidget *parent = 0);
    virtual ~RoleScene();
    bool IsValid() const;
    const GL_NETLizard_3D_Model * Model() const;
    const NETLizard_3D_Frame_Animation * Config() const;
    const NETLizard_3D_Frame_Animation * CurrentAnimation() const;
    int CurrentAnimationFrames() const;
    int Anim() const;
    int Frame() const;
    int AnimFPS() const;
    bool IsPlaying() const;

public Q_SLOTS:
    bool LoadFile(const QString &file, const QString &resourcePath, int game, int index);
    void Reset();
    void NextFrame();
    void PrevFrame();
    void Play();
    void Stop();
    void Toggle();
    void SetAnim(int anim);
    void SetFrame(int frame);
    void SetAnimFPS(int fps);

Q_SIGNALS:
    void frameChanged(int frame);
    void animChanged(int frame);
    void playing();
    void stopped();

protected:
    virtual void Init();
    virtual void paintGL();
    virtual void Update(float f);
    virtual void Deinit();

private:
    void LoadAnimFrame(int index);

private:
    GL_NETLizard_3D_Model *m_model;
    NETLizardAnimationModelRenderer *m_renderer;
    const NETLizard_3D_Frame_Animation *m_frameAnim;
    int m_anim;
    int m_frame;
    bool m_playing;
    int m_animFPS;
    int m_frameInterval;
    qint64 m_lastFrameTime;

    Q_DISABLE_COPY(RoleScene)
};

#endif // _KARIN_ROLESCENE_H
