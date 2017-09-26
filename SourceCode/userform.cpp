#include "userform.h"
#include "ui_userform.h"



UserForm::UserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserForm)
{
    ui->setupUi(this);
}

UserForm::~UserForm()
{
    delete ui;
}

void UserForm::start(int mode, QString userNameInp)
{
    if (userNameInp.isEmpty())
    {
        extern QString userName;
        userNameInput = userName;
    }
    else
        userNameInput = userNameInp;
    currentMode = mode;         //Lưu lại chế độ hiện tại
    passWord = QString();       //Khởi tạo passWord
    ui->userName->clear();
    ui->hoTen->clear();
    ui->ngaySinh->clear();
    ui->CMND->clear();
    ui->MSSV->clear();
    ui->sdt->clear();
    ui->email->clear();
    ui->khoa->setCurrentIndex(0);
    ui->khoaLineEdit->clear();
    ui->khoaLineEdit->hide();
    ui->diaChi->clear();
    ui->thongTin->clear();

    if (mode == DANG_KY_TK)     //Đăng ký tài khoản
    {
        ui->readerCheck->setChecked(true);
        ui->librarianCheck->setChecked(false);
        ui->userManaCheck->setChecked(false);
        ui->readerCheck->setDisabled(true);
        ui->librarianCheck->setDisabled(true);
        ui->userManaCheck->setDisabled(true);
        ui->doiTuongComboBox->setCurrentText("SV");
        ui->Button->setText("Đăng ký");
        ui->createLabel->setText("Đăng ký người dùng mới");
        ui->passWordButton->setText("Tạo mật khẩu");

        ui->userName->setDisabled(false);
        ui->CMND->setDisabled(false);
        ui->MSSV->setDisabled(false);
        ui->doiTuongComboBox->setDisabled(false);
    }
    else //THONG_TIN_TK  &  CHINH_SUA_TK
    {
        ui->passWordButton->setText("Đổi mật khẩu");

        if (mode == THONG_TIN_TK)           //Thông tin tài khoản
        {
            ui->readerCheck->setDisabled(true);
            ui->librarianCheck->setDisabled(true);
            ui->userManaCheck->setDisabled(true);
            ui->doiTuongComboBox->setDisabled(true);
            ui->userName->setDisabled(true);
            ui->CMND->setDisabled(true);
            ui->MSSV->setDisabled(true);
            ui->doiTuongComboBox->setDisabled(true);
        }
        else //CHINH_SUA_TK         //Chỉnh sửa tài khoản
        {
            ui->readerCheck->setDisabled(false);
            ui->librarianCheck->setDisabled(false);
            ui->userManaCheck->setDisabled(false);
            ui->doiTuongComboBox->setDisabled(false);
            ui->userName->setDisabled(true);
            ui->CMND->setDisabled(false);
            ui->MSSV->setDisabled(false);
            ui->doiTuongComboBox->setDisabled(false);
        }

        if (mode == TAO_TK)         //Tạo tài khoản
        {
            ui->readerCheck->setChecked(true);
            ui->librarianCheck->setChecked(false);
            ui->userManaCheck->setChecked(false);
            ui->userName->setDisabled(false);
            ui->passWordButton->setText("Tạo mật khẩu");
            ui->Button->setText("Tạo người dùng");
            show();
            return;
        }
        query.exec("SELECT * FROM user WHERE userName = '" + userNameInput + "'");      //Lấy thông tin người dungf
        query.first();
        ui->userName->setText(userNameInput);       //Đưa thông tin lên UserForm
        passWord = query.value(3).toString();
        ui->hoTen->setText(query.value(4).toString());
        ui->ngaySinh->setDate(QDate::fromString(query.value(5).toString(),"yyyy-MM-dd"));
        ui->CMND->setText(query.value(6).toString());
        ui->MSSV->setText(query.value(7).toString());
        ui->khoa->setCurrentText(query.value(8).toString());
        if (ui->khoa->currentText() != query.value(8).toString())
        {
            ui->khoa->setCurrentIndex(14);
            ui->khoaLineEdit->show();
            ui->khoaLineEdit->setText(query.value(8).toString());
        }
        ui->email->setText(query.value(9).toString());
        ui->sdt->setText(query.value(10).toString());
        ui->diaChi->setText(query.value(11).toString());
        ui->thongTin->setText(query.value(12).toString());
        ui->doiTuongComboBox->setCurrentText(query.value(13).toString());
        int role = query.value(1).toInt();
        ui->readerCheck->setChecked(role%2); role/=2;
        ui->librarianCheck->setChecked(role%2); role/=2;
        ui->userManaCheck->setChecked(role%2);
        ui->Button->setText("Cập nhật");                    //Set label cần thiết
        ui->createLabel->setText("Thông tin tài khoản");
    }
    show();

}

