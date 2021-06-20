#include "GPIO.h"
#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include "tlacitko.h"

int main (int argc, char * argv[]) {
    // creates instance of gpio class and sets gpio4 as out and volt as HIGH
    // is on until user finnishes the program

    QApplication app (argc, argv);
    QWidget w1;

    GPIOClass gpio4 = GPIOClass("4");
    gpio4.export_gpio();
    gpio4.setdir_gpio("out");
    gpio4.setval_gpio("1");

    /*
    std::cout << "Press 'f' key and hit enter to finnish the program: ";
    char finnish;
    std::cin >> finnish;

    while (1) {
        if (finnish == 'f') break;
        else
            std::cout << "\n If you wish to finnish the programm, press 'f' and hit enter...";
        std::cin >> finnish;
    }

    gpio4.unexport_gpio();
    */
    Tlacitko t1;
    QObject::connect(&t1, &Tlacitko::clicked, [&] (){gpio4.unexport_gpio();});

    QVBoxLayout vbox;
    vbox.addWidget(&t1);
    w1.setLayout(&vbox);
    w1.show();

    return app.exec();
}