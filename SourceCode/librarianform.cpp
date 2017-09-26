#include "librarianform.h"
#include "ui_librarianform.h"


LibrarianForm::LibrarianForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibrarianForm)//,
    //returnBookForm(this)
{
    ui->setupUi(this);
    queryBook();        //lấy dữ liệu bảng sách
    queryModel.setHeaderData(0, Qt::Horizontal, "Mã sách");     //Set tiêu đề cột
    queryModel.setHeaderData(1, Qt::Horizontal, "Tên sách");
    queryModel.setHeaderData(2, Qt::Horizontal, "Danh mục");
    queryModel.setHeaderData(3, Qt::Horizontal, "Chủ đề");
    queryModel.setHeaderData(4, Qt::Horizontal, "Tên tác giả");
    queryModel.setHeaderData(5, Qt::Horizontal, "Nhà xuất bản");
    queryModel.setHeaderData(6, Qt::Horizontal, "Năm xuất bản");
    queryModel.setHeaderData(7, Qt::Horizontal, "Đã mượn");
    queryModel.setHeaderData(8, Qt::Horizontal, "Cho mượn");

    ui->bookTableView->setModel(&queryModel);                                   //Gán Model với bảng TableView
    ui->bookTableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //Chế độ chọn hàng
    ui->bookTableView->setSelectionMode(QAbstractItemView::SingleSelection);    //Chế độ chọn đơn

    queryRequest();     //Lấy dữ liệu bảng yêu cầu
    queryModelRequest.setHeaderData(0, Qt::Horizontal, "Mã số");        //Set tiêu đề cột
    queryModelRequest.setHeaderData(1, Qt::Horizontal, "Tài khoản");
    queryModelRequest.setHeaderData(2, Qt::Horizontal, "Họ tên");
    queryModelRequest.setHeaderData(3, Qt::Horizontal, "CMND");
    queryModelRequest.setHeaderData(4, Qt::Horizontal, "MSSV/GV");
    queryModelRequest.setHeaderData(5, Qt::Horizontal, "Mã sách");
    queryModelRequest.setHeaderData(6, Qt::Horizontal, "Tên sách");
    queryModelRequest.setHeaderData(7, Qt::Horizontal, "Tác giả");
    queryModelRequest.setHeaderData(8, Qt::Horizontal, "SL");
    queryModelRequest.setHeaderData(9, Qt::Horizontal, "Yêu cầu");
    queryModelRequest.setHeaderData(10, Qt::Horizontal, "Mượn");
    queryModelRequest.setHeaderData(11, Qt::Horizontal, "Hết hạn");
    queryModelRequest.setHeaderData(12, Qt::Horizontal, "Trả");
    queryModelRequest.setHeaderData(13, Qt::Horizontal, "Trạng thái");
    queryModelRequest.setHeaderData(14, Qt::Horizontal, "Phạt");
    queryModelRequest.setHeaderData(15, Qt::Horizontal, "Chú thích");
    ui->requestTableView->setModel(&queryModelRequest);                             //Gán Model với bảng TableView
    ui->requestTableView->setSelectionBehavior(QAbstractItemView::SelectRows);      //Chế độ chọn hàng
    ui->requestTableView->setSelectionMode(QAbstractItemView::SingleSelection);     //Chế độ chọn đơn

    queryNoti();        //Lấy dữ liệu bảng thông báo
    queryModelNoti.setHeaderData(0, Qt::Horizontal, "Mã");      //Set tiêu đề cột
    queryModelNoti.setHeaderData(1, Qt::Horizontal, "Từ");
    queryModelNoti.setHeaderData(2, Qt::Horizontal, "Tới");
    queryModelNoti.setHeaderData(3, Qt::Horizontal, "Thời gian");
    queryModelNoti.setHeaderData(4, Qt::Horizontal, "Tiêu đề");
    queryModelNoti.setHeaderData(5, Qt::Horizontal, "Nội dung");
    queryModelNoti.setHeaderData(6, Qt::Horizontal, "Đã đọc");
    ui->notiTableView->setModel(&queryModelNoti);                               //Gán Model với bảng TableView
    ui->notiTableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //Chế độ chọn hàng
    ui->notiTableView->setSelectionMode(QAbstractItemView::SingleSelection);    //Chế độ chọn đơn

    querySetting();     //Lấy dữ liệu cài đặt
}

