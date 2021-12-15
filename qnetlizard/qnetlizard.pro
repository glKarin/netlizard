######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 17 20:37:54 2016
######################################################################

TEMPLATE = app
TARGET = ../qnetlizard

QT += opengl xml

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
    src/misc/settings.h

SOURCES += \
    main.cpp \
    src/qdef.cpp \
    src/utils/ioutility.cpp \
    src/misc/logoutput.cpp \
    src/misc/settings.cpp

# Widget
HEADERS += \
    src/widget/baseviewer.h \
    src/widget/settingwidget.h \
    src/widget/statusbar.h \
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
    src/settingdialog.h \
    src/indexviewer.h \
    src/spriteviewer.h \
    src/changelogdialog.h

SOURCES += \
    src/widget/baseviewer.cpp \
    src/widget/settingwidget.cpp \
    src/widget/statusbar.cpp \
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
    src/settingdialog.cpp \
    src/indexviewer.cpp \
    src/spriteviewer.cpp \
    src/changelogdialog.cpp

# Component
DEPENDPATH += ./src/comp ./src/comp/scene ./src/comp/component ./src/comp/renderer ./src/comp/widget
INCLUDEPATH += ./src/comp ./src/comp/scene ./src/comp/component ./src/comp/renderer ./src/comp/widget

HEADERS += \
    src/comp/simplecameraactor.h \
    src/comp/simplelightsourceactor.h
SOURCES += \
    src/comp/simplecameraactor.cpp \
    src/comp/simplelightsourceactor.cpp

HEADERS += \
    src/comp/component/simplecontrolcomponent.h \
    src/comp/component/simplecontrol2dcomponent.h \
    src/comp/component/simpleimagecontrolcomponent.h \
    src/comp/component/simplecameracomponent.h \
    src/comp/component/simplelightsourcecomponent.h \
    src/comp/component/mapeventhandlercomponent.h
SOURCES += \
    src/comp/component/simplecontrolcomponent.cpp \
    src/comp/component/simplecontrol2dcomponent.cpp \
    src/comp/component/simpleimagecontrolcomponent.cpp \
    src/comp/component/simplecameracomponent.cpp \
    src/comp/component/simplelightsourcecomponent.cpp \
    src/comp/component/mapeventhandlercomponent.cpp

HEADERS += \
    src/comp/renderer/netlizardmapmodelrenderer.h \
    src/comp/renderer/netlizarditemmodelrenderer.h \
    src/comp/renderer/netlizardtexturerenderer.h \
    src/comp/renderer/netlizardfontrenderer.h \
    src/comp/renderer/netlizardanimationmodelrenderer.h \
    src/comp/renderer/netlizardshadowmodelrenderer.h \
    src/comp/renderer/netlizardspriterenderer.h \
    src/comp/renderer/netlizardmapmodeldebugrenderer.h
SOURCES += \
    src/comp/renderer/netlizardmapmodelrenderer.cpp \
    src/comp/renderer/netlizarditemmodelrenderer.cpp \
    src/comp/renderer/netlizardtexturerenderer.cpp \
    src/comp/renderer/netlizardfontrenderer.cpp \
    src/comp/renderer/netlizardanimationmodelrenderer.cpp \
    src/comp/renderer/netlizardshadowmodelrenderer.cpp \
    src/comp/renderer/netlizardspriterenderer.cpp \
    src/comp/renderer/netlizardmapmodeldebugrenderer.cpp

HEADERS += \
    src/comp/scene/imagescene.h \
    src/comp/scene/mapscene.h \
    src/comp/scene/fontscene.h \
    src/comp/scene/animationscene.h \
    src/comp/scene/itemscene.h \
    src/comp/scene/spritescene.h
SOURCES += \
    src/comp/scene/imagescene.cpp \
    src/comp/scene/mapscene.cpp \
    src/comp/scene/fontscene.cpp \
    src/comp/scene/animationscene.cpp \
    src/comp/scene/itemscene.cpp \
    src/comp/scene/spritescene.cpp

HEADERS += \
    src/comp/widget/sceneinfowidget.h \
    src/comp/widget/scenetreewidget.h \
    src/comp/widget/actorpropertywidget.h
