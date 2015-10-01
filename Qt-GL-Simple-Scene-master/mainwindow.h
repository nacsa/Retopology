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

private:
    MainView *glView;

    void createActions();
    void createMenus();

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
    QToolBar *modeToolBar;

};

#endif // MAINWINDOW_H