LibrarianForm::~LibrarianForm()
{
    delete ui;
}
void LibrarianForm::setRole(int role)
{
    ui->roleComboBox->clear();
    if (role & DOC_GIA)
        ui->roleComboBox->addItem("Độc giả");   //Thêm mục Độc giả
    if (role & THU_THU)
        ui->roleComboBox->addItem("Thủ thư");   //Thêm mục Thủ thư
    if (role & QUAN_LY_NGUOI_DUNG)
        ui->roleComboBox->addItem("Quản lý người dùng");    //Thêm mục Quản lý người dùng
}

void LibrarianForm::start()
{
    active = true;
    //Đưa về trạng thái ban đầu
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    ui->sortComboBox->setCurrentIndex(0);
    ui->sortModeComboBox->setCurrentIndex(1);
    ui->sortModeComboBox->setDisabled(true);
    queryBook();

    queryRequest();

    queryNoti();

    ui->roleComboBox->setCurrentText("Thủ thư");
    extern QString userName;
    ui->user->setText(userName);
    show();
    LoginForm::writeHistory("Đăng nhập chức năng Thủ thư");
}

void LibrarianForm::on_roleComboBox_currentTextChanged(const QString &arg1)
{
    extern ReaderForm *readerForm;
    extern UserManagerForm *userManagerForm;
    if (active)
    {
        if (arg1 == "Độc giả")
        {
            readerForm->start();
            hide();
        }
        if (arg1 == "Quản lý người dùng")
        {
            userManagerForm->start();
            hide();
        }
    }

}

QString LibrarianForm::selectColumn(int choice)
{
    switch (choice)
    {
    case 1:
        return "maSach ";
        break;
    case 2:
        return "tenSach ";
        break;
    case 3:
        return "chuDe ";
        break;
    case 4:
        return "tenTacGia ";
        break;
    case 5:
        return "nhaXuatBan ";
        break;
    case 6:
        return "namXuatBan ";
        break;
    case 7:
        return "soSachDaMuon ";
        break;
    case 8:
        return "tomTatND ";
        break;
    }
    return "";
}

void LibrarianForm::on_logoutButton_clicked()
{
    extern LoginForm *loginForm;
    hide();
    loginForm->start();
    LoginForm::writeHistory("Đăng xuất");
}

void LibrarianForm::on_searchButton_clicked()
{
    queryBook();
}

void LibrarianForm::on_viewAllButton_clicked()
{
    ui->searchBox->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    queryBook();
}

void LibrarianForm::queryBook()
{
    QString type = QString(), search, order;
    if (ui->typeComboBox->currentIndex() != 0)
        type = "(danhMuc = '"+ ui->typeComboBox->currentText() +"') AND ";

    switch (ui->searchComboBox->currentIndex())
    {
    case 0:
        for (int i = 1; i < 9; i++)
        {
            search += selectColumn(i) + " LIKE '%" + ui->searchBox->text() + "%' ";
            if (i!=8)
                search += " OR ";
        }
        break;
    default:
        search = selectColumn(ui->searchComboBox->currentIndex()) + " LIKE '%" + ui->searchBox->text() + "%' ";
        break;
    }

    if (ui->sortComboBox->currentIndex() == 0)
        order += "ORDER BY ngayNhap DESC";
    else
    {
        order += "ORDER BY " + selectColumn(ui->sortComboBox->currentIndex());
        order += (ui->sortModeComboBox->currentIndex() == 1)?(" DESC"):("");
    }
    query.exec("SELECT maSach, tenSach, danhMuc, chuDe, tenTacGia, nhaXuatBan, "
               "namXuatBan, soSachDaMuon || '/' || soSachTongCong, CASE choMuon WHEN 0 THEN 'Không' ELSE 'Có' END "
               "FROM book WHERE " + type + " (" + search + ") " + order);   //Lấy các bản ghi thuộc danh mục type, nội dung search và sắp xếp
    queryModel.setQuery(query);
    ui->bookTableView->resizeColumnsToContents();   //Điều chỉnh kích thuớc
    ui->bookTableView->resizeRowsToContents();
    ui->bookTableView->setColumnWidth(1,250);   //Điều chỉnh chiều rộng cột
    ui->bookTableView->setColumnWidth(2,120);
    ui->bookTableView->setColumnWidth(4,180);
    ui->bookTableView->setColumnWidth(5,180);
}

