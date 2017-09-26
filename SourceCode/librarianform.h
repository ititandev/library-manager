#ifndef LIBRARIANFORM_H
#define LIBRARIANFORM_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QSystemTrayIcon>
#include <loginform.h>
#include <readerform.h>
#include <usermanagerform.h>
#include <returnbookform.h>

namespace Ui {
class LibrarianForm;
}

class LibrarianForm : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarianForm(QWidget *parent = 0);
    ~LibrarianForm();
    bool active;            //trạng thái kích hoạt cho phép form hoạt động

    void setRole(int role); //thiết lập các vai trò có thể chuyển đổi
    void start();           //Khởi chạy LibrarianForm
    void queryBook();       //truy xuất bảng Sách
    void queryRequest();    //truy xuất bảng Yêu cầu
    void queryNoti();       //truy xuất bảng Thông báo

private slots:
    void on_roleComboBox_currentTextChanged(const QString &arg1);

    void on_logoutButton_clicked();

    void on_deleteBookButton_clicked();

    void on_searchButton_clicked();

    void on_viewAllButton_clicked();

    void on_sortComboBox_currentIndexChanged(int index);

    void on_sortModeComboBox_currentIndexChanged(int index);

    void on_viewMoreButton_clicked();

    void on_addBookButton_clicked();

    void on_user_clicked();

    void on_typeComboBox_currentIndexChanged(int index);

    void on_searchComboBox_currentIndexChanged(int index);

    void on_viewAllRequestButton_clicked();

    void on_searchRequestButton_clicked();

    void on_searchRequestComboBox_currentIndexChanged(int index);

    void on_typeRequestComboBox_currentIndexChanged(int index);

    void on_button_clicked();

    void on_deleteButton_clicked();

    void on_deleteNotiButton_clicked();

    void on_sendNotiButton_clicked();

    void on_editNotiButton_clicked();

    void on_notiTableView_pressed(const QModelIndex &index);

    void on_updateSettingButton_clicked();

    void on_typeNotiComboBox_currentTextChanged(const QString &arg1);

    void on_searchNotiButton_clicked();

    void on_extendButton_clicked();

private:
    Ui::LibrarianForm *ui;
    QSqlQuery query;                            //truy xuất database
    QSqlQueryModel queryModel;                  //Hiện thị bảng tìm kiếm sách
    QSqlQueryModel queryModelRequest;           //Hiện thị bảng yêu cầu mượn sách
    QSqlQueryModel queryModelNoti;              //Hiện thị bảng thông báo

    void querySetting();                        //truy xuất Cài đặt
    QString selectColumn(int choice);           //chuyển về cột trong bảng Book
    QString selectColumnRequest(int choice);    //chuyển về cột trong bảng Request
    QString getRecord(int i);                   //Lấy dữ liệu cột thứ i trong bảng Request Table
    QString getFromUser();                      //Lấy dữ liệu người gửi
    QString getMaThongBao();                    //Lấy dữ liệu mã thông báo
    bool checkSelectedNoti();                   //Kiểm tra có thông báo được chọn
    bool checkSelectedBook();                   //Kiểm tra có sách được chọn
    bool checkSelectedRequest();                //Kiểm tra có yêu cầu được chọn

};

#endif // LIBRARIANFORM_H
