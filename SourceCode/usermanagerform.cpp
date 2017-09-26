#include "usermanagerform.h"
#include "ui_usermanagerform.h"



UserManagerForm::UserManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManagerForm)
{
    ui->setupUi(this);
    queryUser();    //Lấy dữ liệu bảng người dùng
    ui->userTableView->setModel(&queryUserModel);                   //Gán Model với TableView
    queryUserModel.setHeaderData(0, Qt::Horizontal, "userName");    //Set label cột
    queryUserModel.setHeaderData(1, Qt::Horizontal, "Họ Tên");
    queryUserModel.setHeaderData(2, Qt::Horizontal, "Tình trạng");
    queryUserModel.setHeaderData(3, Qt::Horizontal, "Vai trò");
    queryUserModel.setHeaderData(4, Qt::Horizontal, "Ngày Sinh");
    queryUserModel.setHeaderData(5, Qt::Horizontal, "CMND");
    queryUserModel.setHeaderData(6, Qt::Horizontal, "MSSV/GV");
    queryUserModel.setHeaderData(7, Qt::Horizontal, "Khoa");
    queryUserModel.setHeaderData(8, Qt::Horizontal, "Email");
    queryUserModel.setHeaderData(9, Qt::Horizontal, "Số ĐT");
    queryUserModel.setHeaderData(10, Qt::Horizontal, "Thông tin");
    queryUserModel.setHeaderData(11, Qt::Horizontal, "Đối tượng");
    ui->userTableView->resizeColumnsToContents();   //Điều chỉnh kích thước cột
    ui->userTableView->resizeRowsToContents();      //Điều chỉnh kích thước hàng
    ui->userTableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //Chế độ chọn hàng
    ui->userTableView->setSelectionMode(QAbstractItemView::SingleSelection);    //CHế độ chọn đơn

    queryNoti();    //Lấy dữ liệu bảng thông báo
    ui->notiTableView->setModel(&queryModelNoti);
    queryModelNoti.setHeaderData(0, Qt::Horizontal, "Mã");
    queryModelNoti.setHeaderData(1, Qt::Horizontal, "Từ");
    queryModelNoti.setHeaderData(2, Qt::Horizontal, "Tới");
    queryModelNoti.setHeaderData(3, Qt::Horizontal, "Thời gian");
    queryModelNoti.setHeaderData(4, Qt::Horizontal, "Tiêu đề");
    queryModelNoti.setHeaderData(5, Qt::Horizontal, "Nội dung");
    queryModelNoti.setHeaderData(6, Qt::Horizontal, "Đã đọc");
    ui->notiTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->notiTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    queryHistory();     //Lấy dữ liệu bảng lịch sử
    ui->historyTableView->setModel(&queryModelHistory);
    queryModelHistory.setHeaderData(0, Qt::Horizontal, "Mã");
    queryModelHistory.setHeaderData(1, Qt::Horizontal, "Thời gian");
    queryModelHistory.setHeaderData(2, Qt::Horizontal, "userName");
    queryModelHistory.setHeaderData(3, Qt::Horizontal, "Nội dung");
    ui->historyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->historyTableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

UserManagerForm::~UserManagerForm()
{
    delete ui;
}

void UserManagerForm::setRole(int role)
{
    ui->roleComboBox->clear();
    if (role & DOC_GIA)
        ui->roleComboBox->addItem("Độc giả");   //Thêm mục Độc giả
    if (role & THU_THU)
        ui->roleComboBox->addItem("Thủ thư");   //Thêm mục Thủ thư
    if (role & QUAN_LY_NGUOI_DUNG)
        ui->roleComboBox->addItem("Quản lý người dùng");    //Thêm mục Quản lý người dùng
}

void UserManagerForm::start()
{
    active = true;
    ui->roleComboBox->setCurrentText("Quản lý người dùng");
    extern QString userName;
    ui->user->setText(userName);

    ui->searchComboBox->setCurrentIndex(0);
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchUserEdit->clear();
    queryUser();

    ui->searchNotiBox->clear();
    ui->typeNotiComboBox->setCurrentIndex(0);
    queryNoti();


    show();

    LoginForm::writeHistory("Đăng nhập chức năng Quản lý người dùng");
    queryHistory();     //Lấy dữ liệu lịch sử hệ thống
}

void UserManagerForm::queryUser()
{
    QString type, search = QString(), searchBody = ui->searchUserEdit->text();
    switch (ui->typeComboBox->currentIndex())
    {
    case 0:
        type = QString(); break;
    case 1:
        type = "trangThai = -1 AND"; break;
    case 2:
        type = "trangThai = 0 AND"; break;
    case 3:
        type = "trangThai = 1 AND"; break;
    case 4:
        type = "vaiTroStr LIKE 'ĐG%' AND"; break;
    case 5:
        type = "vaiTroStr LIKE '%TT%' AND"; break;
    case 6:
        type = "vaiTroStr LIKE '%QLND' AND"; break;
    case 7:
        type = "doiTuong = 'SV' AND"; break;
    case 8:
        type = "doiTuong = 'GV' AND"; break;
    case 9:
        type = "doiTuong = 'CNVC' AND"; break;
    }

    switch (ui->searchComboBox->currentIndex())
    {
    case 0:
        for (int i = 1; i < 9; i++)
        {
            search += selectColumn(i) + " LIKE '%" + searchBody + "%'";
            if (i!=8)
                search += " OR ";
        }
        break;
    default:
        search = selectColumn(ui->searchComboBox->currentIndex()) + " LIKE '%" + searchBody + "%'";
        break;
    }

    query.prepare("SELECT userName, hoTen, "
                  "CASE trangThai "
                  "WHEN -1 THEN 'Chờ tạo' "
                  "WHEN 0 THEN 'Hoạt động' "
                  "WHEN 1 THEN 'Bị khóa' "
                  "END AS trangThai, "
                  "CASE vaiTro "
                  "WHEN 1 THEN 'ĐG' "
                  "WHEN 2 THEN 'TT' "
                  "WHEN 4 THEN 'QLND' "
                  "WHEN 3 THEN 'ĐG+TT' "
                  "WHEN 5 THEN 'ĐG+QLND' "
                  "WHEN 6 THEN 'TT+QLND' "
                  "WHEN 7 THEN 'ĐG+TT+QLND' "
                  "END AS vaiTroStr, "
                  "ngaySinh, CMND, MSSV, khoa, email, soDienThoai, thongTin, doiTuong "
                  "FROM user WHERE " + type + " (" + search + ") ORDER BY trangThai");      //Lấy dữ liệu người dùng
    query.exec();
    queryUserModel.setQuery(query);
    ui->userTableView->resizeColumnsToContents();
    ui->userTableView->resizeRowsToContents();

    query.exec("SELECT COUNT(*) FROM user WHERE trangThai = " + QString(CHO_XET_DUYET_STRING));
    query.first();
    ui->tabWidget->setTabText(0, "Người dùng (" + query.value(0).toString() + ")");
    ui->typeComboBox->setItemText(1,"Chờ tạo (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM user WHERE trangThai = 0");
    query.first();
    ui->typeComboBox->setItemText(2,"Hoạt động (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM user WHERE trangThai = 1");
    query.first();
    ui->typeComboBox->setItemText(3,"Bị Khóa (" + query.value(0).toString() + ")");
}

QString UserManagerForm::selectColumn(int index)
{
    switch (index)
    {
    case 1:
        return "userName";
        break;
    case 2:
        return "hoTen";
        break;
    case 3:
        return "CMND";
        break;
    case 4:
        return "MSSV";
        break;
    case 5:
        return "khoa";
        break;
    case 6:
        return "thongTin";
        break;
    case 7:
        return "email";
        break;
    case 8:
        return "soDienThoai";
        break;
    }
}

void UserManagerForm::on_roleComboBox_currentTextChanged(const QString &arg1)
{
    extern LibrarianForm *librarianForm;
    extern ReaderForm *readerForm;
    if (active)
    {
        if (arg1 == "Thủ thư")
        {
            hide();
            librarianForm->start();
        }
        if (arg1 == "Độc giả")
        {
            hide();
            readerForm->start();
        }
    }
}

void UserManagerForm::on_logoutForm_clicked()
{
    extern LoginForm *loginForm;
    hide();
    loginForm->start();
    LoginForm::writeHistory("Đăng xuất");
}

void UserManagerForm::on_user_clicked()
{
    extern UserForm *userForm;
    userForm->start(CHINH_SUA_TK);
}

void UserManagerForm::on_createUserButton_clicked()
{
    extern UserForm *userForm;
    userForm->start(TAO_TK);
}

bool UserManagerForm::checkSelectedUser()
{
    return ui->userTableView->selectionModel()->selectedRows().count();
}

void UserManagerForm::on_editUserButton_clicked()
{
    extern UserForm *userForm;
    if (!checkSelectedUser())
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn 1 người dùng");
        return;
    }
    QModelIndex index = ui->userTableView->selectionModel()->selectedRows().at(0);
    QString selectedUserName = ui->userTableView->model()->sibling(index.row(),0,index).data().toString();
    userForm->start(CHINH_SUA_TK, selectedUserName);
}

