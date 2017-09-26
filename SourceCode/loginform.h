#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <userform.h>
#include <QModelIndex>
#include <QCryptographicHash>
#include <QString>
#include <bookform.h>
#include <QString>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QModelIndexList>
#include <QDebug>
#include <QKeyEvent>
#include <readerform.h>
#include <librarianform.h>
#include <usermanagerform.h>
#include <macro.h>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();
    void start();
    static void writeHistory(QString bodyAction)
    {
        QSqlQuery query;
        extern QString userName;
        query.prepare("INSERT INTO history SELECT MAX(maSo) + 1, :dateTime, :userName, :bodyAction FROM history");
        query.bindValue(":dateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        query.bindValue(":userName", userName);
        query.bindValue(":bodyAction", bodyAction);
        query.exec();
    }

private slots:
    void on_loginButton_clicked();

    void on_searchButton_clicked();

    void on_viewAllButton_clicked();

    void on_signUpButton_clicked();

    void on_sortComboBox_currentIndexChanged(int index);

    void on_sortModeComboBox_currentIndexChanged(int index);

    void on_noiQuyButton_clicked();

    void on_viewMoreButton_clicked();

    void on_typeComboBox_currentIndexChanged(int index);

    void on_searchComboBox_currentIndexChanged(int index);

    void on_bookTableView_pressed(const QModelIndex &index);

    void on_gopYButton_clicked();

    void on_muaTaiLieuButton_clicked();

private:
    Ui::LoginForm *ui;
    QSqlQuery query;                        //truy xuất database
    QSqlQueryModel queryModel;              //Hiện thị dữ liệu bảng tìm kiếm sách

    void preQuery();                        //Truy xuất đầu chương trình (xóa chờ mượn quá lâu hoặc khóa tài khoản quá hạn)
    QString selectColumn(int choice);       //chuyển về cột trong bảng Book
    void queryBook();                       //truy xuất sách
    void keyPressEvent(QKeyEvent *event);   //sự kiện nhấn phím
};

#endif // LOGINFORM_H
