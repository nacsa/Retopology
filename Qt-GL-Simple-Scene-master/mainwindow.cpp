#include "mainwindow.h"
#include "mainview.h"

#include <QtWidgets>
#include "UserToolState.h"

//MainWindow::MainWindow(QWidget *parent) :
//    QMainWindow(parent)
//{
//}

MainWindow::MainWindow(){
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QSlider *slider = new QSlider();
    slider->setFocusPolicy(Qt::StrongFocus);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);
    slider->setSingleStep(1);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(sliderValueChanged(int)));

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                                 "invoke a context menu</i>"));

    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGLFormat format;
    format.setVersion(3,3);
    format.setProfile(QGLFormat::CompatibilityProfile);
    glView = new MainView(format);
    QHBoxLayout *parentLayout = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(1);
    //layout->addWidget(slider);//topFiller
    layout->addWidget(createCommonActionToolbar());
    layout->addWidget(glView);
    //layout->addWidget(bottomFiller);
    parentLayout->addWidget(createLeftActionToolbars());
    parentLayout->addLayout(layout);
    widget->setLayout(parentLayout);


    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);



    setWindowTitle(tr("Retopology"));
    setMinimumSize(160, 160);
    resize(900, 900);

}



void MainWindow::sliderValueChanged(int value)
{
    glView->zoomSlide(value);
}

//void MainWindow::contextMenuEvent(QContextMenuEvent *event)
//{
//    QMenu menu(this);
//    menu.addAction(cutAct);
//    menu.addAction(copyAct);
//    menu.addAction(pasteAct);
//    menu.exec(event->globalPos());
//}

void MainWindow::newProject()
{
    infoLabel->setText(tr("Invoked <b>File|New</b>"));
}


void MainWindow::openProject()
{
    infoLabel->setText(tr("Invoked <b>File|Open Project</b>"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                                     "",
                                                     tr("Project (*.rtp)"));
    if(fileName.isNull() || fileName.isEmpty())
        return;
    glView->openProject(fileName.toStdString().c_str());
}


void MainWindow::saveProject()
{
    infoLabel->setText(tr("Invoked <b>File|Save Project</b>"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"),
                                                     "",
                                                     tr("Project (*.rtp)"));
    if(fileName.isNull() || fileName.isEmpty())
        return;
    glView->saveProject(fileName.toStdString().c_str());
}

void MainWindow::openModel()
{
    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model"),
                                                     "",
                                                     tr("Obj (*.obj)"));
    if(fileName.isNull() || fileName.isEmpty())
        return;
    glView->loadModelFromFile(fileName.toStdString().c_str());
}

void MainWindow::exportTopology()
{
    infoLabel->setText(tr("Invoked <b>File|Export topology</b>"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export topology"),
                                                     "",
                                                     tr("Obj (*.obj)"));
    if(fileName.isNull() || fileName.isEmpty())
        return;
}

void MainWindow::importTopology()
{
    infoLabel->setText(tr("Invoked <b>File|Import topology</b>"));
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import Topology"),
                                                     "",
                                                     tr("Obj (*.obj)"));
    if(fileName.isNull() || fileName.isEmpty())
        return;
    glView->importModel(fileName.toStdString().c_str());
}

void MainWindow::cylinderProj()
{
    glView->cylinderProj();
}

void MainWindow::linearReg()
{
    glView->linearReg();
}


void MainWindow::print()
{
    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void MainWindow::undo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void MainWindow::redo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void MainWindow::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void MainWindow::copy()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void MainWindow::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void MainWindow::bold()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void MainWindow::italic()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void MainWindow::leftAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void MainWindow::rightAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void MainWindow::justify()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void MainWindow::center()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void MainWindow::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void MainWindow::setParagraphSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void MainWindow::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About"),
                      // "This \n asd </br> asdadad");
            tr("Retopology tool, made by Gabor <u>Nacsa</u>.") % QString(" \r \n  </br> ") % tr(" All rights reserved. 2015"));
}