void UserManagerForm::on_deleteUserButton_clicked()
{
    extern QString userName;
    if (!checkSelectedUser())
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn 1 người dùng");
        return;
    }
    QString selectedUserName = getSelectedUser(0);
    if (selectedUserName == "admin")
    {
        QMessageBox::critical(this, "Thông báo", "Không thể xóa tài khoản hệ thống");
        return;
    }
    if (selectedUserName == userName)
    {
        QMessageBox::critical(this, "Thông báo", "Không thể xóa tài khoản chính mính");
        return;
    }
    else
        query.prepare("DELETE FROM user WHERE userName = '" + selectedUserName + "'");      //Xóa người dùng
    if (query.exec())
    {
        QMessageBox::information(this, "Thông báo", "Xóa thành công tài khoản " + selectedUserName);
        LoginForm::writeHistory("Xóa tài khoản " + selectedUserName);
    }
    else
        QMessageBox::critical(this, "Thông báo", "Xóa thất bại");
    queryUser();
}

void UserManagerForm::on_typeComboBox_currentIndexChanged(int index)
{
    queryUser();
}

void UserManagerForm::on_searchUserButton_clicked()
{
    queryUser();
}

void UserManagerForm::on_searchComboBox_currentIndexChanged(int index)
{
    queryUser();
}

