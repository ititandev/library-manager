#ifndef USERMANAGERFORM_H
#define USERMANAGERFORM_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QDebug>
#include <macro.h>
#include <loginform.h>
#include <readerform.h>
#include <librarianform.h>



namespace Ui {
class UserManagerForm;
}

class UserManagerForm : public QWidget
{
    Q_OBJECT

public:
    explicit UserManagerForm(QWidget *parent = 0);
    ~UserManagerForm();
    bool active;            //trạng thái kích hoạt cho phép form hoạt động

    void setRole(int role); //thiết lập các vai trò có thể chuyển đổi
    void start();           //Khởi chạy UserManagerForm
    void queryUser();       //Truy xuất bảng Người dùng
    void queryNoti();       //Truy xuất bảng Thông báo

private slots:
    void on_roleComboBox_currentTextChanged(const QString &arg1);

    void on_logoutForm_clicked();

    void on_user_clicked();

    void on_createUserButton_clicked();

    void on_editUserButton_clicked();

    void on_deleteUserButton_clicked();

    void on_searchComboBox_currentIndexChanged(int index);

    void on_typeComboBox_currentIndexChanged(int index);

    void on_searchUserButton_clicked();

    void on_viewAllButton_clicked();

    void on_lockButton_clicked();

    void on_notiTableView_pressed(const QModelIndex &index);

    void on_editNotiButton_clicked();

    void on_sendNotiButton_clicked();

    void on_deleteNotiButton_clicked();

    void on_typeNotiComboBox_currentTextChanged(const QString &arg1);

    void on_searchNotiButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::UserManagerForm *ui;
    QSqlQuery query;                    //truy xuất Database
    QSqlQueryModel queryUserModel;      //Hiện thị dữ liệu bảng Người dùng
    QSqlQueryModel queryModelNoti;      //Hiện thị dữ liệu bảng Thông báo
    QSqlQueryModel queryModelHistory;   //Hiện thị dữ liệu bảng Lịch sử

    QString selectColumn(int index);    //Chuyển về cột trong bảng Book
    QString getSelectedUser(int i);     //Lấy dữ liệu cột thứ i của người dùng được chọn
    QString getMaThongBao();            //Lấy mã thông báo của thông báo được chọn
    QString getFromUser();              //Lấy người gửi của thông báo được chọn
    bool checkSelectedUser();           //Kiểm tra có người dùng được chọn
    bool checkSelectedNoti();           //Kiểm tra có thông báo được chọn
    void queryHistory();                //truy xuất bảng
};

#endif // USERMANAGERFORM_H
