#include "mainview.h"
#include "simplescene.h"

#include <iostream>
using std::cout;
using std::endl;

MainView::MainView(const QGLFormat & format, QWidget *parent) : QGLWidget(format, parent)
{
    // This tells the widget to accept keyboard focus when the widget is clicked.
    this->setFocusPolicy(Qt::ClickFocus);

    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(timerUpdate()) );
    this->mouseDown = false;
    this->ctrlDown = false;
    this->altDown = false;
    this->shiftDown = false;
    this->qDown = false;
    this->sDown = false;
    this->dDown = false;
    this->eDown = false;
    this->tDown = false;
    this->lDown = false;
    this->mDown = false;
    this->oDown = false;
    this->pDown = false;
    this->iDown = false;
    this->extrudeProcess = false;
    this->isMovingPoint = false;
    this->isDrawing = false;
    this->isRectSelecting = false;
    isRadiusSelection = false;
    isHorizontalSelection = false;
    isVerticalSelection = false;
    selectionBuffer = 0;
    this->setMinimumSize(800,800);
    this->setMouseTracking(true);

    constraintProjState = ConstraintProjState::MAKE_CONSTRAINTS;
}

void MainView::zoomSlide(int zoom){
    scene->zoomSlide(zoom);
}

void MainView::initializeGL()
{
    scene = new SimpleScene();

    if (gl3wInit()==-1)
    {
        std::cerr << "Failed to initialize OpenGL.\n";
        return;
     }
    std::cout << "\nOpenGL " << glGetString(GL_VERSION) << "\nGLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

//     if (!gl3wIsSupported(4, 3))
//     {
//        std::cerr << "OpenGL 4.3 is not supported.\n";
//        return;
//     }
     std::cout << "\nOpenGL " << glGetString(GL_VERSION) << "\nGLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glClearColor(0.2f,0.2f,0.2f,1.0f);


    scene->initScene();
}

void MainView::paintGL()
{
    if(context() != QGLContext::currentContext())
    {
        std::cout << "\n hibaaaaaa";
    }

    scene->render();
}