void UserManagerForm::on_viewAllButton_clicked()
{
    ui->searchComboBox->setCurrentIndex(0);
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchUserEdit->clear();
    queryUser();
}

QString UserManagerForm::getSelectedUser(int i)
{
    QModelIndex index = ui->userTableView->selectionModel()->selectedRows().at(0);
    return ui->userTableView->model()->sibling(index.row(),i,index).data().toString();
}

void UserManagerForm::on_lockButton_clicked()
{
    if (!checkSelectedUser())
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn 1 người dùng");
        return;
    }
    if (getSelectedUser(0) == "admin")
    {
        QMessageBox::critical(this, "Thông báo", "Không thể thao tác với tài khoản hệ thống");
        return;
    }
    else
        query.prepare("UPDATE user SET trangThai = :state WHERE userName = :user");     //Cập nhật trạng thái
    if (getSelectedUser(2) == "Hoạt động")
        query.bindValue(":state", BI_KHOA);
    else
        query.bindValue(":state", HOAT_DONG);
    query.bindValue(":user", getSelectedUser(0));
    if (!query.exec())
        QMessageBox::critical(this, "Thông báo", "Lỗi khi thay đổi trạng thái người dùng");
    else
    {
        if (getSelectedUser(2) == "Hoạt động")
            LoginForm::writeHistory("Đổi trạng thái tài khoản " + getSelectedUser(0) + " thành Bị khóa");
        else
            LoginForm::writeHistory("Đổi trạng thái tài khoản " + getSelectedUser(0) + " thành Hoạt động");
        queryUser();
    }
    //temp = ui->userTableView->model()->sibling(index.row(),2,index).data().toString()
}

QString UserManagerForm::getFromUser()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),1,index).data().toString();
}

