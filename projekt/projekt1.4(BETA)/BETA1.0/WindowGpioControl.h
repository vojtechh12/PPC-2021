#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include "GPIO.h"
#include "ToggleButton.h"

class WindowGpioControl : public QWidget
{
    Q_OBJECT

public:
    WindowGpioControl(QWidget* parent = nullptr) : QWidget(parent) {
        QGroupBox* gpiocWinGroupBox = new QGroupBox(tr("GPIO control mode:"));
        QGridLayout* glayout = new QGridLayout(gpiocWinGroupBox);

        // creating toggle buttons one for each gpio pin TODO the rest, this is BETA
        ToggleButton* b1 = new ToggleButton("5V"); b1->setStyleSheet("background-color: red; color: black;");
        ToggleButton* b2 = new ToggleButton("3V3"); b2->setStyleSheet("background-color: orange; color: black;");
        ToggleButton* b3 = new ToggleButton("5V"); b3->setStyleSheet("background-color: red; color: black;");
        ToggleButton* b4 = new ToggleButton("gpio2");
        ToggleButton* b5 = new ToggleButton("GND"); b5->setStyleSheet("background-color: black; color: white;");
        ToggleButton* b6 = new ToggleButton("gpio3");
        ToggleButton* b7 = new ToggleButton("gpio14");
        ToggleButton* b8 = new ToggleButton("gpio4");

        // arranging the buttons into a desired layout
        glayout->addWidget(b1, 0,0);
        glayout->addWidget(b2, 1,0);
        glayout->addWidget(b3, 0,1);
        glayout->addWidget(b4, 1,1);
        glayout->addWidget(b5, 0,2);
        glayout->addWidget(b6, 1,2);
        glayout->addWidget(b7, 0,3);
        glayout->addWidget(b8, 1,3);

        // connecting the toggled signal from the buttons to appropriate slot
        QObject::connect(b4, &QAbstractButton::toggled, b4, &ToggleButton::did_toggle);
        QObject::connect(b6, &QAbstractButton::toggled, b6, &ToggleButton::did_toggle);
        QObject::connect(b7, &QAbstractButton::toggled, b7, &ToggleButton::did_toggle);
        QObject::connect(b8, &QAbstractButton::toggled, b8, &ToggleButton::did_toggle);

        // setting up the layout
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(gpiocWinGroupBox);
        setLayout(layout);
    }
};