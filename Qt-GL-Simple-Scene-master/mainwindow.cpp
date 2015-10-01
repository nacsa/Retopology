#include "mainwindow.h"
#include "mainview.h"

#include <QtWidgets>

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
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    //layout->addWidget(slider);//topFiller

    toolBar = new QToolBar(tr("ToolBar"),this);
    toolBar->setFloatable(true);
    toolBar->setMovable(true);
    toolBar->setAllowedAreas(Qt::AllToolBarAreas);
    toolBar->addAction("Tik1");
    toolBar->addAction("Tik2");
    toolBar->addSeparator();
    toolBar->addAction("Tik3");

    this->addToolBar(toolBar);

    layout->addWidget(glView);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);


    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    //toolBar->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    toolBar->show();

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