SOURCES += \
    src/comp/widget/sceneinfowidget.cpp \
    src/comp/widget/scenetreewidget.cpp \
    src/comp/widget/actorpropertywidget.cpp

# Engine
DEPENDPATH += ./src/engine ./src/engine/editor
INCLUDEPATH += ./src/engine ./src/engine/editor

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
    src/engine/nlmath.h \
    src/engine/nlrigidbody.h \
    src/engine/nlphysics.h \
    src/engine/nlforce.h \
    src/engine/nlforcecontainer.h \
    src/engine/nlproperties.h \
    src/engine/nlglobals.h \
    src/engine/nlscenecamerabase.h \
    src/engine/nlrendermodel.h \
    src/engine/nlmodelrenderer.h

SOURCES += \
    src/engine/nldef.cpp \
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
    src/engine/nlmath.cpp \
    src/engine/nlrigidbody.cpp \
    src/engine/nlphysics.cpp \
    src/engine/nlforce.cpp \
    src/engine/nlforcecontainer.cpp \
    src/engine/nlproperties.cpp \
    src/engine/nlglobals.cpp \
    src/engine/nlscenecamerabase.cpp \
    src/engine/nlrendermodel.cpp \
    src/engine/nlmodelrenderer.cpp

HEADERS += \
    src/engine/editor/nlscenetreewidget.h \
    src/engine/editor/nlsceneinfowidget.h \
    src/engine/editor/nlactorpropwidget.h

SOURCES += \
    src/engine/editor/nlscenetreewidget.cpp \
    src/engine/editor/nlsceneinfowidget.cpp \
    src/engine/editor/nlactorpropwidget.cpp

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
    lib/line.h \
    lib/ortho.h \
    lib/frustum.h \
    lib/matrix.h \
    lib/math_std.h \
    lib/euler.h \
    lib/opengl_render.h

SOURCES += \
    lib/mesa_gl_math.c \
#    lib/camera.c \
    lib/vector3.c \
    lib/bound.c \
    lib/plane.c \
    lib/triangle.c \
    lib/line.c \
    lib/ortho.c \
    lib/frustum.c \
    lib/matrix.c \
    lib/euler.c \
    lib/opengl_render.c

# NL-GL
DEPENDPATH += ./src/gl
INCLUDEPATH += ./src/gl

HEADERS += \
    src/gl/gl_texture.h \
    src/gl/nl_texture.h \
    src/gl/nl_gl.h \
    src/gl/nl_shadow_render.h \
    src/gl/stencil_shadow.h \
    src/gl/glk.h \
    src/gl/nl_util.h \
    src/gl/nl_gl_debug.h \
    src/gl/nl_algo.h \
    src/gl/nl_gl_algo_debug.h

SOURCES += \
    src/gl/gl_texture.c \
    src/gl/nl_texture.c \
    src/gl/nl_gl_font.c \
    src/gl/nl_gl_reader.c \
    src/gl/nl_gl_render.c \
    src/gl/nl_gl.c \
    src/gl/nl_gl_re3d.c \
    src/gl/nl_gl_sprite.c \
    src/gl/nl_shadow_render.cpp \
    src/gl/stencil_shadow.cpp \
    src/gl/glk.c \
    src/gl/nl_util.c \
    src/gl/nl_gl_debug.c \
    src/gl/nl_algo.c \
    src/gl/nl_gl_algo_debug.cpp

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
    src/test/testscene.h

SOURCES += \
    src/test/testviewer.cpp \
    src/test/testscene.cpp
} else {
}

RESOURCES += \
    qnetlizard.qrc

MOC_DIR = ./moc
OBJECTS_DIR = ./obj
RCC_DIR = ./
UI_DIR = ./ui

#i18n.files = \
#    i18n/$${TARGET}.zh_CN.ts \
#    i18n/$${TARGET}.zh_CN.qm
#i18n.path = ./i18n

#INSTALLS        += i18n

OTHER_FILES += README.md \
                debian/changelog \
                resource/img/lz.png \
                resource/img/logo.png \
                resource/cfg/help.xml \
                resource/cfg/setting.xml

INSTALLS += qnetlizard
