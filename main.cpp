#include "notepad.h"
#include <QApplication>
#include <iostream>
#include <QString>
#include <QDebug>
#include <string>
#include "word.h"
using namespace std;

QList <Word> createlist();

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setApplicationName("Steven Text Editor");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("University of Belize, CMPS2212: gui programming");
    a.setOrganizationDomain("ub.edu.bz");
    Notepad w;
    w.show();

    return a.exec();//event loop

}
