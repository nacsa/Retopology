
QT       += opengl
CONFIG   += console
CONFIG   += c++11

TARGET = simplescene

TEMPLATE = app
INCLUDEPATH += ./include

SOURCES += main.cpp \
    mainview.cpp \
    glslprogram.cpp \
    simplescene.cpp \
    gl3w.c \
    modelimporter.cpp \
    mainwindow.cpp \
    shader.cpp \
    framebuffer.cpp \
    topologypoint.cpp \
    topology.cpp \
    topologyedge.cpp \
    topologyhelperquad.cpp \
    topologyhelpertriangle.cpp \
    camera.cpp \
    projectmanager.cpp \
    cylinderprojection.cpp \
    linearregressor.cpp \
    selectionrectangle.cpp \
    topologyimporter.cpp \
    similarmodelprojection.cpp \
    circlegenerator.cpp \
    cylindergenerator.cpp \
    vertexisland.cpp \
    libgizmo/GizmoTransformMove.cpp \
    libgizmo/GizmoTransformRender.cpp \
    libgizmo/GizmoTransformRotate.cpp \
    libgizmo/GizmoTransformScale.cpp \
    libgizmo/stdafx.cpp \
    libgizmo/ZBaseMaths.cpp \
    libgizmo/ZMathsFunc.cpp \
    modeltoretopo.cpp \
    constraintprojection.cpp \
    constraintpoint.cpp \
    MeshMorph.cpp


HEADERS += \
    mainview.h \
    scene.h \
    glslprogram.h \
    simplescene.h \
    modelimporter.h \
    mainwindow.h \
    shader.hpp \
    framebuffer.hpp \
    topologypoint.h \
    topology.h \
    topologyedge.h \
    topologyhelperquad.h \
    topologyhelpertriangle.h \
    camera.h \
    extrudeedge.h \
    projectmanager.h \
    cylinderprojection.h \
    linearregressor.h \
    selectionrectangle.h \
    topologyimporter.h \
    similarmodelprojection.h \
    circlegenerator.h \
    cylindergenerator.h \
    vertexisland.h \
    libgizmo/IGizmo.h \
    libgizmo/GizmoTransform.h \
    libgizmo/GizmoTransformMove.h \
    libgizmo/GizmoTransformRender.h \
    libgizmo/GizmoTransformRotate.h \
    libgizmo/GizmoTransformScale.h \
    libgizmo/LibBase.h \
    libgizmo/stdafx.h \
    libgizmo/targetver.h \
    libgizmo/ZBaseDefs.h \
    libgizmo/ZBaseMaths.h \
    libgizmo/ZBaseMaths.inl \
    libgizmo/ZCollisionsUtils.h \
    libgizmo/ZMathsFunc.h \
    UserToolState.h \
    modeltoretopo.h \
    constraintprojection.h \
    constraintpoint.h \
    MeshMorph.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    fragment.glsl \
    vertex.glsl \
    color_fragment.glsl \
    color_geometry.glsl \
    color_vertex.glsl \
    newpoint_geometry.glsl \
    newpoint_fragment.glsl \
    newpoint_vertex.glsl \
    pointid_fragment.glsl \
    pointid_vertex.glsl \
    edgeid_fragment.glsl \
    edgeid_vertex.glsl \
    pointnormal_fragment.glsl \
    pointnormal_vertex.glsl \
    edge_fragment.glsl \
    edge_vertex.glsl \
    quad_fragment.glsl \
    quad_vertex.glsl \
    triangle_fragment.glsl \
    triangle_vertex.glsl \
    triangleid_fragment.glsl \
    triangleid_vertex.glsl \
    cylinderproj_fragment.glsl \
    cylinderproj_geometry.glsl \
    cylinderproj_vertex.glsl \
    selectrect_fragment.glsl \
    selectrect_vertex.glsl \
    similarmodelposproj_fragment.glsl \
    similarmodelposproj_vertex.glsl \
    similarmodelnormproj_fragment.glsl \
    similarmodelnormproj_vertex.glsl \
    constraintproj_fragment.glsl \
    constraintproj_geometry.glsl \
    constraintproj_vertex.glsl



INCLUDEPATH += ./include/Assimp/include/assimp
INCLUDEPATH +=  D:/msconlab/Qt-GL-Simple-Scene-master/include/Assimp/include/assimp
INCLUDEPATH +=  "D:\Programok\boost_1_5\include"

LIBS += -L$$PWD/include/Assimp/lib/x64/ -lassimp
LIBS += -L$$PWD/include/LibGizmo/lib -lgizmo


INCLUDEPATH += $$PWD/include/Assimp/include/assimp
INCLUDEPATH += $$PWD/include/LibGizmo/include
INCLUDEPATH += $$PWD/include/boost_1_5/include/

DEPENDPATH += $$PWD/include/Assimp/include/assimp
DEPENDPATH += $$PWD/include/LibGizmo/include
DEPENDPATH += $$PWD/include/boost_1_5/include
