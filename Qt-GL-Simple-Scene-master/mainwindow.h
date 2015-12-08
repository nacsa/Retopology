#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QContextMenuEvent>
#include "mainview.h"

//class QAction;
//class QActionGroup;
//class QLabel;
//class QMenu;
//class QContextMenuEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //explicit MainWindow(QWidget *parent = 0);
    MainWindow();
signals:

public slots:
    void sliderValueChanged(int value);
protected:
//    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    // file
    void newProject();
    void openProject();
    void saveProject();
    void openModel();
    void exportTopology();
    void importTopology();

    //tool
    void cylinderProj();
    void linearReg();

    //rest
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();
    void button1Toggled();
    void button2Toggled();

    //Common toolbar
    void selectButtonToggled();
    void translateButtonToggled();
    void rotateButtonToggled();
    void scaleButtonToggled();
    void selectPointButtonToggled();
    void selectPointIslandButtonToggled();
    void selectEdgeButtonToggled();
    void selectEdgeBorderButtonToggled();
    void selectTriangleButtonToggled();
    void selectQuadButtonToggled();

    //Simple toolbar
    void addPointButtonToggled();
    void addPointAutoButtonToggled();
    void addEdgeButtonToggled();
    void extrudeEdgeButtonToggled();
    void divideEdgeButtonToggled();
    void divideTriangleButtonToggled();

    //Advenced toolbar
    void cylinderProjButtonToggled();
    void similarProjButtonToggled();
    void constraintProjButtonToggled();

private:
    MainView *glView;

    void createActions();
    void createMenus();
    void createSimpleToolbar();
    QWidget *createLeftActionToolbars();
    QWidget *createCommonActionToolbar();
    QWidget* createSimpleActionToolbar();
    QWidget* createAdvencedActionToolbar();
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;

    //File menu actions
    QAction *newProjectAct;
    QAction *openProjectAct;
    QAction *saveProjectAct;
    QAction *openModelAct;
    QAction *exportTopologyAct;
    QAction *importTopologyAct;

    // Tool menu actions
    QAction *cylinderProjAct;
    QAction *linearRegAct;

    //TMP remove
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;

    QToolBar *toolBar;
    QToolBar *simpleToolBar;
    QToolBar *advancedToolBar;
    QToolBar *modeToolBar;

    QToolButton* qtoolButton1;
    QToolButton* qtoolButton2;
    QMenu* but1menu;

    //Common toolbar
    QToolButton* selectButton;
    QToolButton* translateButton;
    QToolButton* rotateButton;
    QToolButton* scaleButton;
    QToolButton* selectPointButton;
    QToolButton* selectPointIslandButton;
    QToolButton* selectEdgeButton;
    QToolButton* selectEdgeBorderButton;
    QToolButton* selectTriangleButton;
    QToolButton* selectQuadButton;
    QToolButton* selectPointHelperButton;
    QMenu* selectPointHelperMenu;
    QToolButton* selectEdgeHelperButton;
    QMenu* selectEdgeHelperMenu;

    //Simple toolbar
    QToolButton* addPointButton;
    QToolButton* addPointAutoButton;
    QToolButton* addEdgeButton;
    QToolButton* extrudeEdgeButton;
    QToolButton* divideEdgeButton;
    QToolButton* divideTriangleButton;
    QToolButton* addPointHelperButton;
    QMenu* addPointHelperMenu;

    //Advenced toolbar
    QToolButton* cylinderProjButton;
    QToolButton* similarProjButton;
    QToolButton* constraintProjButton;

    void toggleOffAllTools();
    void toggleOffCommonTools();
    void toggleOffSelectionTools();
    void toggleOffManipulatorTools();
    void toggleOffSimpleTools();
    void toggleOffAdvancedTools();
    void toggleOffNotCommonTools();

};

#endif // MAINWINDOW_H
