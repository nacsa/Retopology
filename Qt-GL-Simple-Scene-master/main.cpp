#include <QApplication>
//#include <QWidget>
//#include <QHBoxLayout>
//#include <QVBoxLayout>
//#include <QPushButton>
//#include "mainview.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // main window
    MainWindow window;
    window.show();
    return app.exec();
/*
    // GL format
    QGLFormat format;
    format.setVersion(4,3);
    format.setProfile(QGLFormat::CoreProfile);
    MainView *glView = new MainView(format);

    QVBoxLayout *mainLayout = new QVBoxLayout(&win);
    mainLayout->addWidget(glView);

    QWidget *cPanel = new QWidget(&win);
    QHBoxLayout *cPanelLayout = new QHBoxLayout();
    cPanel->setLayout(cPanelLayout);

    QPushButton *sShotButton = new QPushButton("Screen Shot");
    cPanelLayout->addWidget(sShotButton);
    QObject::connect( sShotButton, SIGNAL(clicked()), glView, SLOT(takeScreenShot()) );

    QPushButton *switchButton = new QPushButton("Switch Buffers");
    cPanelLayout->addWidget(switchButton);
    QObject::connect(switchButton, SIGNAL(clicked()), glView, SLOT(switchModel()));

    QPushButton *animButton = new QPushButton("Start/Stop Animation");
    cPanelLayout->addWidget(animButton);
    QObject::connect(animButton, SIGNAL(clicked()), glView, SLOT(toggleAnimation()) );

    mainLayout->addWidget(cPanel);

    win.setLayout(mainLayout);
    win.setWindowTitle("Simple Scene");*/
//    win.show();

//    return app.exec();
}

