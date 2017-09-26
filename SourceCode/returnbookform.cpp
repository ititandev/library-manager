#include "returnbookform.h"
#include "ui_returnbookform.h"

ReturnBookForm::ReturnBookForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReturnBookForm)
{
    ui->setupUi(this);
}

ReturnBookForm::~ReturnBookForm()
{
    delete ui;
}

void ReturnBookForm::start(QString input)
{
    show();
    maSo = input;   //Lưu lại mã số
    ui->huHongCheckBox->setChecked(false);      //Cài đặt ban đầu
    ui->matSachCheckBox->setChecked(false);
    ui->huHongSpin->setValue(0);
    ui->matSachSpin->setValue(0);
    ui->huHongSpin2->setValue(0);

    query.prepare("SELECT bookBorrow.userName, bookBorrow.maSach, hoTen, MSSV, CMND, soLuong, giaSach, heSoPhatMatSach, "
                  "CASE "
                  "WHEN date('now') > thoiGianHetHan THEN "
                  "(julianday(date('now')) - julianday(thoiGianHetHan))*(parameter.phiPhatNgay) "
                  "ELSE NULL "
                  "END AS 'Phạt' FROM "
                  "bookBorrow, user, parameter, book WHERE "
                  "bookBorrow.userName = user.userName AND "
                  "bookBorrow.maSach = book.maSach AND maSo = :maSo");      //Lấy dữ liệu yêu cầu
    query.bindValue(":maSo", maSo);
    query.exec();
    query.first();
    ui->maSach->setText(query.value(1).toString());
    ui->userName->setText(query.value(0).toString());
    ui->hoTen->setText(query.value(2).toString());
    ui->MSSV->setText(query.value(3).toString());
    ui->CMND->setText(query.value(4).toString());
    int SL = query.value(5).toInt();
    ui->SL->setText(QString::number(SL));
    ui->quaHanSpin->setMaximum(SL);
    ui->huHongSpin->setMaximum(SL);
    ui->matSachSpin->setMaximum(SL);
    int giaSach = query.value(6).toInt();
    ui->giaSach->setText(QString::number(giaSach));
    ui->matSachSpin_2->setValue(giaSach * query.value(7).toInt());
    int phat = query.value(8).toInt();
    ui->quaHanSpin2->setValue(phat);
    if (phat == 0)
    {
        ui->quaHanCheckBox->setChecked(false);
        ui->quaHanSpin->setValue(0);
    }
    else
    {
        ui->quaHanSpin->setValue(SL);
        ui->quaHanCheckBox->setChecked(true);
    }
    getPunishValue();
}

void ReturnBookForm::getPunishValue()
{
    phatValue = 0;
    chuThich = QString();
    if (ui->quaHanCheckBox->isChecked())
    {
        chuThich += "Quá hạn & ";
        phatValue += ui->quaHanSpin->value() * ui->quaHanSpin2->value();
    }
    if (ui->huHongCheckBox->isChecked())
    {
        chuThich += "Hư hỏng & ";
        phatValue += ui->huHongSpin->value() * ui->huHongSpin2->value();
    }
    if (ui->matSachCheckBox->isChecked())
    {
        chuThich += "Mất sách & ";
        phatValue += ui->matSachSpin->value() * ui->matSachSpin_2->value();
    }
    ui->phat->setText(QString::number(phatValue));
    chuThich.remove(chuThich.size() - 3, 3);
    if (!ui->chuThich->text().isEmpty())
    {
        if (chuThich.isEmpty())
            chuThich += ui->chuThich->text();
        else
            chuThich += " & " + ui->chuThich->text();
    }
}

void ReturnBookForm::on_exitButton_clicked()
{
    hide();
}

void ReturnBookForm::on_pushButton_clicked()
{
    getPunishValue();
    query.prepare("UPDATE bookBorrow SET trangThai = :trangThai, phat = :phat, "        //Cập nhật yêu cầu thành đã mượn hoặc đã phạt
                  "thoiGianTra = date('now'), chuThich = :chuThich "
                  "WHERE maSo = :maSo");
    query.bindValue(":chuThich", chuThich);
    query.bindValue(":maSo", maSo);
    if (phatValue == 0)
    {
        query.bindValue(":trangThai", DA_MUON);
        query.bindValue(":phat", QString());
    }
    else
    {
        query.bindValue(":trangThai", DA_PHAT);
        query.bindValue(":phat", phatValue);
    }

    if (query.exec())
    {
        query.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon - :soLuong WHERE maSach = :maSach");     //Trả lại sách cho thư viện
        query.bindValue(":soLuong", QString::number(ui->SL->text().toInt() - ui->matSachSpin->value()));
        query.bindValue(":maSach", ui->maSach->text());
        query.exec();
        QMessageBox::information(this, "Thông báo", "Trả/Phạt thành công");
        hide();
        extern LibrarianForm *librarianForm;
        librarianForm->queryRequest();
        librarianForm->queryBook();
    }
    else
        QMessageBox::critical(this, "Thất bại", "Thao tác thất bại");
    //extern LibrarianForm *librarianForm;
    //librarianForm->queryRequest();
}

void ReturnBookForm::on_quaHanCheckBox_toggled(bool checked)
{
    getPunishValue();
}

void ReturnBookForm::on_huHongCheckBox_toggled(bool checked)
{
    getPunishValue();
}

void ReturnBookForm::on_matSachCheckBox_toggled(bool checked)
{
    getPunishValue();
}

void ReturnBookForm::on_quaHanSpin_valueChanged(int arg1)
{
    getPunishValue();
}

void ReturnBookForm::on_huHongSpin_valueChanged(int arg1)
{
    getPunishValue();
}

void ReturnBookForm::on_matSachSpin_valueChanged(int arg1)
{
    getPunishValue();
}

void ReturnBookForm::on_quaHanSpin2_valueChanged(int arg1)
{
    getPunishValue();
}

void ReturnBookForm::on_huHongSpin2_valueChanged(int arg1)
{
    getPunishValue();
}

void ReturnBookForm::on_matSachSpin_2_valueChanged(int arg1)
{
    getPunishValue();
}
