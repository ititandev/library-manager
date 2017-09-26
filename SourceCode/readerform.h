#ifndef READERFORM_H
#define READERFORM_H

#include <QWidget>
#include <librarianform.h>
#include <usermanagerform.h>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <bookform.h>
#include <QInputDialog>
#include <macro.h>
#include <QDebug>
#include <QMessageBox>
#include <loginform.h>
#include <QDate>
#include <QModelIndex>
#include <notiform.h>

namespace Ui {
class ReaderForm;
}

class ReaderForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReaderForm(QWidget *parent = 0);
    ~ReaderForm();
    bool active;            //trạng thái kích hoạt cho phép form hoạt động

    void start();           //khởi chạy ReaderForm
    void setRole(int role); //thiết lập các vai trò có thể chuyển đổi
    void queryNoti();       //truy xuất thông báo

private slots:
    void on_roleComboBox_currentTextChanged(const QString &arg1);

    void on_logoutForm_clicked();

    void on_user_clicked();

    void on_searchButton_clicked();

    void on_viewAllButton_clicked();

    void on_sortComboBox_currentIndexChanged(int index);

    void on_sortModeComboBox_currentIndexChanged(int index);

    void on_borrowButton_clicked();

    void on_viewMoreButton_clicked();

    void on_deleteCartButton_clicked();

    void on_requestButton_clicked();

    void on_typeComboBox_currentIndexChanged(int index);

    void on_searchComboBox_currentIndexChanged(int index);

    void on_deleteRequestButton_clicked();

    void on_viewNotiButton_clicked();

    void on_deleteNotiButton_clicked();

    void on_hoTroButton_clicked();

    void on_gopYButton_clicked();

    void on_deNghiMuaSachButton_clicked();

    void on_searchNotiButton_clicked();

private:
    Ui::ReaderForm *ui;
    QSqlQuery query;                    //truy xuất DataBase
    QSqlQueryModel queryModel;          //Hiển thị bảng tìm kiếm sách
    QSqlQueryModel queryModelCart;      //Hiện thị bảng giỏ sách
    QSqlQueryModel queryModelBorrow;    //Hiện thị bảng mượn sách
    QSqlQueryModel queryModelHistory;   //Hiện thị bảng lịch sử
    QSqlQueryModel queryModelNoti;      //Hiện thị bảng thông báo

    void getCart();                     //lấy dữ liệu bảng giỏ sách
    bool isPunished();                  //Kiểm tra bị phạt vì trễ hạn trả sách
    bool checkDeadline();               //Kiểm tra sắp tới hạn trả sách
    void queryBook();                   //truy xuất bảng Sách
    void queryBorrow();                 //truy xuất bảng Mượn sách
    void queryHistory();                //truy xuất bảng Lịch sử

    QString selectColumn(int choice);   //chuyển về tên cột trong bảng Book
    int getRemainBook(QString maSach);  //Lấy dữ liệu số sách còn lại của một sách nào đó
    int getBorrowedNum(QString userName);//Lấy dữ liệu số sách đã mượn của một người dùng
    QString getMaThongBao();            //Lấy mã thông báo được chọn
    QString getToUser();                //Lấy dữ liệu gửi tới của thông báo được chọn
    bool checkSelectedNoti();           //Kiểm tra có thông báo được chọn
};

#endif // READERFORM_H