void MainView::resizeGL(int w, int h )
{
    scene->resize(w,h);
}
void MainView::mousePressEvent(QMouseEvent *event){
     if (event->button() == Qt::RightButton){
         mouseDown = true;
         oldMousePosX = (float)event->pos().x() / (float)this->size().width();
         oldMousePosY = (float)event->pos().y() / (float)this->size().height();
         //toggleAnimation();
     }else{
         //TODO: rectangle selection
         //TODO: mode alapján mehet rektangle, kitalálni hogy
         switch(currentToolState){
         case SELECT_POINT:
             //if(scene->setActivePoint(event->pos().x(), event->pos().y())){
                // isMovingPoint = true; //TODO: pont mozgatas!
                //TODO: enum átadása paraméterben
             if(shiftDown){
                scene->addActivePoint(event->pos().x(), event->pos().y());
             }else if(altDown){
                scene->unsetActivePoint(event->pos().x(), event->pos().y());
             }else{
                 scene->setActivePoint(event->pos().x(), event->pos().y());
             }
             break;
         case SELECT_POINT_ISLAND:
              scene->selectPointIsland(event->pos().x(), event->pos().y());
              //TODO: megoldani hogy shift cuccok menjenek
              break;
         case SELECT_EDGE:
             if(shiftDown){
                scene->addActiveEdge(event->pos().x(), event->pos().y());
             }else if(altDown){
                scene->unsetActiveEdge(event->pos().x(), event->pos().y());
             }else{
                scene->setActiveEdge(event->pos().x(), event->pos().y());
             }
             break;
         case SELECT_BORDER_EDGES:
             scene->selectBorderEdgeLine(event->pos().x(), event->pos().y());
             //TODO: megoldani hogy shift cuccok menjenek
             break;
         case ADD_POINT:
             scene->makeNewPoint(event->pos().x(), event->pos().y());
             scene->clearPointSelection();
             break;
         case ADD_POINT_CHAIN:
             scene->makeNewPoint(event->pos().x(), event->pos().y());
             break;
         case EXTRUDE_EDGE:
             extrudeProcess = true;scene->tmpExtrudeFirstTriangle(event->pos().x(), event->pos().y());//scene->extrudeEdge(event->pos().x(), event->pos().y());
             break;
         case DIVIDE_EDGE:
             scene->divideEdge(event->pos().x(), event->pos().y());
             break;
         case DIVIDE_TRIANGLE:
             scene->divideTriangle(event->pos().x(), event->pos().y());
             break;
         case CYLINDER_PROJECTION:
             isDrawing = true;
             scene->startDrawingOnSurface(event->pos().x(), event->pos().y());
             if(isRadiusSelection){
                 isRadiusSelection = false;
                 isHorizontalSelection = true;
                 selectionBuffer = 0;
             }else if(isHorizontalSelection){
                 isHorizontalSelection = false;
                 isVerticalSelection = true;
                 selectionBuffer = 0;
             }else if(isVerticalSelection){
                 isVerticalSelection = false;
                 selectionBuffer = 0;
                 scene->projectTMPCylinderViaShader();
             }
             break;
         case CONSTRAINT_PROJECTION:
             scene->constaintProjection(event->pos().x(), event->pos().y());
             break;
         case MANIP_TRANSLATE:
             scene->moveManipulatorStart(event->pos().x(), event->pos().y());
             break;
         case MANIP_ROTATE:
             scene->rotateManipulatorStart(event->pos().x(), event->pos().y());
             break;
         case MANIP_SCALE:
             scene->scaleManipulatorStart(event->pos().x(), event->pos().y());
             break;

         }

         updateGL();
     }
}
/*
void MainView::mousePressEvent(QMouseEvent *event){
     if (event->button() == Qt::RightButton){
         mouseDown = true;
         oldMousePosX = (float)event->pos().x() / (float)this->size().width();
         oldMousePosY = (float)event->pos().y() / (float)this->size().height();
         //toggleAnimation();
     }else if (event->button() == Qt::LeftButton && qDown){
        scene->divideTriangle(event->pos().x(), event->pos().y());
        updateGL();
     }else if (event->button() == Qt::LeftButton){
        //scene->onClickEvent(event->pos().x(), event->pos().y());
        if(ctrlDown){
            if(scene->setActivePoint(event->pos().x(), event->pos().y())){
                isMovingPoint = true;
            }else{
                scene->setActiveEdge(event->pos().x(), event->pos().y());
            }
        }else if(dDown){
            scene->divideEdge(event->pos().x(), event->pos().y());

        }else if(eDown){
            extrudeProcess = scene->extrudeEdge(event->pos().x(), event->pos().y());
        }else if(tDown){
            scene->tmpExtrudeFirstTriangle(event->pos().x(), event->pos().y());
            extrudeProcess = true;
        }else if(lDown){
            scene->selectBorderEdgeLine(event->pos().x(), event->pos().y());
        }else if(altDown){
            if(!scene->unsetActivePoint(event->pos().x(), event->pos().y()))
                scene->unsetActiveEdge(event->pos().x(), event->pos().y());
        }else if(shiftDown){
            if(!scene->addActivePoint(event->pos().x(), event->pos().y()))
                scene->addActiveEdge(event->pos().x(), event->pos().y());
        }else if(mDown){
            isDrawing = true;
            scene->startDrawingOnSurface(event->pos().x(), event->pos().y());
        }else if(oDown){
            isRectSelecting = true;
            scene->startRectangleSelection(event->pos().x(), event->pos().y());
        }else if(iDown){
            scene->selectPointIsland(event->pos().x(), event->pos().y());
        }else if(isRadiusSelection){
            isRadiusSelection = false;
            isHorizontalSelection = true;
            selectionBuffer = 0;
        }else if(isHorizontalSelection){
            isHorizontalSelection = false;
            isVerticalSelection = true;
            selectionBuffer = 0;
        }else if(isVerticalSelection){
            isVerticalSelection = false;
            selectionBuffer = 0;
            scene->projectTMPCylinderViaShader();
        }else{
           scene->moveManipulatorStart(event->pos().x(), event->pos().y());
           scene->rotateManipulatorStart(event->pos().x(), event->pos().y());
           scene->scaleManipulatorStart(event->pos().x(), event->pos().y());
        }
        /*else{
            scene->makeNewPoint(event->pos().x(), event->pos().y());
        }*//*

        updateGL();
     }
}*/



void MainView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton){
        mouseDown = false;
        isMovingPoint = false;
        //toggleAnimation();
    }else if(event->button() == Qt::LeftButton){
        if(extrudeProcess){
            extrudeProcess = false;
            scene->stopExtrude();
        }
        if(isDrawing){
            isDrawing = false;
            scene->stopDrawingOnSurface(event->pos().x(), event->pos().y());
            scene->linearRegression();
            isRadiusSelection = true;
        }
        if(isRectSelecting){
            isRectSelecting = false;
            scene->stopRectangleSelection(event->pos().x(), event->pos().y());
            updateGL();
        }else{
            scene->moveManipulatorEnd(event->pos().x(), event->pos().y());
            scene->rotateManipulatorEnd(event->pos().x(), event->pos().y());
            scene->scaleManipulatorEnd(event->pos().x(), event->pos().y());
        }
    }
}



