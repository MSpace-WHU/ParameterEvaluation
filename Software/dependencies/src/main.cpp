#include "../include/ParameterEvaluation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParameterEvaluation w;
    w.show();
    return a.exec();
}