void LibrarianForm::on_sortComboBox_currentIndexChanged(int index)
{
    if (index == 0) //Chế độ sắp xếp theo mới về
        ui->sortModeComboBox->setDisabled(true);
    else
        ui->sortModeComboBox->setDisabled(false);
    queryBook();
}

void LibrarianForm::on_sortModeComboBox_currentIndexChanged(int index)
{
    queryBook();
}


void LibrarianForm::on_typeComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void LibrarianForm::on_searchComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void LibrarianForm::on_viewMoreButton_clicked()
{
    if (ui->bookTableView->selectionModel()->selectedRows().count() != 1)   //Kiểm tra số lượng sách được chọn
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để xem/chỉnh sửa");
        return;
    }
    QModelIndex index = ui->bookTableView->selectionModel()->selectedRows().at(0);
    QString maSach = ui->bookTableView->model()->sibling(index.row(),0,index).data().toString();
    extern BookForm *bookForm;
    bookForm->start(CHINH_SUA_SACH, maSach);    //Chỉnh sửa sách
}


void LibrarianForm::on_deleteBookButton_clicked()
{
    if (ui->bookTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để xóa");
        return;
    }
    QModelIndex index = ui->bookTableView->selectionModel()->selectedRows().at(0);
    QString maSach = ui->bookTableView->model()->sibling(index.row(),0,index).data().toString();
    QString tenSach = ui->bookTableView->model()->sibling(index.row(),1,index).data().toString();
    query.prepare("DELETE FROM book WHERE maSach = :maSach");       //Xóa sách trong cơ sở dữ liệu
    query.bindValue(":maSach", maSach);
    if (query.exec())
    {
        LoginForm::writeHistory("Xóa sách " + maSach + ": " + tenSach);
        QMessageBox::information(this, "Thông báo", "Xóa thành công sách " + maSach + "khỏi thư viện");
        queryBook();
    }

}

void LibrarianForm::on_addBookButton_clicked()
{
    extern BookForm *bookForm;
    bookForm->start(THEM_MOI_SACH,"");  //Chế độ thêm mới sách
}

void LibrarianForm::on_user_clicked()
{
    extern UserForm *userForm;
    userForm->start(THONG_TIN_TK);      //Xem thông tin sách
}

QString LibrarianForm::selectColumnRequest(int choice)
{   //Đổi về cột trong database của bảng borrowBook
    switch (choice)
    {
    case 1:
        return "bookBorrow.userName"; break;
    case 2:
        return "user.hoTen"; break;
    case 3:
        return "user.CMND"; break;
    case 4:
        return "user.MSSV"; break;
    case 5:
        return "bookBorrow.maSach"; break;
    case 6:
        return "book.tenSach"; break;
    case 7:
        return "thoiGianYeuCau"; break;
    case 8:
        return "thoiGianMuon"; break;
    case 9:
        return "thoiGianHetHan"; break;
    case 10:
        return "thoiGianTra"; break;
    case 11:
        return "chuThich"; break;
    }
}