void UserManagerForm::queryNoti()
{
    QString search = ui->searchNotiBox->text();
    QString type = QString();
    switch (ui->typeNotiComboBox->currentIndex())
    {
    case 0:
        break;
    case 1:
        type = "AND daDoc2 = 'Chưa đọc' ";
        break;
    case 2:
        type = "AND daDoc2 = 'Đã đọc' ";
        break;
    case 3:
        type = "AND titleNoti = 'Yêu cầu hỗ trợ' ";
        break;
    case 4:
        type = "AND titleNoti = 'Góp ý' ";
        break;
    }
    query.exec("SELECT maThongBao, fromUser, "
               "CASE WHEN toUser IS NULL THEN 'Tất cả' "
               "ELSE toUser END AS toUser2, timeNoti, titleNoti, "
               "bodyNoti, CASE daDoc WHEN 1 THEN 'Đã đọc' "
               "WHEN 0 THEN 'Chưa đọc' END AS daDoc2 "
               "FROM noti WHERE titleNoti != 'Đề nghị mua tài liệu' AND ("
               "maThongBao LIKE '%" + search + "%' OR "
               "fromUser LIKE '%" + search + "%' OR "
               "toUser LIKE '%" + search + "%' OR "
               "toUser2 LIKE '%" + search + "%' OR "
               "timeNoti LIKE '%" + search + "%' OR "
               "titleNoti LIKE '%" + search + "%' OR "
               "bodyNoti LIKE '%" + search + "%') " + type +
               "ORDER BY timeNoti DESC");                           //Lấy thông báo
    queryModelNoti.setQuery(query);
    ui->notiTableView->resizeColumnsToContents();
    ui->notiTableView->resizeRowsToContents();
    ui->editNotiButton->setDisabled(true);
    ui->deleteNotiButton->setDisabled(true);

    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti != 'Đề nghị mua tài liệu'");      //Đếm số thông báo chưa đọc
    query.first();
    ui->tabWidget->setTabText(1,"Thông báo (" + query.value(0).toString() + ")");
    ui->typeNotiComboBox->setItemText(1, "Chưa đọc (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti == 'Yêu cầu hỗ trợ'");    //Đếm số thông báo yêu cầu hỗ trợ chưa đọc
    query.first();
    ui->typeNotiComboBox->setItemText(3, "Yêu cầu hỗ trợ (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti == 'Góp ý'");     //Đếm số góp ý chưa đọc
    query.first();
    ui->typeNotiComboBox->setItemText(4, "Góp ý (" + query.value(0).toString() + ")");

}

QString UserManagerForm::getMaThongBao()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),0,index).data().toString();
}

bool UserManagerForm::checkSelectedNoti()
{
    return ui->notiTableView->selectionModel()->selectedRows().count();
}

void UserManagerForm::on_notiTableView_pressed(const QModelIndex &index)
{
    if (checkSelectedNoti())
    {
        ui->editNotiButton->setDisabled(false);
        ui->deleteNotiButton->setDisabled(false);
    }
    else
    {
        ui->editNotiButton->setDisabled(true);
        ui->deleteNotiButton->setDisabled(true);
    }
}

void UserManagerForm::on_editNotiButton_clicked()
{
    extern NotiForm *notiForm;
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    if (getFromUser() == "Quản lý người dùng")
        notiForm->start(CHINH_SUA_THONG_BAO, getMaThongBao());
    else
    {
        notiForm->start(XEM_THONG_BAO, getMaThongBao());
        if (getFromUser() != "Thủ thư")
        {
            query.exec("UPDATE noti SET daDoc = 1 WHERE maThongBao = '" + getMaThongBao() + "'");   //Đánh dấu đã đọc
            queryNoti();
        }
    }
}

void UserManagerForm::on_sendNotiButton_clicked()
{
    extern NotiForm *notiForm;
    if (checkSelectedNoti())
    {
        QString fromUser = getFromUser();
        if (fromUser != "Thủ thư" && fromUser != "Quản lý người dùng")
            notiForm->start(GUI_THONG_BAO, "Thủ thư", fromUser);
    }
    else
    notiForm->start(GUI_THONG_BAO, "Quản lý người dùng");
}

void UserManagerForm::on_deleteNotiButton_clicked()
{
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    if (getFromUser() == "Thủ thư")
    {
        QMessageBox::critical(this, "Thông báo", "Không thể xóa thông báo gửi bởi 'Thủ thư'");
        return;
    }
    query.prepare("DELETE FROM noti WHERE maThongBao = :maThongBao");       //Xóa thông báo
    query.bindValue(":maThongBao", getMaThongBao());
    if (query.exec())
        queryNoti();
    else
        QMessageBox::critical(this, "Lỗi", "Thao tác thất bại");
}

void UserManagerForm::on_typeNotiComboBox_currentTextChanged(const QString &arg1)
{
    queryNoti();
}

void UserManagerForm::on_searchNotiButton_clicked()
{
    queryNoti();
}

void UserManagerForm::queryHistory()
{
    query.exec("SELECT * FROM history ORDER BY timeAction DESC");       //Lấy dữ liệu lịch sử hệ thống
    queryModelHistory.setQuery(query);
    ui->historyTableView->resizeColumnsToContents();
    ui->historyTableView->resizeRowsToContents();
}

void UserManagerForm::on_pushButton_clicked()
{
    query.exec("DELETE FROM history");      //Reset lịch sử
    queryHistory();
}
