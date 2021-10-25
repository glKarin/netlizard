######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 17 20:37:54 2016
######################################################################

TEMPLATE = app
TARGET = ../qnetlizard

QT += opengl

DEPENDPATH += . .. ../libnetlizard ./src ./src/utils ./src/widget ./src/misc
INCLUDEPATH += . .. ../libnetlizard ./src ./src/utils ./src/widget ./src/misc

PRE_TARGETDEPS += ../libnetlizard/debug/libnetlizard.a
LIBS += ../libnetlizard/debug/libnetlizard.a

CONFIG          += debug #_and_release

# Input
HEADERS += \
    src/qdef.h \
    src/utils/ioutility.h \
    src/misc/logoutput.h \
    src/misc/settings.h \
    src/indexviewer.h \
    src/spriteviewer.h \
    src/comp/renderer/netlizardspriterenderer.h \
    src/comp/scene/spritescene.h \
    lib/ortho.h \
    lib/frustum.h \
    lib/matrix.h \
    src/gl/nl_util.h \
    src/comp/component/simplelightsourcecomponent.h \
    src/comp/simplelightsourceactor.h \
    src/gl/nl_shadow.h \
    src/gl/shadow.h \
    src/comp/renderer/netlizardshadowmodelrenderer.h

SOURCES += \
    main.cpp \
    src/utils/ioutility.cpp \
    src/misc/logoutput.cpp \
    src/misc/settings.cpp \
    src/indexviewer.cpp \
    src/gl/nl_gl_sprite.c \
    src/spriteviewer.cpp \
    src/comp/renderer/netlizardspriterenderer.cpp \
    src/comp/scene/spritescene.cpp \
    lib/ortho.c \
    lib/frustum.c \
    lib/matrix.c \
    src/gl/nl_util.c \
    src/comp/component/simplelightsourcecomponent.cpp \
    src/comp/simplelightsourceactor.cpp \
    src/gl/nl_shadow.cpp \
    src/gl/shadow.cpp \
    src/comp/renderer/netlizardshadowmodelrenderer.cpp

# Widget
HEADERS += \
    src/widget/baseviewer.h \
    src/widget/settingwidget.h \
    src/mainwindow.h \
    src/imageviewer.h \
    src/textviewer.h \
    src/stringviewer.h \
    src/mapviewer.h \
    src/fontviewer.h \
    src/itemviewer.h \
    src/helpdialog.h \
    src/aboutdialog.h \
    src/logdialog.h \
    src/animationviewer.h \
    src/settingdialog.h

SOURCES += \
    src/widget/baseviewer.cpp \
    src/widget/settingwidget.cpp \
    src/mainwindow.cpp \
    src/imageviewer.cpp \
    src/textviewer.cpp \
    src/stringviewer.cpp \
    src/mapviewer.cpp \
    src/fontviewer.cpp \
    src/itemviewer.cpp \
    src/helpdialog.cpp \
    src/aboutdialog.cpp \
    src/logdialog.cpp \
    src/animationviewer.cpp \
    src/settingdialog.cpp

# Component
DEPENDPATH += ./src/comp ./src/comp/scene ./src/comp/component ./src/comp/renderer
INCLUDEPATH += ./src/comp ./src/comp/scene ./src/comp/component ./src/comp/renderer

HEADERS += \
    src/comp/simplecameraactor.h
SOURCES += \
    src/comp/simplecameraactor.cpp

HEADERS += \
    src/comp/component/simplecontrolcomponent.h \
    src/comp/component/simplecontrol2dcomponent.h \
    src/comp/component/simpleimagecontrolcomponent.h \
    src/comp/component/simplecameracomponent.h
SOURCES += \
    src/comp/component/simplecontrolcomponent.cpp \
    src/comp/component/simplecontrol2dcomponent.cpp \
    src/comp/component/simpleimagecontrolcomponent.cpp \
    src/comp/component/simplecameracomponent.cpp

HEADERS += \
    src/comp/renderer/netlizardmapmodelrenderer.h \
    src/comp/renderer/netlizarditemmodelrenderer.h \
    src/comp/renderer/netlizardtexturerenderer.h \
    src/comp/renderer/netlizardfontrenderer.h \
    src/comp/renderer/netlizardanimationmodelrenderer.h
SOURCES += \
    src/comp/renderer/netlizardmapmodelrenderer.cpp \
    src/comp/renderer/netlizarditemmodelrenderer.cpp \
    src/comp/renderer/netlizardtexturerenderer.cpp \
    src/comp/renderer/netlizardfontrenderer.cpp \
    src/comp/renderer/netlizardanimationmodelrenderer.cpp

HEADERS += \
    src/comp/scene/imagescene.h \
    src/comp/scene/mapscene.h \
    src/comp/scene/fontscene.h \
    src/comp/scene/animationscene.h \
    src/comp/scene/itemscene.h
