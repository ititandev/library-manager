#ifndef RETURNBOOKFORM_H
#define RETURNBOOKFORM_H

#include <QWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <librarianform.h>
#include <macro.h>


namespace Ui {
class ReturnBookForm;
}

class ReturnBookForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReturnBookForm(QWidget *parent = 0);
    ~ReturnBookForm();
    void start(QString input);  //khởi chạy ReturnBookForm với đầu vào là mã sách

private slots:
    void on_exitButton_clicked();

    void on_pushButton_clicked();

    void on_quaHanCheckBox_toggled(bool checked);

    void on_huHongCheckBox_toggled(bool checked);

    void on_matSachCheckBox_toggled(bool checked);

    void on_quaHanSpin_valueChanged(int arg1);

    void on_huHongSpin_valueChanged(int arg1);

    void on_matSachSpin_valueChanged(int arg1);

    void on_quaHanSpin2_valueChanged(int arg1);

    void on_huHongSpin2_valueChanged(int arg1);

    void on_matSachSpin_2_valueChanged(int arg1);

private:
    Ui::ReturnBookForm *ui;
    QSqlQuery query;        //truy xuất database
    QString maSo;           //lưu mã số sách được trả
    int phatValue;          //lưu giá trị tiền phạt sau khi tính
    QString chuThich;       //lưu chú thích cho lần trả sách

    void getPunishValue();  //tính giá trị tiền phạt
};

#endif // RETURNBOOKFORM_H
