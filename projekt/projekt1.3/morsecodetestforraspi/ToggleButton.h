#include <QPushButton>
#include <QAbstractButton>
#include "GPIO.h"


class ToggleButton : public QPushButton
{
    Q_OBJECT


public:
    ToggleButton(QWidget * parent = 0) : QPushButton (parent)
    {
        setStyleSheet("background-color: grey;");
    }
    ToggleButton(const QString &text, QWidget *parent = 0) : QPushButton(text, parent) 
    { 
        this->setCheckable(true);
        setStyleSheet("background-color: grey;");
        // this->setStyleSheet("QPushButton{background-color:red;} QPushButton:checked{background-color:green;}" );
    }

public slots: 
    void did_toggle(bool checked) {
        //creating a map container with instances of GpioClass, comes handy in did_toggle slot
        GPIOClass gpio2 = GPIOClass("2");
        GPIOClass gpio3 = GPIOClass("3");
        GPIOClass gpio4 = GPIOClass("4");
        GPIOClass gpio14 = GPIOClass("14");
        std::map<QString, GPIOClass> mapOfGpio { {"gpio2", gpio2}, {"gpio3", gpio3}, {"gpio4", gpio4}, {"gpio14", gpio14} };

        if (checked) { 
            this->setStyleSheet("background-color: green;");
            mapOfGpio.at(this->text()).export_gpio();
            mapOfGpio.at(this->text()).setdir_gpio("out");
            mapOfGpio.at(this->text()).setval_gpio("1");
        }
        else { 
            this->setStyleSheet("background-color: grey;");
            mapOfGpio.at(this->text()).unexport_gpio();
        }
    }

};