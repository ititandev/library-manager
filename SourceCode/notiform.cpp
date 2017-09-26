#include "notiform.h"
#include "ui_notiform.h"

NotiForm::NotiForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotiForm)
{
    ui->setupUi(this);
}

NotiForm::~NotiForm()
{
    delete ui;
}

void NotiForm::start(int mode, QString inp, QString input2)
{
    currentMode = mode;     //Lưu chế độ hiện tại
    input = inp;        //Lưu lại input
    ui->fromEdit->clear();
    ui->toEdit->clear();
    ui->titleEdit->clear();
    ui->bodyEdit->clear();

    if (mode == XEM_THONG_BAO || mode == CHINH_SUA_THONG_BAO)
    {
        query.exec("SELECT * FROM noti WHERE maThongBao = '" + input + "'");    //Lấy dữ liệu bảng thông báo
        query.first();
        ui->fromEdit->setText(query.value(1).toString());
        ui->toEdit->setText(query.value(2).toString());
        ui->dateEdit->setDate(QDate::fromString(query.value(3).toString(),"yyyy-MM-dd"));
        ui->titleEdit->setText(query.value(4).toString());
        ui->bodyEdit->setText(query.value(5).toString());

        if (mode == XEM_THONG_BAO)      //Xem thông báo
        {
            ui->fromEdit->setReadOnly(true);
            ui->toEdit->setReadOnly(true);
            ui->dateEdit->setReadOnly(true);
            ui->titleEdit->setReadOnly(true);
            ui->bodyEdit->setReadOnly(true);
            ui->button->setText("OK");
            if (query.value(2).toString().isEmpty())
                ui->toEdit->setText("Tất cả");
        }
        else        //Chỉnh sửa thông báo
        {
            ui->fromEdit->setReadOnly(true);
            ui->toEdit->setReadOnly(false);
            ui->dateEdit->setReadOnly(true);
            ui->titleEdit->setReadOnly(false);
            ui->bodyEdit->setReadOnly(false);
            ui->button->setText("Chỉnh sửa");
        }

    }
    else if (mode == YEU_CAU_HO_TRO || mode == GOP_Y || mode == DE_NGHI_MUA_SACH)       //Yêu cầu hỗ trợ, góp ý, đề nghị mua sách
    {
        if (input.isEmpty())
            ui->fromEdit->setReadOnly(false);
        else
            ui->fromEdit->setReadOnly(true);
        ui->fromEdit->setText(input);
        ui->toEdit->setText("LibPro");
        ui->dateEdit->setDate(QDate::currentDate());
        if (mode == YEU_CAU_HO_TRO)
            ui->titleEdit->setText("Yêu cầu hỗ trợ");
        else if (mode == GOP_Y)
            ui->titleEdit->setText("Góp ý");
        else
            ui->titleEdit->setText("Đề nghị mua tài liệu");
        ui->toEdit->setReadOnly(true);
        ui->dateEdit->setReadOnly(true);
        ui->titleEdit->setReadOnly(true);
        ui->bodyEdit->setReadOnly(false);
        ui->button->setText("Gửi yêu cầu");
    }
    else if (mode == GUI_THONG_BAO)         //Gửi thông báo
    {
        ui->fromEdit->setText(input);
        ui->toEdit->setText(input2);
        ui->dateEdit->setDate(QDate::currentDate());
        ui->fromEdit->setReadOnly(true);
        ui->toEdit->setReadOnly(false);
        ui->dateEdit->setReadOnly(true);
        ui->titleEdit->setReadOnly(false);
        ui->bodyEdit->setReadOnly(false);
        ui->button->setText("Gửi thông báo");
    }
    show();
}

void NotiForm::on_button_clicked()
{
    extern LibrarianForm *librarianForm;
    extern UserManagerForm *userManagerForm;

    QString fromUser = ui->fromEdit->text(),
            toUser = ui->toEdit->text(),
            timeNoti = ui->dateEdit->date().toString("yyyy-MM-dd"),
            titleNoti = ui->titleEdit->text(),
            bodyNoti = ui->bodyEdit->toPlainText(),
            daDoc;
    if (currentMode == GUI_THONG_BAO)       //chế độ gửi thông báo
    {
        if (toUser.isEmpty())
        {
            toUser = "NULL";
            daDoc = "NULL";
        }
        else
        {
            toUser = "'" + toUser + "'";
            daDoc = '0';
        }
        query.prepare("INSERT INTO noti SELECT MAX(maThongBao) + 1, :fromUser, " + toUser + ", "
                      ":timeNoti, :titleNoti, :bodyNoti, " + daDoc + " FROM noti");                 //Thêm thông báo vào cơ sở dữ liệu
        query.bindValue(":fromUser", fromUser);
        query.bindValue(":timeNoti", timeNoti);
        query.bindValue(":titleNoti", titleNoti);
        query.bindValue(":bodyNoti", bodyNoti);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Gửi thông báo thành công");
            librarianForm->queryNoti();
            userManagerForm->queryNoti();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Gửi thông báo thất bại");
    }
    else if (currentMode == CHINH_SUA_THONG_BAO)        //Chế độ chỉnh sửa thông báo
    {
        if (toUser.isEmpty())
        {
            toUser = "NULL";
        }
        else
        {
            toUser = "'" + toUser + "'";
        }
        query.prepare("UPDATE noti SET toUser = " + toUser + ", titleNoti = :titleNoti, "       //Cập nhật thông báo
                      "bodyNoti = :bodyNoti WHERE maThongBao = :maThongBao");
        query.bindValue(":titleNoti", titleNoti);
        query.bindValue(":bodyNoti", bodyNoti);
        query.bindValue(":maThongBao", input);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Chỉnh sửa thông báo thành công");
            librarianForm->queryNoti();
            userManagerForm->queryNoti();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Chỉnh sửa thông báo thất bại");
    }
    else if (currentMode == YEU_CAU_HO_TRO || currentMode == GOP_Y || currentMode == DE_NGHI_MUA_SACH)      //Chế độ yêu cầu hỗ trợ, góp ý, đề nghị mua sách
    {
        if (fromUser.isEmpty())
        {
            QMessageBox::critical(this, "Thông báo", "Không được để trống người gửi");
            return;
        }
        if (bodyNoti.isEmpty())
        {
            QMessageBox::critical(this, "Thông báo", "Không được để nội dung trống");
            return;
        }
        query.prepare("INSERT INTO noti SELECT MAX(maThongBao) + 1, :fromUser, :toUser, "       //Thêm thông báo tương ứng
                      ":timeNoti, :titleNoti, :bodyNoti, 0 FROM noti");
        query.bindValue(":fromUser", fromUser);
        query.bindValue(":toUser", toUser);
        query.bindValue(":timeNoti", timeNoti);
        query.bindValue(":titleNoti", titleNoti);
        query.bindValue(":bodyNoti", bodyNoti);
        if (query.exec())
        {
            if (currentMode == GOP_Y)
                QMessageBox::information(this, "Thông báo", "Cảm ơn góp ý của bạn về LibPro\nChúng tôi sẽ xem xét để cải thiện phần mềm");
            else if (currentMode == YEU_CAU_HO_TRO)
                QMessageBox::information(this, "Thông báo", "Gửi yêu cầu hỗ trợ thành công");
            else
                QMessageBox::information(this, "Thông báo", "Đề nghị mua tài liệu thành công\nChúng tôi sẽ xem xét");
            extern ReaderForm *readerForm;
            readerForm->queryNoti();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Thao tác thất bại");
    }
    hide();
}