/*
void MainView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton){
        mouseDown = false;
        isMovingPoint = false;
        //toggleAnimation();
    }else if(event->button() == Qt::LeftButton){
        if(extrudeProcess){
            extrudeProcess = false;
            scene->stopExtrude();
        }
        if(isDrawing){
            isDrawing = false;
            scene->stopDrawingOnSurface(event->pos().x(), event->pos().y());
            scene->linearRegression();
            isRadiusSelection = true;
        }
        if(isRectSelecting){
            isRectSelecting = false;
            scene->stopRectangleSelection(event->pos().x(), event->pos().y());
            updateGL();
        }else{
            scene->moveManipulatorEnd(event->pos().x(), event->pos().y());
            scene->rotateManipulatorEnd(event->pos().x(), event->pos().y());
            scene->scaleManipulatorEnd(event->pos().x(), event->pos().y());
        }
    }
}*/
void MainView::mouseMoveEvent(QMouseEvent *event){
   //printf("%d ;  ", event->pos().x());
    float x = (float)event->pos().x() / (float)this->size().width();
    float y = (float)event->pos().y() / (float)this->size().height();

    scene->moveManipulatorMove(event->pos().x(), event->pos().y());
    scene->rotateManipulatorMove(event->pos().x(), event->pos().y());
    scene->scaleManipulatorMove(event->pos().x(), event->pos().y());

   if(mouseDown && altDown){
       //float x = (float)event->pos().x() / (float)this->size().width();
       //float y = (float)event->pos().y() / (float)this->size().height();

       scene->moveModel(x-oldMousePosX,y-oldMousePosY);
       updateGL();
      // oldMousePosX = x;
       //oldMousePosY = y;
   }else if(isMovingPoint){
       scene->moveActivePoint(event->pos().x(), event->pos().y());
       updateGL();

   }else if(extrudeProcess){
       scene->moveExtrudablePoint(event->pos().x(), event->pos().y());
       updateGL();

   }else if(isDrawing){
        scene->drawingOnSurface(event->pos().x(), event->pos().y());
   }else if(isRectSelecting){
       scene->rectangleSelection(event->pos().x(), event->pos().y());
       updateGL();
   }else if(isRadiusSelection){
        scene->setCylinderRadius(x-oldMousePosX);
        updateGL();
   }else if(isHorizontalSelection){
       selectionBuffer += x-oldMousePosX;
       if(selectionBuffer > 0.1){
           selectionBuffer = 0;
           scene->setCylinderHorizontal(1);
       }else if(selectionBuffer < -0.1){
           selectionBuffer = 0;
           scene->setCylinderHorizontal(-1);
       }
       updateGL();
   }else if(isVerticalSelection){
       selectionBuffer += x-oldMousePosX;
       if(selectionBuffer > 0.1){
           selectionBuffer = 0;
           scene->setCylinderVertical(1);
       }else if(selectionBuffer < -0.1){
           selectionBuffer = 0;
           scene->setCylinderVertical(-1);
       }
       updateGL();

   }else if(mouseDown){
       //float x = (float)event->pos().x() / (float)this->size().width();
      // float y = (float)event->pos().y() / (float)this->size().height();

       scene->rotateScene(x-oldMousePosX,y-oldMousePosY);
       updateGL();
       //oldMousePosX = x;
      // oldMousePosY = y;

   }else if(sDown){
        scene->indicateSnappingPoint(event->pos().x(), event->pos().y());
        updateGL();
   }

   updateGL();
   oldMousePosX = x;
   oldMousePosY = y;
   //updateGL();

}

