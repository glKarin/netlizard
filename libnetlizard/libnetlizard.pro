######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 17 20:37:54 2016
######################################################################

TEMPLATE = lib
TARGET = netlizard

CONFIG += plugin
CONFIG += static
#CONFIG += shared

# Input
HEADERS += \
                                         netlizard.h \
					 netlizard_3d.h \
    def.h \
    jport.h \
    log.h \
    io.h \
    util.h \
    texture.h \
    priv_local.h \
    soil/image.h \
    soil/image_dxt.h \
    soil/stb_image_aug.h \
    soil/stbi_dds_aug_c.h \
    soil/stbi_dds_aug.h \
    media_util.h

SOURCES += \
    jport.c \
    io.c \
    png.c \
    util.c \
    texture_v2.c \
    texture_v3.c \
    source_string.c \
    text.c \
    log.c \
    netlizard_3d.c \
    ct3d.c \
    texture.c \
    soil/image.c \
    soil/image_dxt.c \
    soil/stb_image_aug.c \
    texture_v2_file.c \
    media_util.c \
    spirit.c \
    font.c \
    texture_v3_file.c \
    png_file.c \
    ct3dep2.c \
    specnaz3d.c \
    egypt3d.c \
    ct3dep3.c

#libnetlizard.files = $$DESTDIR/libnetlizard*
#libnetlizard.path = /usr/lib

#INSTALLS += libnetlizard
