#ifndef BOOKFORM_H
#define BOOKFORM_H

#include <QWidget>
#include <QInputDialog>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>
#include <QDate>
#include <librarianform.h>
#include <macro.h>

namespace Ui {
class BookForm;
}

class BookForm : public QWidget
{
    Q_OBJECT

public:
    explicit BookForm(QWidget *parent = 0);
    ~BookForm();
    void start(int mode, QString maSachInput); //Hàm khởi chạy Book Form với các chế độ được định nghĩa trong macro.h

private slots:
    void on_button_clicked(); //sự kiện button được click.

private:
    Ui::BookForm *ui;
    int currentMode;    //lưu lại chế độ hiện tại
    QSqlQuery query;    //truy xuất database
    QString maSach;     //lưu lại mã sách
};

#endif // BOOKFORM_H