void MainWindow::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}

void MainWindow::button1Toggled()
{
    qtoolButton1->setChecked(true);
    qtoolButton2->setChecked(false);
    but1menu->close();
    //qtoolButton1-
}

void MainWindow::button2Toggled()
{
    qtoolButton1->setChecked(false);
    if(qtoolButton2->isChecked())
        printf("togg");
}

void MainWindow::selectButtonToggled()
{
    toggleOffNotCommonTools();
    toggleOffManipulatorTools();
    selectButton->setChecked(true);
    glView->changeState(UserToolState::SELECT_MODE);
}

void MainWindow::translateButtonToggled()
{
    toggleOffAllTools();
    translateButton->setChecked(true);
    glView->changeState(UserToolState::MANIP_TRANSLATE);
}

void MainWindow::rotateButtonToggled()
{
    toggleOffAllTools();
    rotateButton->setChecked(true);
    glView->changeState(UserToolState::MANIP_ROTATE);
}

void MainWindow::scaleButtonToggled()
{
    toggleOffAllTools();
    scaleButton->setChecked(true);
    glView->changeState(UserToolState::MANIP_SCALE);
}

void MainWindow::selectPointButtonToggled()
{
    toggleOffAllTools();
    selectPointButton->setChecked(true);
    selectPointHelperButton->setChecked(true);
    selectPointHelperMenu->close();
    glView->changeState(UserToolState::SELECT_POINT);
}

void MainWindow::selectPointIslandButtonToggled()
{
    toggleOffAllTools();
    selectPointIslandButton->setChecked(true);
    selectPointHelperButton->setChecked(true);
    selectPointHelperMenu->close();
    glView->changeState(UserToolState::SELECT_POINT_ISLAND);
}

void MainWindow::selectEdgeButtonToggled()
{
    toggleOffAllTools();
    selectEdgeButton->setChecked(true);
    selectEdgeHelperButton->setChecked(true);
    selectEdgeHelperMenu->close();
    glView->changeState(UserToolState::SELECT_EDGE);
}

void MainWindow::selectEdgeBorderButtonToggled()
{
    toggleOffAllTools();
    selectEdgeBorderButton->setChecked(true);
    selectEdgeHelperButton->setChecked(true);
    selectEdgeHelperMenu->close();
    glView->changeState(UserToolState::SELECT_BORDER_EDGES);
}

void MainWindow::selectTriangleButtonToggled()
{
    toggleOffAllTools();
    selectTriangleButton->setChecked(true);
    glView->changeState(UserToolState::SELECT_TRIANGLE);
}

void MainWindow::selectQuadButtonToggled()
{
    toggleOffAllTools();
    selectQuadButton->setChecked(true);
    glView->changeState(UserToolState::SELECT_QUAD);
}

void MainWindow::addPointButtonToggled()
{
    toggleOffNotCommonTools();
    toggleOffManipulatorTools();
    addPointButton->setChecked(true);
    addPointHelperButton->setChecked(true);
    addPointHelperMenu->close();
    glView->changeState(UserToolState::ADD_POINT);
}

void MainWindow::addPointAutoButtonToggled()
{
    toggleOffNotCommonTools();
    toggleOffManipulatorTools();
    addPointAutoButton->setChecked(true);
    addPointHelperButton->setChecked(true);
    addPointHelperMenu->close();
    glView->changeState(UserToolState::ADD_POINT_CHAIN);
}

void MainWindow::addEdgeButtonToggled()
{
    toggleOffNotCommonTools();
    toggleOffManipulatorTools();
    addEdgeButton->setChecked(true);
    glView->changeState(UserToolState::ADD_EDGE);
}

void MainWindow::extrudeEdgeButtonToggled()
{
    toggleOffNotCommonTools();
    toggleOffManipulatorTools();
    extrudeEdgeButton->setChecked(true);
    glView->changeState(UserToolState::EXTRUDE_EDGE);
}

