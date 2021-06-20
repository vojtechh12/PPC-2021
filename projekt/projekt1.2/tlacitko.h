#include <QPushButton>
#include <QDebug>
#include "GPIO.h"


class Tlacitko : public QPushButton
{
    Q_OBJECT
    QList<QString> barvy;
    

    int i;

public:
    Tlacitko(QWidget * parent = 0) : QPushButton (parent) {}

public slots: 
    void LEDoff() {
        
    }

};