void LibrarianForm::queryRequest()
{
    QString type, search, searchBody = ui->searchRequestBox->text();
    switch (ui->typeRequestComboBox->currentIndex())
    {
    case 0:
        break;
    case 1:
        type = "trangThaiStr = 'Giỏ sách' AND "; break;
    case 2:
        type = "trangThaiStr = 'Chờ mượn' AND "; break;
    case 3:
        type = "trangThaiStr = 'Đang mượn' AND "; break;
    case 4:
        type = "trangThaiStr = 'Đang mượn' AND date('now') > thoiGianHetHan AND "; break;
    case 5:
        type = "trangThaiStr = 'Đã mượn' AND "; break;
    case 6:
        type = "trangThaiStr = 'Đã phạt' AND "; break;
    }

    switch (ui->searchRequestComboBox->currentIndex())
    {
    case 0:
        for (int i = 1; i < 12; i++)
        {
            search += selectColumnRequest(i) + " LIKE '%" + searchBody + "%'";
            if (i != 11)
                search += " OR ";
        }
        break;
    default:
        search = selectColumnRequest(ui->searchRequestComboBox->currentIndex()) + " LIKE '%" + searchBody + "%'";
        break;
    }

    query.prepare("SELECT maSo, bookBorrow.userName, hoTen, CMND, MSSV, bookBorrow.maSach, tenSach, tenTacGia, "
                  "soLuong, thoiGianYeuCau, thoiGianMuon, thoiGianHetHan, thoiGianTra, "
                  "CASE bookBorrow.trangThai "
                  "WHEN -2 THEN 'Giỏ sách' "
                  "WHEN -1 THEN 'Chờ mượn' "
                  "WHEN 0 THEN 'Đang mượn' "
                  "WHEN 1 THEN 'Đã mượn' "
                  "WHEN 2 THEN 'Đã phạt' END "
                  "AS trangThaiStr, "
                  "CASE "
                  "WHEN date('now') > thoiGianHetHan AND bookBorrow.trangThai = 0 THEN "
                  "(julianday(date('now')) - julianday(thoiGianHetHan))*(parameter.phiPhatNgay) "
                  "ELSE bookBorrow.phat "
                  "END AS 'Phạt', chuThich "
                  "FROM book, bookBorrow, user, parameter "
                  "WHERE bookBorrow.maSach = book.maSach AND bookBorrow.userName = user.userName AND " +
                  type + " (" + search + ") ORDER BY trangThaiStr");        //Xem những yêu cầu với danh mục type và nội dung search
    query.exec();
    queryModelRequest.setQuery(query);
    ui->requestTableView->resizeColumnsToContents();
    ui->requestTableView->resizeRowsToContents();
    ui->requestTableView->setColumnWidth(6,250);

    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE trangThai = " + QString(CHO_MUON));   //Đếm yêu cầu cho mượn
    query.first();
    ui->tabWidget->setTabText(1, "Yêu cầu (" + query.value(0).toString() + ")");
    ui->typeRequestComboBox->setItemText(2,"Chờ mượn (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE trangThai = " + QString(DANG_MUON));  //Đếm yêu cầu đang mượn
    query.first();
    ui->typeRequestComboBox->setItemText(3,"Đang mượn (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE trangThai = " + QString(DANG_MUON) + " AND date('now') > thoiGianHetHan");    //Đếm yêu cầu quá hạn
    query.first();
    ui->typeRequestComboBox->setItemText(4,"Quá hạn (" + query.value(0).toString() + ")");  //Đếm số yêu cầu đã mượn
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE trangThai = " + QString(DA_MUON));
    query.first();
    ui->typeRequestComboBox->setItemText(5,"Đã mượn (" + query.value(0).toString() + ")");  //Đếm số yêu cầu đã phạt
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE trangThai = " + QString(DA_PHAT));
    query.first();
    ui->typeRequestComboBox->setItemText(6,"Đã phạt (" + query.value(0).toString() + ")");
}

void LibrarianForm::on_viewAllRequestButton_clicked()
{
    ui->searchRequestComboBox->setCurrentIndex(0);
    ui->typeRequestComboBox->setCurrentIndex(0);
    ui->searchRequestBox->clear();
    queryRequest();
}