void UserForm::on_Button_clicked()
{
QString userName = ui->userName->text(),
        hoTen = ui->hoTen->text(),
        ngaySinh = ui->ngaySinh->date().toString("yyyy-MM-dd"),
        CMND = ui->CMND->text(),
        MSSV = ui->MSSV->text(),
        email = ui->email->text(),
        sdt = ui->sdt->text(),
        khoa = ui->khoa->currentText(),
        khoa1 = ui->khoaLineEdit->text(),
        diaChi = ui->diaChi->text(),
        thongTin = ui->thongTin->text(),
        doiTuong = ui->doiTuongComboBox->currentText();
        int role =  ui->readerCheck->isChecked()*1 |
                    ui->librarianCheck->isChecked() * 2 |
                    ui->userManaCheck->isChecked() * 4;

    if (currentMode == DANG_KY_TK || currentMode == TAO_TK)     //Đăng ký tài khoản hoặc tạo tài khoản
    {

        if (userName.isEmpty() || passWord.isEmpty() || hoTen.isEmpty() ||
            CMND.isEmpty() || MSSV.isEmpty() || email.isEmpty() || khoa.isEmpty() || diaChi.isEmpty())      //Kiểm tra nhập trống
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "Bạn điền thiếu thông tin");
            return;
        }
        if(checkUnique("CMND", CMND))
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "Người dùng này đã tồn tại");
            return;
        }
        if(checkUnique("MSSV", MSSV))
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "SV/GV/CNVC này đã tồn tại");
            return;
        }
        if(checkUnique("email", email))
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "Email này đã tồn tại");
            return;
        }
        if(checkUnique("soDienThoai", sdt))
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "Số điện thoại này đã tồn tại");
            return;
        }
        if(checkUnique("userName", userName))
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo tài khoản\n"
                                                      "Tên tài khoản này đã tồn tại");
            return;
        }

        query.prepare("INSERT INTO user VALUES (:trangThai, :vaiTro, :userName, :passWord, :hoTen, "            //Thêm người dùng vào cơ sở dữ liệu
                      ":ngaySinh, :CMND, :MSSV, :khoa, :email, :sdt, :diaChi, :thongTin, :doiTuong)");

        if (currentMode == DANG_KY_TK)
            query.bindValue(":trangThai", CHO_XET_DUYET_STRING);
        else if (currentMode == TAO_TK)
            query.bindValue(":trangThai", HOAT_DONG_STRING);

        query.bindValue(":userName", userName);
        query.bindValue(":vaiTro", QString::number(role));
        query.bindValue(":passWord", passWord);
        query.bindValue(":hoTen", hoTen);
        query.bindValue(":ngaySinh", ngaySinh);
        query.bindValue(":CMND", CMND);
        query.bindValue(":MSSV", MSSV);
        if (khoa == "Khác")
            query.bindValue(":khoa", khoa1);
        else
            query.bindValue(":khoa", khoa);
        query.bindValue(":email", email);
        query.bindValue(":sdt", sdt);
        query.bindValue(":diaChi", diaChi);
        query.bindValue(":thongTin", thongTin);
        query.bindValue(":doiTuong", doiTuong);

        if (query.exec())
        {
            if (currentMode == DANG_KY_TK)
            {
                QMessageBox::information(this, "Thông báo", "Tài khoản " + userName +
                                     " đã được đăng ký thành công và đang chờ xét duyệt");
                LoginForm::writeHistory("Đăng ký tài khoản " + userName);
            }
            else
            {
                QMessageBox::information(this, "Thông báo", "Tài khoản " + userName + " đã được tạo thành công và sẵn sàng sử dụng");
                extern UserManagerForm *userManagerForm;
                userManagerForm->queryUser();
                userManagerForm->update();
                LoginForm::writeHistory("Tạo tài khoản " + userName);
            }
            hide();
        }
        else
        {
            QMessageBox::critical(this,"Lỗi", "Không thể tạo sửa tài khoản");
            return;
        }


    }
    else if (currentMode == CHINH_SUA_TK)
    {
        if (userName.isEmpty() || passWord.isEmpty() || hoTen.isEmpty() ||
            CMND.isEmpty() || MSSV.isEmpty() || email.isEmpty() || khoa.isEmpty() || diaChi.isEmpty())      //Kiểm tra nhập trống
        {
            QMessageBox::critical(this,"Lỗi", "Không thể chỉnh sửa tài khoản\n"
                                                      "Bạn điền thiếu thông tin");
            return;
        }
        if (!(role & QUAN_LY_NGUOI_DUNG) && userNameInput == "admin")       //Kiểm tra đang chỉnh sửa tài khoản admin
        {
            QMessageBox::critical(this, "Thông báo", "Không thể tước quyền admin");
            return;
        }

        query.prepare("UPDATE user SET vaiTro = :vaiTro, passWord = :passWord, hoTen = :hoTen, ngaySinh = :ngaySinh, "          //Cập nhật tài khoản
                          "CMND = :CMND, MSSV = :MSSV, khoa = :khoa, email = :email, soDienThoai = :sdt, "
                          "diaChi = :diaChi, thongTin = :thongTin, doiTuong = :doiTuong WHERE userName = :userNameInput");
        query.bindValue(":userNameInput", userNameInput);
        query.bindValue(":vaiTro", QString::number(role));
        query.bindValue(":passWord", passWord);
        query.bindValue(":hoTen", hoTen);
        query.bindValue(":ngaySinh", ngaySinh);
        query.bindValue(":CMND", CMND);
        query.bindValue(":MSSV", MSSV);
        if (khoa == "Khác")
            query.bindValue(":khoa", khoa1);
        else
            query.bindValue(":khoa", khoa);
        query.bindValue(":email", email);
        query.bindValue(":sdt", sdt);
        query.bindValue(":diaChi", diaChi);
        query.bindValue(":thongTin", thongTin);
        query.bindValue(":doiTuong", doiTuong);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Tài khoản " + userNameInput + " được chỉnh sửa thành công");
            LoginForm::writeHistory("Chỉnh sửa tài khoản " + userNameInput);
            extern UserManagerForm *userManagerForm;
            userManagerForm->queryUser();
        }

        else
            QMessageBox::critical(this, "Thông báo", "Lỗi khi chỉnh sửa tài khoản " + userNameInput);
        hide();
    }
    else if (currentMode == THONG_TIN_TK)
    {
        if (hoTen.isEmpty() || (khoa == "Khác" && khoa1.isEmpty()) || email.isEmpty() || sdt.isEmpty() || diaChi.isEmpty())
        {
            QMessageBox::critical(this,"Lỗi cập nhật", "Không thể cập nhật tài khoản\n"
                                                      "Bạn điền thiếu thông tin");
            return;
        }
        query.prepare("UPDATE user SET passWord = :passWord, hoTen = :hoTen, ngaySinh = :ngaySinh, khoa = :khoa, "          //Cập nhật thông tin
                      "email = :email, soDienThoai = :sdt, diaChi = :diaChi, thongTin = :thongTin WHERE userName = '" + userNameInput + "'");
        query.bindValue(":passWord", passWord);
        query.bindValue(":hoTen", hoTen);
        query.bindValue(":ngaySinh", ngaySinh);
        query.bindValue(":MSSV", MSSV);
        if (khoa == "Khác")
            query.bindValue(":khoa", khoa1);
        else
            query.bindValue(":khoa", khoa);
        query.bindValue(":email", email);
        query.bindValue(":sdt", sdt);
        query.bindValue(":diaChi", diaChi);
        query.bindValue(":thongTin", thongTin);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Cập nhật thông tin thành công");
            LoginForm::writeHistory("Cập nhật thông tin tài khoản " + userNameInput);
        }
        else
            QMessageBox::critical(this, "Thông báo", "Cập nhật thông tin thất bại");
    }
}

void UserForm::on_khoa_currentIndexChanged(int index)
{
    if (index == 14)
        ui->khoaLineEdit->show();
    else
        ui->khoaLineEdit->hide();
}

void UserForm::on_passWordButton_clicked()
{
    bool ok = true;
    QString tempPassWord;
    do
    {
        tempPassWord = QInputDialog::getText(this, "Mật khẩu", "Nhập mật khẩu", QLineEdit::Password, QString(), &ok);
        if (!ok)
            return;
    } while (ok && tempPassWord.isEmpty());

    while (ok && (QInputDialog::getText(this, "Mật khẩu", "Nhập lại mật khẩu", QLineEdit::Password, QString(), &ok) != tempPassWord))   //Kiểm tra mật khẩu trùng
    {
        if (!ok)
            return;
        QMessageBox::critical(this, "Thông báo", "Mật khẩu không trùng nhau");

    }

    passWord = QString(QCryptographicHash::hash(tempPassWord.toUtf8(),QCryptographicHash::Sha256).toHex());
}

bool UserForm::checkUnique(QString column, QString data)
{
    query.exec("SELECT COUNT(*) FROM user WHERE user." + column + " = '" + data + "'");     //Kiểm tra duy nhất
    query.first();
    return query.value(0).toInt();
}
