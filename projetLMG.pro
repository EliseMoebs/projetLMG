#Specify this is an app, and not a lib or what
TEMPLATE = app

#Set the name of the app
Debug:TARGET        = projetLMG_debug
Release:TARGET      = projetLMG

QT = opengl

DESTDIR = $$PWD/bin

app_data.path    = $${DESTDIR}
app_data.files   = $$PWD/data

INSTALLS += app_data

win32:DEPLOY_COMMAND    = windeployqt
macx:DEPLOY_COMMAND     = macdeployqt


Debug:BUILDDIR      = debug
Release:BUILDDIR    = release
#OBJECTS_DIR         = $${BUILDDIR}/.obj
#MOC_DIR             = $${BUILDDIR}/.moc
#RCC_DIR             = $${BUILDDIR}/.rcc
#UI_DIR              = $${BUILDDIR}/.ui


INCLUDEPATH +=  $$PWD/src
INCLUDEPATH += include/

VPATH   =   $$PWD/src
HEADERS +=  openglwindow.h \
            main.h \
    src/GPUProgram.h \
    src/common.h \
    src/Mesh.h \
    src/RenderTarget.h \
    src/Texture.h \
    src/GPUParams.h

SOURCES +=  openglwindow.cpp \
            main.cpp \
    src/GPUProgram.cpp \
    src/common.cpp \
    src/Mesh.cpp \
    src/RenderTarget.cpp \
    src/Texture.cpp

DISTFILES += \
    data/VS_scene_map.glsl \
    data/FS_SSAO.glsl \
    data/VS_SSAO.glsl

win32:LIBS      += -lopengl32

#macx:LIBS       += -L$${PWD}/../deps/glew/lib/
#macx:LIBS       += -lGLEW -lGLU
	
#unix!macx:LIBS  += -L$${PWD}/../deps/glew/lib/
#unix!macx:LIBS  += -lGLU



#message(CONFIG: $${CONFIG})
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