void MainView::keyPressEvent(QKeyEvent *event){
    if(currentToolState == UserToolState::CONSTRAINT_PROJECTION){
        if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
            if(constraintProjState == ConstraintProjState::MAKE_CONSTRAINTS){
                scene->applyConstaintProjection();
                constraintProjState = ConstraintProjState::APPLIED;
            }else if(constraintProjState == ConstraintProjState::APPLIED){
                scene->finishConstaintProjection();
            }
            updateGL();
        }else if(event->key()== Qt::Key_Escape){
            if(constraintProjState == ConstraintProjState::APPLIED){
                scene->revertConstaintProjection();
                constraintProjState = ConstraintProjState::MAKE_CONSTRAINTS;
            }
            updateGL();
        }
    }
    if(event->key()==Qt::Key_Control){
        ctrlDown = true;
    }else if(event->key() == Qt::Key_Alt){
        altDown = true;
    }else if(event->key()==Qt::Key_Shift){
        shiftDown = true;
    }else if(event->key() == Qt::Key_Q){
        qDown = true;
    }else if(event->key() == Qt::Key_D){
        dDown = true;
    }else if(event->key() == Qt::Key_E){
        eDown = true;
    }else if(event->key() == Qt::Key_T){
        tDown = true;
    }else if(event->key() == Qt::Key_M){
        mDown = true;
    }else if(event->key() == Qt::Key_S){
        sDown = true;
        scene->setSnapping(true);
    }else if(event->key() == Qt::Key_G){
        updateGL();
    }else if(event->key() == Qt::Key_L){
        lDown = true;
    }else if(event->key() == Qt::Key_O){
        oDown = true;
    }else if(event->key() == Qt::Key_B){
        scene->selectBorderEdges();
        updateGL();
    }else if(event->key() == Qt::Key_C){
        scene->clearEdgeSelection();
        updateGL();
    }else if(event->key() == Qt::Key_P){
        scene->projectToSimilarModel();
        updateGL();
    }else if(event->key() == Qt::Key_I){
        iDown = true;
    }else if(event->key() == Qt::Key_A){
       scene->setShowMoveGizmo(false);
       updateGL();
    }else if(event->key() == Qt::Key_Y){
        scene->switchShowRotateGizmo();
        updateGL();
     }else if(event->key() == Qt::Key_X){
        scene->switchShowScaleGizmo();
        updateGL();
     }
}

void MainView::keyReleaseEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Control){
        ctrlDown = false;
        isMovingPoint = false;
    }else if(event->key() == Qt::Key_Alt){
        altDown = false;
    }else if(event->key() == Qt::Key_Shift){
        shiftDown = false;
    }else if(event->key() == Qt::Key_S){
        sDown = false;
        scene->setSnapping(false);
    }else if(event->key() == Qt::Key_Q){
        qDown = false;
    }else if(event->key() == Qt::Key_D){
        dDown = false;
    }else if(event->key() == Qt::Key_E){
        eDown = false;
    }else if(event->key() == Qt::Key_T){
       tDown = false;
    }else if(event->key() == Qt::Key_M){
        mDown = false;
    }else if(event->key() == Qt::Key_L){
        lDown = false;
    }else if(event->key() == Qt::Key_O){
        oDown = false;
    }else if(event->key() == Qt::Key_I){
        iDown = false;
    }else if(event->key()== Qt::Key_Delete){
        scene->removeActivePoint();
        scene->removeActiveEdge();
        updateGL();
    }
}

void MainView::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta();
    int numStep = numDegrees/15;

    scene->zoomSlide(numStep);
    updateGL();
}


void MainView::takeScreenShot()
{
    QImage img = this->grabFrameBuffer(true);
    img.save("screen.png", "PNG");
}

void MainView::toggleAnimation()
{
    if( timer->isActive() )
        timer->stop();
    else
        timer->start(16);
}

void MainView::switchModel()
{
//    static int activeBuffer = 1;
//    activeBuffer = activeBuffer == 1 ? 2 : 1;
//    scene->changeScene(activeBuffer);
    //    updateGL();
}

void MainView::loadModelFromFile(const char *fileName)
{
    scene->loadModelFromFile(fileName);
}

void MainView::saveProject(const char *fileName)
{
    scene->saveProject(fileName);
}

void MainView::openProject(const char *fileName)
{
    scene->openProject(fileName);
}

void MainView::importModel(const char *fileName)
{
    scene->importModel(fileName);
    updateGL();
}

void MainView::cylinderProj()
{
    //scene->projectTMPCylinder();
    scene->projectTMPCylinderViaShader();
    updateGL();
}

void MainView::linearReg()
{
    scene->linearRegression();
}

void MainView::projectToSurface()
{
    scene->projectToSimilarModel();
    updateGL();
}

void MainView::constraintProjection()
{
    scene->startConstaintProjection();
    updateGL();
}

void MainView::changeState(UserToolState state)
{
    currentToolState = state;
    switch(currentToolState){
    case MANIP_TRANSLATE:
        scene->setShowMoveGizmo(false);
        break;
    case MANIP_ROTATE:
        scene->switchShowRotateGizmo();
        break;
    case MANIP_SCALE:
        scene->switchShowScaleGizmo();
        break;
    }
}

void MainView::timerUpdate()
{
    //scene->update(0.2f);
    updateGL();
}

