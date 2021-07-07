#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include "GPIO.h"

class WindowMorse : public QWidget
{
    Q_OBJECT

public:
    WindowMorse(QWidget* parent = 0) : QWidget(parent) {
        // constructs the whole window for morsecode mode
        QGroupBox* morseWinGroupbox = new QGroupBox(tr("morse code mode:"));
        QLabel* morseWinLabel = new QLabel(tr("Select GPIO pin:"));
        QComboBox* morseWinComboBox = new QComboBox;

        // TODO add all the GPIO pins
        morseWinComboBox->addItem(tr("gpio4"));
        morseWinComboBox->addItem(tr("gpio2"));
        morseWinComboBox->addItem(tr("gpio3"));
        morseWinComboBox->addItem(tr("gpio14"));
        morseWinLineEdit = new QLineEdit;
        morseWinLineEdit->setPlaceholderText("Write the text and hit enter:");

        // arrange widgets into QGridLayout and set it as layout for QGroupBox
        QGridLayout* morseWinGridLayout = new QGridLayout;
        morseWinGridLayout->addWidget(morseWinLabel, 0,0);
        morseWinGridLayout->addWidget(morseWinComboBox, 0,1);
        morseWinGridLayout->addWidget(morseWinLineEdit, 1,0,1, 2);
        morseWinGroupbox->setLayout(morseWinGridLayout);

        // connect the change in QComboBox to approp slots
        connect(morseWinComboBox, QOverload<int>::of(&QComboBox::activated), this, &WindowMorse::GPIOchanged);

        // connect the returnpressed signal from lineedit to to slot in this window
        connect (morseWinLineEdit, &QLineEdit::returnPressed, this, &WindowMorse::readTextEntered);
        
        /* OPTIONAL- using lambda fct

        connect(morseWinLineEdit, &QLineEdit::returnPressed, this, [=] () {
            mapOfGpio.at(using_gpio).export_gpio();
            mapOfGpio.at(using_gpio).setdir_gpio("out");
            for (int  i = 0; i < morseWinLineEdit->text().size(); i++) {
                mapOfGpio.at(using_gpio).morsechar(morseWinLineEdit->text().at(i));
            }
            mapOfGpio.at(using_gpio).unexport_gpio();
        });
        */


        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(morseWinGroupbox);
        setLayout(layout);
    }

private:
    GPIOClass gpio2 = GPIOClass("2");
    GPIOClass gpio3 = GPIOClass("3");
    GPIOClass gpio4 = GPIOClass("4");
    GPIOClass gpio14 = GPIOClass("14");
    std::map<QString, GPIOClass> mapOfGpio { {"gpio2", gpio2}, {"gpio3", gpio3}, {"gpio4", gpio4}, {"gpio14", gpio14} };
    QString using_gpio = QString("gpio4"); //sets the default
    QLineEdit* morseWinLineEdit;

public slots:
    void GPIOchanged(int index) {
        switch (index)
        {
        case 0:
            using_gpio = QString("gpio4");
            break;
        case 1:
            using_gpio = QString("gpio2");
            break;
        case 2:
            using_gpio = QString("gpio3");
            break;
        case 3: using_gpio = QString("gpio14");
        break;
        
        default:
            break;
        }
    }
    void readTextEntered() {  
        mapOfGpio.at(using_gpio).export_gpio();
        mapOfGpio.at(using_gpio).setdir_gpio("out");
        for (int  i = 0; i < morseWinLineEdit->text().size(); i++) {
            mapOfGpio.at(using_gpio).morsechar(morseWinLineEdit->text().at(i));
        }
        mapOfGpio.at(using_gpio).unexport_gpio();
    }
    

};