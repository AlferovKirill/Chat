#include "../headers/chat.h"
#include <QApplication>


int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    Chat w;
    w.show();

    return a.exec();
}
