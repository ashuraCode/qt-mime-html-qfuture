#include <QApplication>
#include <QtCore>
#include "ashurawind.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AshuraWind w;
    w.show();

    return a.exec();
}