SOURCES += \
    src/comp/scene/imagescene.cpp \
    src/comp/scene/mapscene.cpp \
    src/comp/scene/fontscene.cpp \
    src/comp/scene/animationscene.cpp \
    src/comp/scene/itemscene.cpp

# Engine
DEPENDPATH += ./src/engine
INCLUDEPATH += ./src/engine

HEADERS += \
    src/engine/nldef.h \
    src/engine/nlscene.h \
    src/engine/nlobject.h \
    src/engine/nlactor.h \
    src/engine/nlrenderable.h \
    src/engine/nlobjectcontainer.h \
    src/engine/nlactorcontainer.h \
    src/engine/nlcomponent.h \
    src/engine/nlcomponentcontainer.h \
    src/engine/nlobjectpool.h \
    src/engine/nlscenecamera.h \
    src/engine/nlsceneorthocamera.h \
    src/engine/nlsceneperspectivecamera.h \
    src/engine/nlfuncs.h \
    src/engine/nlmath.h

SOURCES += \
    src/engine/nlscene.cpp \
    src/engine/nlobject.cpp \
    src/engine/nlactor.cpp \
    src/engine/nlrenderable.cpp \
    src/engine/nlobjectcontainer.cpp \
    src/engine/nlactorcontainer.cpp \
    src/engine/nlcomponent.cpp \
    src/engine/nlcomponentcontainer.cpp \
    src/engine/nlobjectpool.cpp \
    src/engine/nlscenecamera.cpp \
    src/engine/nlsceneorthocamera.cpp \
    src/engine/nlsceneperspectivecamera.cpp \
    src/engine/nlfuncs.cpp \
    src/engine/nlmath.cpp

# Lib
DEPENDPATH += ./lib
INCLUDEPATH += ./lib

HEADERS += \
    lib/mesa_gl_math.h \
#    lib/camera.h \
    lib/vector3.h \
    lib/bound.h \
    lib/plane.h \
    lib/triangle.h \
    lib/line.h

SOURCES += \
    lib/mesa_gl_math.c \
#    lib/camera.c \
    lib/vector3.c \
    lib/bound.c \
    lib/plane.c \
    lib/triangle.c \
    lib/line.c

# NL-GL
DEPENDPATH += ./src/gl
INCLUDEPATH += ./src/gl

HEADERS += \
    src/gl/nl_font.h \
    src/gl/gl_texture.h \
    src/gl/nl_texture.h \
    src/gl/nl_gl.h

SOURCES += \
    src/gl/gl_texture.c \
    src/gl/nl_texture.c \
    src/gl/nl_gl_font.c \
    src/gl/nl_font.c \
    src/gl/nl_gl_reader.c \
    src/gl/nl_gl_render.c \
    src/gl/nl_gl.c \
    src/gl/nl_gl_re3d.c

# Mesa GL math
DEPENDPATH += ./lib/mesa_math
INCLUDEPATH += ./lib/mesa_math

HEADERS += \
           lib/mesa_math/m_clip_tmp.h \
           lib/mesa_math/m_copy_tmp.h \
           lib/mesa_math/m_debug.h \
           lib/mesa_math/m_debug_util.h \
           lib/mesa_math/m_dotprod_tmp.h \
           lib/mesa_math/m_eval.h \
           lib/mesa_math/m_matrix.h \
           lib/mesa_math/m_norm_tmp.h \
           lib/mesa_math/m_trans_tmp.h \
           lib/mesa_math/m_translate.h \
           lib/mesa_math/m_vector.h \
           lib/mesa_math/m_xform.h \
           lib/mesa_math/m_xform_tmp.h \
           lib/mesa_math/main/compiler.h \
           lib/mesa_math/main/config.h \
           lib/mesa_math/main/glheader.h \
           lib/mesa_math/main/imports.h \
           lib/mesa_math/main/macros.h
SOURCES += \
           lib/mesa_math/m_debug_clip.c \
           lib/mesa_math/m_debug_norm.c \
           lib/mesa_math/m_debug_xform.c \
           lib/mesa_math/m_eval.c \
           lib/mesa_math/m_matrix.c \
           lib/mesa_math/m_translate.c \
           lib/mesa_math/m_vector.c \
           lib/mesa_math/m_xform.c \
           lib/mesa_math/main/imports.c

# Test
CONFIG(debug, debug|release) {
DEFINES += _DEV_TEST
INCLUDEPATH += ./src/test
DEPENDPATH += ./src/test

HEADERS += \
    src/test/testviewer.h \
    src/test/testrenderer.h \
    src/test/testscene.h

SOURCES += \
    src/test/testviewer.cpp \
    src/test/testrenderer.cpp \
    src/test/testscene.cpp
} else {
}

INSTALLS += qnetlizard
