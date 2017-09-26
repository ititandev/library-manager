#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);     //Tạo chưa trình Qt
    MainWindow w;       //Tạo đối tượng class MainWindow
    a.setWindowIcon(QIcon("./images/LibPro.ico"));      //Set icon cho phần mềm

    return a.exec();
}
