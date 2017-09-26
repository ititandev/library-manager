#include "bookform.h"
#include "ui_bookform.h"
#include <QDebug>

BookForm::BookForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookForm)
{
    ui->setupUi(this);
}

BookForm::~BookForm()
{
    delete ui;
}

void BookForm::start(int mode, QString maSachInput)
{
    currentMode = mode;     //Lưu lại chế độ hiện tại
    maSach = maSachInput;   //Lưu lại mã sách đang xử lý

    if (mode != THEM_MOI_SACH)
    {
        query.exec("SELECT * FROM book WHERE maSach = '" + maSach + "'");   //Truy xuất thông tin sách
        query.first();                                                      //Lấy bản ghi đầu tiên
        ui->maSachEdit->setText(query.value(0).toString());                 //Lấy dữ liệu mã sách
        ui->tenSachEdit->setText(query.value(1).toString());                //.......
        ui->danhMucComboBox->setCurrentText(query.value(2).toString());
        ui->chuDeEdit->setText(query.value(3).toString());
        ui->tenTacGiaEdit->setText(query.value(4).toString());
        ui->nhaXuatBanEdit->setText(query.value(5).toString());
        ui->namXuatBanEdit->setText(query.value(6).toString());
        ui->tomTatEdit->setText(query.value(7).toString());
        ui->tongCongEdit->setText(query.value(8).toString());
        ui->daMuonEdit->setText(query.value(9).toString());
        ui->giaSachEdit->setText(query.value(10).toString());
        ui->checkBox->setChecked(query.value(12).toBool());
    }
    if (mode == XEM_THEM)   //Chế độ xem thông tin chi tiết sách
    {
        ui->label->setText("Thông tin sách");   //Set label phù hợp
        ui->button->setText("OK");              //Set label nút nhấn

        ui->maSachEdit->setReadOnly(true);      //Set ReadOnly cho các input cần thiết
        ui->tenSachEdit->setReadOnly(true);
        ui->danhMucComboBox->setDisabled(true);
        ui->chuDeEdit->setReadOnly(true);
        ui->tenTacGiaEdit->setReadOnly(true);
        ui->nhaXuatBanEdit->setReadOnly(true);
        ui->namXuatBanEdit->setReadOnly(true);
        ui->tomTatEdit->setReadOnly(true);
        ui->tongCongEdit->setReadOnly(true);
        ui->daMuonEdit->setReadOnly(true);
        ui->giaSachEdit->setReadOnly(true);
        ui->checkBox->setDisabled(true);
    }
    else if (mode == CHINH_SUA_SACH)    //Chế độ chỉnh sửa sách
    {
        ui->label->setText("Thông tin sách");
        ui->button->setText("Cập nhật");

        ui->maSachEdit->setReadOnly(false);
        ui->tenSachEdit->setReadOnly(false);
        ui->danhMucComboBox->setDisabled(false);
        ui->chuDeEdit->setReadOnly(false);
        ui->tenTacGiaEdit->setReadOnly(false);
        ui->nhaXuatBanEdit->setReadOnly(false);
        ui->namXuatBanEdit->setReadOnly(false);
        ui->tomTatEdit->setReadOnly(false);
        ui->tongCongEdit->setReadOnly(false);
        ui->daMuonEdit->setReadOnly(false);
        ui->giaSachEdit->setReadOnly(false);
        ui->checkBox->setDisabled(false);
    }
    else //THEM_MOI_SACH                    Chế độ thêm mới sách
    {
        ui->label->setText("Thêm mới sách");
        ui->button->setText("Thêm");

        ui->maSachEdit->setReadOnly(false);     //Bỏ set readonly các input
        ui->tenSachEdit->setReadOnly(false);
        ui->danhMucComboBox->setDisabled(false);
        ui->chuDeEdit->setReadOnly(false);
        ui->tenTacGiaEdit->setReadOnly(false);
        ui->nhaXuatBanEdit->setReadOnly(false);
        ui->namXuatBanEdit->setReadOnly(false);
        ui->tomTatEdit->setReadOnly(false);
        ui->tongCongEdit->setReadOnly(false);
        ui->daMuonEdit->setReadOnly(false);
        ui->giaSachEdit->setReadOnly(false);
        ui->checkBox->setDisabled(false);

        ui->maSachEdit->clear();                //Clear các input
        ui->tenSachEdit->clear();
        ui->danhMucComboBox->setCurrentIndex(0);
        ui->daMuonEdit->clear();
        ui->chuDeEdit->clear();
        ui->tongCongEdit->clear();
        ui->namXuatBanEdit->clear();
        ui->giaSachEdit->clear();
        ui->tenTacGiaEdit->clear();
        ui->nhaXuatBanEdit->clear();
        ui->tomTatEdit->clear();
        ui->checkBox->setChecked(true);
    }
    show();     //Hiện Form
}

