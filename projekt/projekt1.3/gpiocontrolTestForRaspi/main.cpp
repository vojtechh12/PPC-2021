#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QTranslator>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <map>
#include "ToggleButton.h"
#include "GPIO.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    QMainWindow mw;
    QVBoxLayout* vbox = new QVBoxLayout;

    QMenuBar* bar = mw.menuBar();
    QMenu* menu = bar->addMenu("&mode");
    
    QAction* gpiocAction = new QAction("gpio-control");
    QAction* morsecAction = new QAction("morsecode");
    menu->addAction(gpiocAction);
    menu->addSeparator();
    menu->addAction(morsecAction);

    QWidget* centralw = new QWidget;
    QWidget* gpiocontrol = new QWidget;



    // initialising QWidget gpiocontrol as gridlayout of togglebuttons
    {
        ToggleButton* b1 = new ToggleButton("5V"); b1->setStyleSheet("background-color: red; color: black;");
        ToggleButton* b2 = new ToggleButton("3V3"); b2->setStyleSheet("background-color: orange; color: black;");
        ToggleButton* b3 = new ToggleButton("5V"); b3->setStyleSheet("background-color: red; color: black;");
        ToggleButton* b4 = new ToggleButton("gpio2");
        ToggleButton* b5 = new ToggleButton("GND"); b5->setStyleSheet("background-color: black; color: white;");
        ToggleButton* b6 = new ToggleButton("gpio3");
        ToggleButton* b7 = new ToggleButton("gpio14");
        ToggleButton* b8 = new ToggleButton("gpio4");


        QGridLayout* glayout = new QGridLayout(gpiocontrol);
        glayout->addWidget(b1, 0,0);
        glayout->addWidget(b2, 1,0);
        glayout->addWidget(b3, 0,1);
        glayout->addWidget(b4, 1,1);
        glayout->addWidget(b5, 0,2);
        glayout->addWidget(b6, 1,2);
        glayout->addWidget(b7, 0,3);
        glayout->addWidget(b8, 1,3);

        QObject::connect(b4, &QAbstractButton::toggled, b4, &ToggleButton::did_toggle);
        QObject::connect(b6, &QAbstractButton::toggled, b6, &ToggleButton::did_toggle);
        QObject::connect(b7, &QAbstractButton::toggled, b7, &ToggleButton::did_toggle);
        QObject::connect(b8, &QAbstractButton::toggled, b8, &ToggleButton::did_toggle);

    }

    QWidget* morsecode = new QWidget;
    // initialising Qwidget for morsecode mode
    {
        QLineEdit* lineedit = new QLineEdit("enter text for encoding");
        QVBoxLayout* vblayout = new QVBoxLayout(morsecode);
        vblayout->addWidget(lineedit);
    }

    // adding both mainwidgets gpiocontrol and morsecode into stackedwidget
    QStackedWidget* stackedw = new QStackedWidget;
    stackedw->addWidget(gpiocontrol);
    stackedw->addWidget(morsecode);
    vbox->addWidget(stackedw);
    centralw->setLayout(vbox);
    mw.setCentralWidget(centralw);

    // zatim navic TODO
    QStatusBar* status = mw.statusBar();
    status->showMessage("gpio-control");

    // connecting menu actions with changing of the current visible widget
    QObject::connect(morsecAction, &QAction::triggered, [&] () {
        stackedw->setCurrentWidget(morsecode);
    });

    QObject::connect(gpiocAction, &QAction::triggered, [&] () {
        stackedw->setCurrentWidget(gpiocontrol);
    });

    mw.show();

    return app.exec();
}