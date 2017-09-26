#ifndef USERFORM_H
#define USERFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDebug>
#include <QInputDialog>
#include <macro.h>
#include <usermanagerform.h>

namespace Ui {
class UserForm;
}

class UserForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserForm(QWidget *parent = 0);
    ~UserForm();
    void start(int mode, QString userNameInp = QString());  //Khởi chạy UserForm với các chế độ được định nghĩa trong file macro.h và đầu vào tương ứng

private slots:
    void on_Button_clicked();

    void on_khoa_currentIndexChanged(int index);

    void on_passWordButton_clicked();

private:
    Ui::UserForm *ui;
    int currentMode;        //lưu chế độ hiện tại
    QSqlQuery query;        //truy xuất database
    QString userNameInput;  //lưu tên tài khoản đầu vào
    QString passWord;       //lưu mật khẩu khi đổi hoặc tạo

    bool checkUnique(QString column, QString data); //Kiểm tra tính duy nhất của các cột như userName, CMND, MSSV, email, sđt

};

#endif // USERFORM_H
