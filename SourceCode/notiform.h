#ifndef NOTIFORM_H
#define NOTIFORM_H

#include <QWidget>
#include <macro.h>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <librarianform.h>
#include <usermanagerform.h>

namespace Ui {
class NotiForm;
}

class NotiForm : public QWidget
{
    Q_OBJECT

public:
    explicit NotiForm(QWidget *parent = 0);
    ~NotiForm();
    void start(int mode, QString inp = QString(), QString input2 = QString());  //khởi chạy NotiForm với các chế độ được định nghĩa trong macro.h

private slots:
    void on_button_clicked();

private:
    Ui::NotiForm *ui;
    int currentMode;    //lưu chế độ hiện tại
    QString input;      //lưu dữ liệu vào
    QSqlQuery query;    //truy xuất dữ liệu
};

#endif // NOTIFORM_H
