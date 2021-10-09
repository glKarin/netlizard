######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 17 20:37:54 2016
######################################################################

TEMPLATE = app
TARGET = ../qnetlizard

QT += opengl

DEPENDPATH += . .. ../libnetlizard ./src ./src/gl ./src/utils ./src/widget ./src/component ./src/component/engine
INCLUDEPATH += . .. ../libnetlizard ./src ./src/gl ./src/utils ./src/widget ./src/component ./src/component/engine

PRE_TARGETDEPS += ../libnetlizard/debug/libnetlizard.a
LIBS += ../libnetlizard/debug/libnetlizard.a

# Input
HEADERS += \
#    src/imageviewer.h \
#    src/widget/imagewidget.h \
    src/textviewer.h \
    src/stringviewer.h \
    src/mainwindow.h \
    src/qdef.h \
    src/widget/mapwidget.h \
    src/mapviewer.h \
    src/utils/ioutility.h \
#    src/widget/fontwidget.h \
#    src/fontviewer.h \
#    src/widget/itemwidget.h \
#    src/itemviewer.h \
    src/widget/baseviewer.h \
    src/helpdialog.h \
    src/aboutdialog.h \
    src/component/logoutput.h \
    src/logdialog.h \
    src/component/engine/nlscene.h \
    src/component/engine/nlobject.h \
    src/component/engine/nlactor.h \
    src/component/engine/nlrenderable.h \
    src/component/engine/nlobjectcontainer.h \
    src/component/engine/nlactorcontainer.h \
    src/component/engine/nlcomponent.h \
    src/component/engine/nlcomponentcontainer.h \
    src/component/engine/nlobjectpool.h \
    src/component/simplecontrolcomponent.h \
    src/component/simplecameracomponent.h \
    src/component/simplecameraactor.h \
    src/component/netlizardmapmodelrenderer.h \
    src/component/engine/scenecamera.h \
    src/component/engine/sceneorthocamera.h \
    src/component/engine/sceneperspectivecamera.h

SOURCES += \
    main.cpp \
#    src/imageviewer.cpp \
#    src/widget/imagewidget.cpp \
    src/textviewer.cpp \
    src/stringviewer.cpp \
    src/mainwindow.cpp \
    src/widget/mapwidget.cpp \
    src/mapviewer.cpp \
    src/utils/ioutility.cpp \
 #   src/widget/fontwidget.cpp \
#    src/fontviewer.cpp \
#    src/widget/itemwidget.cpp \
#    src/itemviewer.cpp \
    src/widget/baseviewer.cpp \
    src/helpdialog.cpp \
    src/aboutdialog.cpp \
    src/component/logoutput.cpp \
    src/logdialog.cpp \
    src/component/engine/nlscene.cpp \
    src/component/engine/nlobject.cpp \
    src/component/engine/nlactor.cpp \
    src/component/engine/nlrenderable.cpp \
    src/component/engine/nlobjectcontainer.cpp \
    src/component/engine/nlactorcontainer.cpp \
    src/component/engine/nlcomponent.cpp \
    src/component/engine/nlcomponentcontainer.cpp \
    src/component/engine/nlobjectpool.cpp \
    src/component/simplecontrolcomponent.cpp \
    src/component/simplecameracomponent.cpp \
    src/component/simplecameraactor.cpp \
    src/component/netlizardmapmodelrenderer.cpp \
    src/component/engine/scenecamera.cpp \
    src/component/engine/sceneorthocamera.cpp \
    src/component/engine/sceneperspectivecamera.cpp

# NL-GL
HEADERS += \
    lib/mesa_gl_math.h \
    lib/camera.h \
    lib/vector3.h \
    src/gl/nl_font.h \
    src/gl/gl_texture.h \
    src/gl/nl_texture.h \
    src/gl/nl_gl.h

SOURCES += \
    lib/mesa_gl_math.c \
    lib/camera.c \
    lib/vector3.c \
    src/gl/gl_texture.c \
    src/gl/nl_texture.c \
    src/gl/nl_gl_font.c \
    src/gl/nl_font.c \
    src/gl/nl_gl_reader.c \
    src/gl/nl_gl_render.c \
    src/gl/nl_gl.c

# Mesa GL math
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


INSTALLS += qnetlizard
