#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <GL/gl3w.h>
#include <QGLWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <glm/gtc/matrix_transform.hpp>

#include "simplescene.h"
#include "UserToolState.h"

enum CylinderProjState{
    PPPPP,
    SSSSSSS
};

enum ConstraintProjState{
    MAKE_CONSTRAINTS,
    APPLIED,
};

class MainView : public QGLWidget
{
    Q_OBJECT

private:
    QTimer *timer;
    SimpleScene *scene;
    UserToolState currentToolState;
    ConstraintProjState constraintProjState;

public:
    MainView( const QGLFormat & format, QWidget *parent = 0 );
    void zoomSlide(int zoom);

protected:
    bool mouseDown;
    bool ctrlDown;
    bool altDown;
    bool shiftDown;
    bool qDown;
    bool iDown;
    bool sDown;
    bool dDown;
    bool eDown;
    bool tDown;
    bool bDown;
    bool lDown;
    bool mDown;
    bool oDown;
    bool pDown;
    bool extrudeProcess;
    bool isMovingPoint;
    bool isDrawing;
    bool isRectSelecting;
    float oldMousePosX;
    float oldMousePosY;

    //stateM
    bool isRadiusSelection;
    bool isHorizontalSelection;
    bool isVerticalSelection;
    float selectionBuffer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

public slots:
    void timerUpdate();
    void toggleAnimation();
    void takeScreenShot();
    void switchModel();
    // file
    void loadModelFromFile(const char* fileName);
    void saveProject(const char* fileName);
    void openProject(const char* fileName);
    void importModel(const char* fileName);

    //tool
    void cylinderProj();
    void linearReg();
    void projectToSurface();
    void constraintProjection();
    void changeState(UserToolState state);
};

#endif // MAINVIEW_H
