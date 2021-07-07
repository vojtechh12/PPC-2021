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
#include <QChar>
#include <map>
#include <QDebug>
#include "GPIO.h"
#include "WindowMorse.h"
#include "WindowGpioControl.h"

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
    
    WindowGpioControl* gpiocontrol = new WindowGpioControl();

    WindowMorse* morsecode = new WindowMorse();

    // adding both Windowwidgets gpiocontrol and morsecode into stackedwidget
    QStackedWidget* stackedw = new QStackedWidget;
    stackedw->addWidget(gpiocontrol);
    stackedw->addWidget(morsecode);
    vbox->addWidget(stackedw);
    centralw->setLayout(vbox);
    mw.setCentralWidget(centralw);

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