void LibrarianForm::on_searchRequestButton_clicked()
{
    queryRequest();
}

void LibrarianForm::on_searchRequestComboBox_currentIndexChanged(int index)
{
    queryRequest();
}

void LibrarianForm::on_typeRequestComboBox_currentIndexChanged(int index)
{
    queryRequest();
}

QString LibrarianForm::getRecord(int i)
{
    QModelIndex index = ui->requestTableView->selectionModel()->selectedRows().at(0);
    return ui->requestTableView->model()->sibling(index.row(),i,index).data().toString();
}

void LibrarianForm::on_button_clicked()
{
    extern ReturnBookForm *returnBookForm;
    if (!checkSelectedRequest())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn một bản ghi");
        return;
    }
    if (getRecord(13) == "Chờ mượn")
    {
        bool ok;
        QString maSach = getRecord(5);
        int soLuong = getRecord(8).toInt();
        int soLuongMuon = QInputDialog::getInt(this, "Cho mượn", "Số lượng cho mượn", getRecord(8).toInt(), 1, getRecord(8).toInt(), 1, &ok);
        if (!ok)
            return;
        int day = QInputDialog::getInt(this, "Cho mượn", "Số ngày cho mượn", 14, 1, 1000, 1, &ok);
        if (ok)
        {
            query.prepare("UPDATE bookBorrow SET thoiGianMuon = date('now'), soLuong = :soLuongMuon, "
                          "thoiGianHetHan = date('now','+" + QString::number(day) + " day'), trangThai = :trangThai "
                          "WHERE maSo = :maSo");    //Cập nhật thời gian mượn, hết hạn, số lượng, trạng thái thành 'Đang mượn'
            query.bindValue(":soLuongMuon", soLuongMuon);
            query.bindValue(":day", day);
            query.bindValue(":trangThai", DANG_MUON);
            query.bindValue(":maSo", getRecord(0));
            if (query.exec())
            {
                QMessageBox::information(this, "Thông báo", "Cho mượn thành công");
                query.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon - :SL WHERE maSach = :maSach");  //Trả lại những sách không lấy
                query.bindValue(":SL", QString(soLuong - soLuongMuon));
                query.bindValue(":maSach", maSach);
                query.exec();
                queryRequest();
            }

            else
                QMessageBox::critical(this, "Thất bại", "Thao tác thất bại");
        }
    }
    else if (getRecord(13) == "Đang mượn")
        returnBookForm->start(getRecord(0));
}

void LibrarianForm::on_deleteButton_clicked()
{
    if (!checkSelectedRequest())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn một bản ghi");
        return;
    }
    QString soLuong = getRecord(8);
    QString maSach = getRecord(5);
    QString state = getRecord(13);
    if (state != "Chờ mượn")
    {
        QMessageBox::critical(this, "Thông báo", "Không được phép\nChọn một bản ghi 'Chờ mượn'");
        return;
    }
    query.prepare("DELETE FROM bookBorrow WHERE maSo = " + getRecord(0));   //Hủy yêu cầu mượn sách
    if (query.exec())
    {
        queryRequest();
        query.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon - :SL WHERE maSach = :maSach");  //Giảm số lượng đã mượn
        query.bindValue(":SL", soLuong);
        query.bindValue(":maSach", maSach);
        query.exec();
        queryBook();
    }
    else
        QMessageBox::information(this, "Lỗi", "Lỗi khi hủy yêu cầu");
}


void LibrarianForm::on_extendButton_clicked()
{
    if (getRecord(13) == "Đang mượn")
    {
        bool ok;
        int soNgay = QInputDialog::getInt(this, "Gia hạn", "Nhập số ngày muốn gia hạn", 7, 1, 1000, 1, &ok);
        if (ok)
        {
            query.prepare("UPDATE bookBorrow SET thoiGianHetHan = date(thoiGianHetHan,'+" + QString::number(soNgay) + " day') WHERE maSo = :maSo"); //Cập nhật số ngày hết hạn
            query.bindValue(":soNgay", soNgay);
            query.bindValue(":maSo", getRecord(0));
            if (query.exec())
            {
                QMessageBox::information(this, "Thông báo", "Gia hạn thành công");
                queryRequest();
            }
            else
                QMessageBox::critical(this, "Thông báo", "Gia hạn thất bại");
        }
    }
    else
    {
        QMessageBox::critical(this, "Thông báo", "Không được phép\nChọn một bản ghi 'Đang mượn'");
    }
}