void MainWindow::divideEdgeButtonToggled()
{
    toggleOffAllTools();
    divideEdgeButton->setChecked(true);
    glView->changeState(UserToolState::DIVIDE_EDGE);
}

void MainWindow::divideTriangleButtonToggled()
{
    toggleOffAllTools();
    divideTriangleButton->setChecked(true);
    glView->changeState(UserToolState::DIVIDE_TRIANGLE);
}

void MainWindow::cylinderProjButtonToggled()
{
    toggleOffAllTools();
    cylinderProjButton->setChecked(true);
    glView->changeState(UserToolState::CYLINDER_PROJECTION);
}

void MainWindow::similarProjButtonToggled()
{
    toggleOffAllTools();
    glView->projectToSurface();
}

void MainWindow::constraintProjButtonToggled()
{
    toggleOffAllTools();
    constraintProjButton->setChecked(true);
    glView->changeState(UserToolState::CONSTRAINT_PROJECTION);
    glView->constraintProjection();
}


void MainWindow::createActions()
{
    newProjectAct = new QAction(tr("&New Project"), this);
    newProjectAct->setShortcuts(QKeySequence::New);
    newProjectAct->setStatusTip(tr("Create an empty project"));
    connect(newProjectAct, SIGNAL(triggered()), this, SLOT(newProject()));

    openProjectAct = new QAction(tr("&Open Project..."), this);
    openProjectAct->setShortcuts(QKeySequence::Open);
    openProjectAct->setStatusTip(tr("Open an existing project"));
    connect(openProjectAct, SIGNAL(triggered()), this, SLOT(openProject()));

    saveProjectAct = new QAction(tr("&Save Project"), this);
    saveProjectAct->setShortcuts(QKeySequence::Save);
    saveProjectAct->setStatusTip(tr("Save the project to disk"));
    connect(saveProjectAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    openModelAct = new QAction(tr("Open &Model..."), this);
    openModelAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    openModelAct->setStatusTip(tr("Open an existing background model"));
    connect(openModelAct, SIGNAL(triggered()), this, SLOT(openModel()));


    importTopologyAct = new QAction(tr("&Import Topology..."), this);
    importTopologyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    importTopologyAct->setStatusTip(tr("Import topology from a model"));
    connect(importTopologyAct, SIGNAL(triggered()), this, SLOT(importTopology()));

    exportTopologyAct = new QAction(tr("&Export Topology..."), this);
    exportTopologyAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    exportTopologyAct->setStatusTip(tr("Export topology to a model"));
    connect(exportTopologyAct, SIGNAL(triggered()), this, SLOT(exportTopology()));


    // Tool actions
    cylinderProjAct = new QAction(tr("&Cylinder proj..."), this);
    cylinderProjAct->setStatusTip(tr("Project cylinder on model"));
    connect(cylinderProjAct, SIGNAL(triggered()), this, SLOT(cylinderProj()));

    linearRegAct = new QAction(tr("&Linear regression..."), this);
    linearRegAct->setStatusTip(tr("Linear regression of the drawed points"));
    connect(linearRegAct, SIGNAL(triggered()), this, SLOT(linearReg()));



    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newProjectAct);
    fileMenu->addAction(openProjectAct);
    fileMenu->addAction(saveProjectAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openModelAct);
    fileMenu->addAction(importTopologyAct);
    fileMenu->addAction(exportTopologyAct);
    //fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);


    toolMenu = menuBar()->addMenu(tr("&Tool"));
    toolMenu->addAction(cylinderProjAct);
    toolMenu->addAction(linearRegAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}

void MainWindow::createSimpleToolbar()
{

}

QWidget *MainWindow::createLeftActionToolbars()
{
    QTabWidget *toolbarTab = new QTabWidget;
    toolbarTab->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    toolbarTab->setTabPosition(QTabWidget::West);
    toolbarTab->addTab(createSimpleActionToolbar(), tr("Simple"));
    toolbarTab->addTab(createAdvencedActionToolbar(), tr("Advanced"));
    return toolbarTab;
}

QWidget * MainWindow::createCommonActionToolbar()
{
    QFrame* layoutWidget = new QFrame;
    layoutWidget->setFrameShape(QFrame::Shape::StyledPanel);
    QVBoxLayout *layoutinner = new QVBoxLayout;
    layoutinner->setMargin(1);

    //Point selection
    selectPointHelperButton = new QToolButton;
    selectPointHelperButton->setCheckable(true);
    selectPointHelperButton->setIcon(QIcon(":/singlePointSel"));
    selectPointHelperButton->setToolTip(tr("Select point"));
    selectPointHelperMenu = new QMenu(selectPointHelperButton);
    QVBoxLayout *selectPointHelperLayout = new QVBoxLayout(selectPointHelperMenu);
    selectPointHelperLayout->setMargin(2);

    selectPointButton = new QToolButton;
    selectPointButton->setCheckable(true);
    selectPointButton->setIcon(QIcon(":/singlePointSel"));
    selectPointButton->setToolTip(tr("Select point"));
    selectPointHelperLayout->addWidget(selectPointButton);
    connect(selectPointButton, SIGNAL(clicked()), this, SLOT(selectPointButtonToggled()));

    selectPointIslandButton = new QToolButton;
    selectPointIslandButton->setCheckable(true);
    selectPointIslandButton->setIcon(QIcon(":/multiPointSel"));
    selectPointIslandButton->setToolTip(tr("Select point island"));
    selectPointHelperLayout->addWidget(selectPointIslandButton);
    connect(selectPointIslandButton, SIGNAL(clicked()), this, SLOT(selectPointIslandButtonToggled()));

    selectPointHelperButton->setMenu(selectPointHelperMenu);
    selectPointHelperButton->setPopupMode(QToolButton::InstantPopup);


    //Edge selection
    selectEdgeHelperButton = new QToolButton;
    selectEdgeHelperButton->setCheckable(true);
    selectEdgeHelperButton->setIcon(QIcon(":/singleEdgeSel"));
    selectEdgeHelperButton->setToolTip(tr("Select edge"));
    selectEdgeHelperMenu = new QMenu(selectEdgeHelperButton);
    QVBoxLayout *selectEdgeHelperLayout = new QVBoxLayout(selectEdgeHelperMenu);
    selectEdgeHelperLayout->setMargin(2);

    selectEdgeButton = new QToolButton;
    selectEdgeButton->setCheckable(true);
    selectEdgeButton->setIcon(QIcon(":/singleEdgeSel"));
    selectEdgeButton->setToolTip(tr("Select edge"));
    selectEdgeHelperLayout->addWidget(selectEdgeButton);
    connect(selectEdgeButton, SIGNAL(clicked()), this, SLOT(selectEdgeButtonToggled()));

    selectEdgeBorderButton = new QToolButton;
    selectEdgeBorderButton->setCheckable(true);
    selectEdgeBorderButton->setIcon(QIcon(":/multiEdgeSel"));
    selectEdgeBorderButton->setToolTip(tr("Select border edges"));
    selectEdgeHelperLayout->addWidget(selectEdgeBorderButton);
    connect(selectEdgeBorderButton, SIGNAL(clicked()), this, SLOT(selectEdgeBorderButtonToggled()));

    selectEdgeHelperButton->setMenu(selectEdgeHelperMenu);
    selectEdgeHelperButton->setPopupMode(QToolButton::InstantPopup);


    //Triangle selection
    selectTriangleButton = new QToolButton;
    selectTriangleButton->setCheckable(true);
    selectTriangleButton->setIcon(QIcon(":/triangleSel"));
    selectTriangleButton->setToolTip(tr("Select triangle"));
    connect(selectTriangleButton, SIGNAL(clicked()), this, SLOT(selectTriangleButtonToggled()));

    //Quad selection
    selectQuadButton = new QToolButton;
    selectQuadButton->setCheckable(true);
    selectQuadButton->setIcon(QIcon(":/quadSel"));
    selectQuadButton->setToolTip(tr("Select quad"));
    connect(selectQuadButton, SIGNAL(clicked()), this, SLOT(selectQuadButtonToggled()));

    //Select arrow
    selectButton = new QToolButton;
    selectButton->setCheckable(true);
    selectButton->setIcon(QIcon(":/selectArrow"));
    selectButton->setToolTip(tr("Selection mode"));
    connect(selectButton, SIGNAL(clicked()), this, SLOT(selectButtonToggled()));

    // Manipulators:
    // Translate
    translateButton = new QToolButton;
    translateButton->setCheckable(true);
    translateButton->setIcon(QIcon(":/manipulatorTranslate"));
    translateButton->setToolTip(tr("Translate"));
    connect(translateButton, SIGNAL(clicked()), this, SLOT(translateButtonToggled()));

    // Rotate
    rotateButton = new QToolButton;
    rotateButton->setCheckable(true);
    rotateButton->setIcon(QIcon(":/manipulatorRotate"));
    rotateButton->setToolTip(tr("Rotate"));
    connect(rotateButton, SIGNAL(clicked()), this, SLOT(rotateButtonToggled()));

    // Scale
    scaleButton = new QToolButton;
    scaleButton->setCheckable(true);
    scaleButton->setIcon(QIcon(":/manipulatorScale"));
    scaleButton->setToolTip(tr("Scale"));
    connect(scaleButton, SIGNAL(clicked()), this, SLOT(scaleButtonToggled()));


    QFrame* separator = new QFrame;
    separator->setFrameShape(QFrame::Shape::VLine);

    layoutinner->addWidget(selectPointHelperButton);
    layoutinner->addWidget(selectEdgeHelperButton);
    layoutinner->addWidget(selectTriangleButton);
    layoutinner->addWidget(selectQuadButton);
    layoutinner->addWidget(separator);
    layoutinner->addWidget(selectButton);
    layoutinner->addWidget(translateButton);
    layoutinner->addWidget(rotateButton);
    layoutinner->addWidget(scaleButton);
    layoutinner->setStretchFactor(scaleButton,1);
    layoutinner->setDirection(QBoxLayout::LeftToRight);
    layoutinner->setAlignment(selectPointHelperButton, Qt::AlignLeft);
    layoutinner->setAlignment(selectEdgeHelperButton, Qt::AlignLeft);
    layoutinner->setAlignment(selectTriangleButton, Qt::AlignLeft);
    layoutinner->setAlignment(selectQuadButton, Qt::AlignLeft);
    layoutinner->setAlignment(selectButton, Qt::AlignLeft);
    layoutinner->setAlignment(translateButton, Qt::AlignLeft);
    layoutinner->setAlignment(rotateButton, Qt::AlignLeft);
    layoutinner->setAlignment(scaleButton, Qt::AlignLeft);

    layoutWidget->setLayout(layoutinner);
    layoutWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    return layoutWidget;
}

QWidget *MainWindow::createSimpleActionToolbar()
{
    QWidget* layoutWidget = new QWidget;
    QVBoxLayout *layoutinner = new QVBoxLayout;




    //Point add
    addPointHelperButton = new QToolButton;
    addPointHelperButton->setCheckable(true);
    addPointHelperButton->setIcon(QIcon(":/pointAdd"));
    addPointHelperButton->setToolTip(tr("Add point"));
    addPointHelperMenu = new QMenu(addPointHelperButton);
    QHBoxLayout *addPointHelperLayout = new QHBoxLayout(addPointHelperMenu);
    addPointHelperLayout->setMargin(2);

    addPointButton = new QToolButton;
    addPointButton->setCheckable(true);
    addPointButton->setIcon(QIcon(":/pointAdd"));
    addPointButton->setToolTip(tr("Add point"));
    addPointHelperLayout->addWidget(addPointButton);
    connect(addPointButton, SIGNAL(clicked()), this, SLOT(addPointButtonToggled()));

    addPointAutoButton = new QToolButton;
    addPointAutoButton->setCheckable(true);
    addPointAutoButton->setIcon(QIcon(":/pointAddAuto"));
    addPointAutoButton->setToolTip(tr("Add point chain"));
    addPointHelperLayout->addWidget(addPointAutoButton);
    connect(addPointAutoButton, SIGNAL(clicked()), this, SLOT(addPointAutoButtonToggled()));

    addPointHelperButton->setMenu(addPointHelperMenu);
    addPointHelperButton->setPopupMode(QToolButton::InstantPopup);


    //Edge add
    addEdgeButton = new QToolButton;
    addEdgeButton->setCheckable(true);
    addEdgeButton->setIcon(QIcon(":/edgeAdd"));
    addEdgeButton->setToolTip(tr("Add edge"));
    connect(addEdgeButton, SIGNAL(clicked()), this, SLOT(addEdgeButtonToggled()));

    //Edge extrude
    extrudeEdgeButton = new QToolButton;
    extrudeEdgeButton->setCheckable(true);
    extrudeEdgeButton->setIcon(QIcon(":/edgeExtrude"));
    extrudeEdgeButton->setToolTip(tr("Extrude edge"));
    connect(extrudeEdgeButton, SIGNAL(clicked()), this, SLOT(extrudeEdgeButtonToggled()));

    //Edge divide
    divideEdgeButton = new QToolButton;
    divideEdgeButton->setCheckable(true);
    divideEdgeButton->setIcon(QIcon(":/edgeDivide"));
    divideEdgeButton->setToolTip(tr("Divide edge"));
    connect(divideEdgeButton, SIGNAL(clicked()), this, SLOT(divideEdgeButtonToggled()));

    //Triangle divide
    divideTriangleButton = new QToolButton;
    divideTriangleButton->setCheckable(true);
    divideTriangleButton->setIcon(QIcon(":/triangleDivide"));
    divideTriangleButton->setToolTip(tr("Divide triangle"));
    connect(divideTriangleButton, SIGNAL(clicked()), this, SLOT(divideTriangleButtonToggled()));


/*

    qtoolButton1 = new QToolButton;
    qtoolButton1->setCheckable(true);
    qtoolButton1->setIcon(QIcon(":/butIcon"));
    but1menu = new QMenu(qtoolButton1);
    QHBoxLayout *layoutbut1 = new QHBoxLayout(but1menu);
    QToolButton* innerBut = new QToolButton;
    layoutbut1->addWidget(innerBut);

    //QWidgetAction* toolAction = new QWidgetAction(but1menu);
    //but1menu->addAction(toolAction);
    qtoolButton1->setMenu(but1menu);
    qtoolButton1->setPopupMode(QToolButton::InstantPopup);

    qtoolButton2 = new QToolButton;
    qtoolButton2->setCheckable(true);
    qtoolButton2->setIcon(QIcon(":/butIcon"));
    qtoolButton2->setToolTip(tr("Button2"));
    //connect(qtoolButton1, SIGNAL(clicked()), this, SLOT(button1Toggled()));
    connect(innerBut, SIGNAL(clicked()), this, SLOT(button1Toggled()));
    connect(qtoolButton2, SIGNAL(clicked()), this, SLOT(button2Toggled()));
*/



    layoutinner->addWidget(addPointHelperButton);
    layoutinner->addWidget(addEdgeButton);
    layoutinner->addWidget(extrudeEdgeButton);
    layoutinner->addWidget(divideEdgeButton);
    layoutinner->addWidget(divideTriangleButton);
    layoutinner->setStretchFactor(divideTriangleButton,1);
    layoutinner->setDirection(QBoxLayout::TopToBottom);
    layoutinner->setAlignment(addPointHelperButton, Qt::AlignTop);
    layoutinner->setAlignment(addEdgeButton, Qt::AlignTop);
    layoutinner->setAlignment(extrudeEdgeButton, Qt::AlignTop);
    layoutinner->setAlignment(divideTriangleButton, Qt::AlignTop);
    layoutWidget->setLayout(layoutinner);

    return layoutWidget;
}

QWidget *MainWindow::createAdvencedActionToolbar()
{
    QWidget* layoutWidget = new QWidget;
    QVBoxLayout *layoutinner = new QVBoxLayout;

    //Edge divide
    cylinderProjButton = new QToolButton;
    cylinderProjButton->setCheckable(true);
    cylinderProjButton->setIcon(QIcon(":/cylinderProj"));
    cylinderProjButton->setToolTip(tr("Cylinder projection"));
    connect(cylinderProjButton, SIGNAL(clicked()), this, SLOT(cylinderProjButtonToggled()));

    //Triangle divide
    similarProjButton = new QToolButton;
    similarProjButton->setIcon(QIcon(":/similarProj"));
    similarProjButton->setToolTip(tr("Surface projection"));
    connect(similarProjButton, SIGNAL(clicked()), this, SLOT(similarProjButtonToggled()));

    //Triangle divide
    constraintProjButton = new QToolButton;
    constraintProjButton->setCheckable(true);
    constraintProjButton->setIcon(QIcon(":/edgeExtrude"));
    constraintProjButton->setToolTip(tr("Constraint projection"));
    connect(constraintProjButton, SIGNAL(clicked()), this, SLOT(constraintProjButtonToggled()));

    layoutinner->addWidget(cylinderProjButton);
    layoutinner->addWidget(similarProjButton);
    layoutinner->addWidget(constraintProjButton);
    layoutinner->setStretchFactor(constraintProjButton,1);
    layoutinner->setDirection(QBoxLayout::TopToBottom);
    layoutinner->setAlignment(cylinderProjButton, Qt::AlignTop);
    layoutinner->setAlignment(similarProjButton, Qt::AlignTop);
    layoutinner->setAlignment(constraintProjButton, Qt::AlignTop);
    layoutWidget->setLayout(layoutinner);

    return layoutWidget;
}

void MainWindow::toggleOffAllTools()
{
    toggleOffCommonTools();
    toggleOffSimpleTools();
    toggleOffAdvancedTools();
}

void MainWindow::toggleOffCommonTools()
{
    selectButton->setChecked(false);
    toggleOffSelectionTools();
    toggleOffManipulatorTools();
}

void MainWindow::toggleOffSelectionTools()
{
    selectPointButton->setChecked(false);
    selectPointIslandButton->setChecked(false);
    selectEdgeButton->setChecked(false);
    selectEdgeBorderButton->setChecked(false);
    selectTriangleButton->setChecked(false);
    selectQuadButton->setChecked(false);
    selectPointHelperButton->setChecked(false);
    selectEdgeHelperButton->setChecked(false);
}

void MainWindow::toggleOffManipulatorTools()
{
    translateButton->setChecked(false);
    rotateButton->setChecked(false);
    scaleButton->setChecked(false);
}

void MainWindow::toggleOffSimpleTools()
{
    addPointButton->setChecked(false);
    addPointAutoButton->setChecked(false);
    addEdgeButton->setChecked(false);
    extrudeEdgeButton->setChecked(false);
    divideEdgeButton->setChecked(false);
    divideTriangleButton->setChecked(false);
    addPointHelperButton->setChecked(false);
}

void MainWindow::toggleOffAdvancedTools()
{
    cylinderProjButton->setChecked(false);
    similarProjButton->setChecked(false);
    constraintProjButton->setChecked(false);
}

void MainWindow::toggleOffNotCommonTools()
{
    toggleOffSimpleTools();
    toggleOffAdvancedTools();
}
