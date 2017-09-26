#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDebug>
//#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QString>
#include "loginform.h"
#include "readerform.h"
#include "librarianform.h"
#include "usermanagerform.h"
#include "userform.h"
#include "bookform.h"
#include "notiform.h"
#include "returnbookform.h"

class MainWindow
{

public:
    explicit MainWindow();
    ~MainWindow();

private:
    QSqlDatabase libProDB;  //Database

};

#endif // MAINWINDOW_H