QString LibrarianForm::getFromUser()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),1,index).data().toString();
}

void LibrarianForm::queryNoti()
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
        type = " AND titleNoti = 'Đề nghị mua tài liệu' ";
        break;
    case 4:
        type = " AND titleNoti = 'Yêu cầu hỗ trợ' ";
        break;
    case 5:
        type = " AND titleNoti = 'Góp ý' ";
        break;
    }
    query.exec("SELECT maThongBao AS 'Mã', fromUser AS 'Từ', "
               "CASE WHEN toUser IS NULL THEN 'Tất cả' "
               "ELSE toUser END AS toUser2, timeNoti AS 'Thời gian', titleNoti AS 'Tiêu đề', "
               "bodyNoti AS 'Nội dung', CASE daDoc WHEN 1 THEN 'Đã đọc' "
               "WHEN 0 THEN 'Chưa đọc' END AS daDoc2 "
               "FROM noti WHERE ("
               "maThongBao LIKE '%" + search + "%' OR "
               "fromUser LIKE '%" + search + "%' OR "
               "toUser LIKE '%" + search + "%' OR "
               "toUser2 LIKE '%" + search + "%' OR "
               "timeNoti LIKE '%" + search + "%' OR "
               "titleNoti LIKE '%" + search + "%' OR "
               "bodyNoti LIKE '%" + search + "%') " + type +
               "ORDER BY timeNoti DESC");       //Lấy dữ liệu yêu cầu mượn sách theo tìm kiếm search, danh mục type
    queryModelNoti.setQuery(query);
    ui->notiTableView->resizeColumnsToContents();
    ui->notiTableView->resizeRowsToContents();
    ui->editNotiButton->setDisabled(true);
    ui->deleteNotiButton->setDisabled(true);
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro'");  //Đếm số thông báo chưa đọc
    query.first();
    ui->tabWidget->setTabText(2,"Thông báo (" + query.value(0).toString() + ")");
    ui->typeNotiComboBox->setItemText(1, "Chưa đọc (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti == 'Đề nghị mua tài liệu'");  //Đếm số thông báo chưa đọc thuộc đề nghị
    query.first();
    ui->typeNotiComboBox->setItemText(3, "Đề nghị mua tài liệu (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti == 'Yêu cầu hỗ trợ'");    //Đếm số thông báo chưa đọc thuộc yêu cầu hỗ trợ
    query.first();
    ui->typeNotiComboBox->setItemText(4, "Yêu cầu hỗ trợ (" + query.value(0).toString() + ")");
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = 'LibPro' AND titleNoti == 'Góp ý'"); //Đếm số yêu cầu chưa đọc thuộc Góp ý
    query.first();
    ui->typeNotiComboBox->setItemText(5, "Góp ý (" + query.value(0).toString() + ")");
}

QString LibrarianForm::getMaThongBao()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),0,index).data().toString();
}

void LibrarianForm::on_deleteNotiButton_clicked()
{
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    if (getFromUser() == "Quản lý người dùng")  //Không thể xóa thông báo của quản lý
    {
        QMessageBox::critical(this, "Thông báo", "Không thể xóa thông báo gửi bởi 'Quản lý người dùng'");
        return;
    }
    query.prepare("DELETE FROM noti WHERE maThongBao = :maThongBao");   //Xóa thông báo
    query.bindValue(":maThongBao", getMaThongBao());
    if (query.exec())
        queryNoti();
    else
        QMessageBox::critical(this, "Lỗi", "Thao tác thất bại");
}