void BookForm::on_button_clicked()
{
    extern LibrarianForm *librarianForm;
    if (currentMode == XEM_THEM)        //Chế độ xem thông tin chi tiết sách
    {
        hide();
    }
    else if (currentMode == CHINH_SUA_SACH)     //Chế độ chỉnh sửa sách
    {
        QString maSachEdit = ui->maSachEdit->text(),    //Lấy dữ liệu các input
        tenSach = ui->tenSachEdit->text(),
        danhMuc = ui->danhMucComboBox->currentText(),
        chuDe = ui->chuDeEdit->text(),
        tenTacGia = ui->tenTacGiaEdit->text(),
        nhaXuatBan = ui->nhaXuatBanEdit->text(),
        namXuatBan = ui->namXuatBanEdit->text(),
        tomTat = ui->tomTatEdit->toPlainText(),
        tongCong = ui->tongCongEdit->text(),
        daMuon = ui->daMuonEdit->text(),
        giaSach = ui->giaSachEdit->text(),
        choMuon = QString::number(ui->checkBox->isChecked()*1);
        query.prepare("UPDATE book SET maSach = :maSach, tenSach = :tenSach, danhMuc = :danhMuc, "      //Cập nhật dữ liệu theo mã sách
                      "chuDe = :chuDe, tenTacGia = :tenTacGia, nhaXuatBan = :nhaXuatBan, "
                      "namXuatBan = :namXuatBan, tomTatND = :tomTat, soSachTongCong = :tongCong, "
                      "soSachDaMuon = :daMuon, giaSach = :giaSach, choMuon = :choMuon "
                      "WHERE maSach = '" + maSach + "'");
        query.bindValue(":maSach", maSachEdit);
        query.bindValue(":tenSach", tenSach);
        query.bindValue(":danhMuc", danhMuc);
        query.bindValue(":chuDe", chuDe);
        query.bindValue(":tenTacGia", tenTacGia);
        query.bindValue(":nhaXuatBan", nhaXuatBan);
        query.bindValue(":namXuatBan", namXuatBan);
        query.bindValue(":tomTat", tomTat);
        query.bindValue(":tongCong", tongCong);
        query.bindValue(":daMuon", daMuon);
        query.bindValue(":giaSach", giaSach);
        query.bindValue(":choMuon", choMuon);

        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Cập nhật thông tin sách thành công");   //Thông báo thành công
            LoginForm::writeHistory("Cập nhật sách " + maSachEdit + ": " + tenSach);        //Ghi lại lịch sử
            librarianForm->queryBook();     //Cập nhật lại bảng tìm kiếm sách
            hide();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Cập nhật thất bại");
    }
    else if (currentMode == THEM_MOI_SACH)  //Chế độ thêm mới sách
    {
        QString maSachEdit = ui->maSachEdit->text(),    //Lấy dữ liệu các input
        tenSach = ui->tenSachEdit->text(),
        danhMuc = ui->danhMucComboBox->currentText(),
        chuDe = ui->chuDeEdit->text(),
        tenTacGia = ui->tenTacGiaEdit->text(),
        nhaXuatBan = ui->nhaXuatBanEdit->text(),
        namXuatBan = ui->namXuatBanEdit->text(),
        tomTat = ui->tomTatEdit->toPlainText(),
        tongCong = ui->tongCongEdit->text(),
        daMuon = ui->daMuonEdit->text(),
        giaSach = ui->giaSachEdit->text(),
        choMuon = QString::number(ui->checkBox->isChecked()*1);
        if (maSachEdit.isEmpty() || tenSach.isEmpty() || tenTacGia.isEmpty() || nhaXuatBan.isEmpty() || namXuatBan.isEmpty() || tongCong.isEmpty() || daMuon.isEmpty())
        {
            QMessageBox::critical(this, "Thông báo", "Bạn nhập thiếu thông tin");
            return;
        }
        query.prepare("INSERT INTO book VALUES (:maSach, :tenSach, :danhMuc, :chuDe, :tenTacGia, "          //Thêm sách vào cơ sở dữ liệu
                      ":nhaXuatBan, :namXuatBan, :tomTat, :tongCong, :daMuon, :giaSach, :ngayNhap, :choMuon)");
        query.bindValue(":maSach", maSachEdit);
        query.bindValue(":tenSach", tenSach);
        query.bindValue(":danhMuc", danhMuc);
        query.bindValue(":chuDe", chuDe);
        query.bindValue(":tenTacGia", tenTacGia);
        query.bindValue(":nhaXuatBan", nhaXuatBan);
        query.bindValue(":namXuatBan", namXuatBan);
        query.bindValue(":tomTat", tomTat);
        query.bindValue(":tongCong", tongCong);
        query.bindValue(":daMuon", daMuon);
        query.bindValue(":giaSach", giaSach);
        query.bindValue(":ngayNhap", QDate::currentDate().toString("yyyy-MM-dd"));
        query.bindValue(":choMuon",choMuon);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Thêm sách " + maSachEdit + " thành công");
            LoginForm::writeHistory("Thêm sách " + maSachEdit + ": " + tenSach);
            librarianForm->queryBook();
            hide();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Thêm sách thất bại");
    }


}