void LibrarianForm::on_sendNotiButton_clicked()
{
    extern NotiForm *notiForm;
    if (checkSelectedNoti())
    {
        QString fromUser = getFromUser();
        if (fromUser != "Thủ thư" && fromUser != "Quản lý người dùng")
            notiForm->start(GUI_THONG_BAO, "Thủ thư", fromUser);    //Gửi thông báo tới người dùng nào đó đã chọn
    }
    else
        notiForm->start(GUI_THONG_BAO, "Thủ thư");  //Gửi thông báo
}

void LibrarianForm::on_editNotiButton_clicked()
{
    extern NotiForm *notiForm;
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    if (getFromUser() == "Thủ thư")
        notiForm->start(CHINH_SUA_THONG_BAO, getMaThongBao());  //Chỉnh sửa thông báo của thủ thư
    else
    {
        notiForm->start(XEM_THONG_BAO, getMaThongBao());
        if (getFromUser() != "Quản lý người dùng")
        {
            query.exec("UPDATE noti SET daDoc = 1 WHERE maThongBao = '" + getMaThongBao() + "'");   //Cập nhật thông báo
            queryNoti();
        }
    }
}

bool LibrarianForm::checkSelectedNoti()
{
    return ui->notiTableView->selectionModel()->selectedRows().count();
}

bool LibrarianForm::checkSelectedBook()
{
    return ui->bookTableView->selectionModel()->selectedRows().count();
}

bool LibrarianForm::checkSelectedRequest()
{
    return ui->requestTableView->selectionModel()->selectedRows().count();
}

void LibrarianForm::on_notiTableView_pressed(const QModelIndex &index)
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

void LibrarianForm::querySetting()
{
    query.exec("SELECT * FROM parameter");  //Lấy dữ liệu cài đặt
    query.first();
    ui->spinBox_0->setValue(query.value(0).toInt());
    ui->spinBox_1->setValue(query.value(1).toInt());
    ui->spinBox_2->setValue(query.value(2).toInt());
    ui->spinBox_3->setValue(query.value(3).toInt());
    ui->spinBox_4->setValue(query.value(4).toInt());
    ui->spinBox_5->setValue(query.value(5).toInt());
    ui->spinBox_6->setValue(query.value(6).toInt());
}

void LibrarianForm::on_updateSettingButton_clicked()
{
    query.prepare("UPDATE parameter SET "
                  "soSachToiDaSV = :soSachToiDaSV,"
                  "soSachToiDaGVCNVC = :soSachToiDaGVCNVC,"
                  "phiPhatNgay = :phiPhatNgay,"
                  "soQuyenToiDaMotTen = :soQuyenToiDaMotTen,"
                  "heSoPhatMatSach = :heSoPhatMatSach,"
                  "soNgayChoMuonToiDa = :soNgayChoMuonToiDa,"
                  "soNgayQuaHanToiDa = :soNgayQuaHanToiDa");        //Cập nhật cài đặt
    query.bindValue(":soSachToiDaSV", ui->spinBox_0->value());
    query.bindValue(":soSachToiDaGVCNVC", ui->spinBox_1->value());
    query.bindValue(":phiPhatNgay", ui->spinBox_2->value());
    query.bindValue(":soQuyenToiDaMotTen", ui->spinBox_3->value());
    query.bindValue(":heSoPhatMatSach", ui->spinBox_4->value());
    query.bindValue(":soNgayChoMuonToiDa", ui->spinBox_5->value());
    query.bindValue(":soNgayQuaHanToiDa", ui->spinBox_6->value());
    if (query.exec())
        QMessageBox::information(this, "Thông báo", "Cập nhật cài đặt thành công");
    else
        QMessageBox::critical(this, "Thông báo", "Cập nhật cài đặt thất bại");
}

void LibrarianForm::on_typeNotiComboBox_currentTextChanged(const QString &arg1)
{
    queryNoti();
}

void LibrarianForm::on_searchNotiButton_clicked()
{
    queryNoti();
}
