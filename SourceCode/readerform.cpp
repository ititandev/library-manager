#include "readerform.h"
#include "ui_readerform.h"

ReaderForm::ReaderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderForm)
{
    ui->setupUi(this);
    queryBook();        //Lấy dữ liệu bảng sách
    queryModel.setHeaderData(0, Qt::Horizontal, "Mã sách");     //Set tiêu đề cột
    queryModel.setHeaderData(1, Qt::Horizontal, "Tên sách");
    queryModel.setHeaderData(2, Qt::Horizontal, "Danh mục");
    queryModel.setHeaderData(3, Qt::Horizontal, "Chủ đề");
    queryModel.setHeaderData(4, Qt::Horizontal, "Tên tác giả");
    queryModel.setHeaderData(5, Qt::Horizontal, "Nhà xuất bản");
    queryModel.setHeaderData(6, Qt::Horizontal, "Năm xuất bản");
    queryModel.setHeaderData(7, Qt::Horizontal, "Đã mượn");
    queryModel.setHeaderData(8, Qt::Horizontal, "Cho mượn");
    ui->bookTableView->setModel(&queryModel);       //Gán Model vào TableView
    ui->bookTableView->setSelectionBehavior(QAbstractItemView::SelectRows);     //Chế độ chọn hàng
    ui->bookTableView->setSelectionMode(QAbstractItemView::SingleSelection);    //Chế độ chọn đơn
    ui->bookTableView->resizeColumnsToContents();   //Điều chỉnh kích thước cột
    ui->bookTableView->resizeRowsToContents();      //Điều chỉnh kích thước hàng


    ui->cartTableView->setModel(&queryModelCart);
    ui->cartTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->cartTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->borrowTableView->setModel(&queryModelBorrow);
    ui->borrowTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->borrowTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->historyTableView->setModel(&queryModelHistory);
    ui->historyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->historyTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->notiTableView->setModel(&queryModelNoti);
    ui->notiTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->notiTableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

ReaderForm::~ReaderForm()
{
    delete ui;
}

void ReaderForm::setRole(int role)
{
    ui->roleComboBox->clear();
    if (role & DOC_GIA)
        ui->roleComboBox->addItem("Độc giả");   //Thêm mục Độc giả
    if (role & THU_THU)
        ui->roleComboBox->addItem("Thủ thư");   //Thêm mục Thủ thư
    if (role & QUAN_LY_NGUOI_DUNG)
        ui->roleComboBox->addItem("Quản lý người dùng");    //Thêm mục Quản lý người dùng
}

void ReaderForm::start()
{
    active = true;
    //Cài đặt ban đầu
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    ui->sortComboBox->setCurrentIndex(0);
    ui->sortModeComboBox->setCurrentIndex(1);
    ui->sortModeComboBox->setDisabled(true);
    queryBook();        //Lấy dữ liệu bảng sách
    ui->roleComboBox->setCurrentText("Độc giả");
    extern QString userName;
    ui->user->setText(userName);

    getCart();  //Lấy dữ liệu giỏ sách
    queryModelCart.setHeaderData(0, Qt::Horizontal, "Mã số");
    queryModelCart.setHeaderData(1, Qt::Horizontal, "Mã sách");
    queryModelCart.setHeaderData(2, Qt::Horizontal, "Tên sách");
    queryModelCart.setHeaderData(3, Qt::Horizontal, "Tác giả");
    queryModelCart.setHeaderData(4, Qt::Horizontal, "Số lượng");
    ui->cartTableView->resizeColumnsToContents();
    ui->cartTableView->resizeRowsToContents();

    queryBorrow();  //Lấy dữ liệu mượn sách
    queryModelBorrow.setHeaderData(0, Qt::Horizontal, "Mã số");
    queryModelBorrow.setHeaderData(1, Qt::Horizontal, "Mã sách");
    queryModelBorrow.setHeaderData(2, Qt::Horizontal, "Tên sách");
    queryModelBorrow.setHeaderData(3, Qt::Horizontal, "Tác giả");
    queryModelBorrow.setHeaderData(4, Qt::Horizontal, "Số lượng");
    queryModelBorrow.setHeaderData(5, Qt::Horizontal, "Ngày yêu cầu");
    queryModelBorrow.setHeaderData(6, Qt::Horizontal, "Ngày mượn");
    queryModelBorrow.setHeaderData(7, Qt::Horizontal, "Ngày trả");
    queryModelBorrow.setHeaderData(8, Qt::Horizontal, "Trạng thái");
    ui->borrowTableView->resizeColumnsToContents();
    ui->borrowTableView->resizeRowsToContents();

    queryHistory(); //Lấy dữ liệu bảng lịch sử mượn sách
    queryModelHistory.setHeaderData(0, Qt::Horizontal, "Mã số");
    queryModelHistory.setHeaderData(1, Qt::Horizontal, "Mã sách");
    queryModelHistory.setHeaderData(2, Qt::Horizontal, "Tên sách");
    queryModelHistory.setHeaderData(3, Qt::Horizontal, "Tác giả");
    queryModelHistory.setHeaderData(4, Qt::Horizontal, "Số lượng");
    queryModelHistory.setHeaderData(5, Qt::Horizontal, "Ngày yêu cầu");
    queryModelHistory.setHeaderData(6, Qt::Horizontal, "Ngày mượn");
    queryModelHistory.setHeaderData(7, Qt::Horizontal, "Ngày hết hạn");
    queryModelHistory.setHeaderData(8, Qt::Horizontal, "Ngày trả");
    queryModelHistory.setHeaderData(9, Qt::Horizontal, "Trạng thái");
    queryModelHistory.setHeaderData(10, Qt::Horizontal, "Phí phạt");
    queryModelHistory.setHeaderData(11, Qt::Horizontal, "Chú thích");
    ui->historyTableView->resizeColumnsToContents();
    ui->historyTableView->resizeRowsToContents();

    ui->searchNotiBox->clear();
    queryNoti();    //Lấy dữ liệu thông báo
    queryModelNoti.setHeaderData(0, Qt::Horizontal, "Mã thông báo");
    queryModelNoti.setHeaderData(1, Qt::Horizontal, "Từ");
    queryModelNoti.setHeaderData(2, Qt::Horizontal, "Tới");
    queryModelNoti.setHeaderData(3, Qt::Horizontal, "Thời gian");
    queryModelNoti.setHeaderData(4, Qt::Horizontal, "Tiêu đề");
    queryModelNoti.setHeaderData(5, Qt::Horizontal, "Nội dung");
    ui->notiTableView->resizeColumnsToContents();
    ui->notiTableView->resizeRowsToContents();

    show();
    if (isPunished())
    {
        QMessageBox::critical(this, "Thông báo", "Bạn đang bị phạt vì quá hạn trả sách");
        ui->tabWidget->setCurrentIndex(1);
    }
    else if (checkDeadline())
    {
        QMessageBox::critical(this, "Thông báo", "Bạn có sách sắp tới hạn trả");
    }

    LoginForm::writeHistory("Đăng nhập chức năng Độc giả");
}

void ReaderForm::getCart()
{
    extern QString userName;
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE userName = '"+ userName + "' AND "
                     "trangThai = " + GIO_HANG);        //Lấy số yêu cầu trong giỏ sách
    query.first();
    int num = query.value(0).toInt();
    ui->tabWidget->setTabText(1, "Giỏ sách (" + QString::number(num) + ") và Mượn sách");
    query.exec("SELECT bookBorrow.maSo, bookBorrow.maSach, book.tenSach, book.tenTacGia, bookBorrow.soLuong "
                     "FROM bookBorrow, book WHERE bookBorrow.maSach = book.maSach "
                     "AND bookBorrow.userName = '"+userName+"' AND trangThai = " + GIO_HANG);       //Lấy yêu cầu giỏ sách từ database
    queryModelCart.setQuery(query);
    ui->cartTableView->resizeColumnsToContents();
    ui->cartTableView->resizeRowsToContents();
}

QString ReaderForm::selectColumn(int choice)
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

void ReaderForm::on_roleComboBox_currentTextChanged(const QString &arg1)
{
    extern LibrarianForm *librarianForm;
    extern UserManagerForm *userManagerForm;
    if (active)
    {
        if (arg1 == "Thủ thư")
        {
            librarianForm->start();
            hide();
        }
        if (arg1 == "Quản lý người dùng")
        {
            userManagerForm->start();
            hide();
        }
    }

}

void ReaderForm::on_logoutForm_clicked()
{
    extern LoginForm *loginForm;
    hide();
    loginForm->start();
    LoginForm::writeHistory("Đăng xuất");
}

void ReaderForm::on_user_clicked()
{
    extern UserForm *userForm;
    userForm->start(1);

}

void ReaderForm::on_searchButton_clicked()
{
    queryBook();
}

void ReaderForm::on_viewAllButton_clicked()
{
    ui->searchBox->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    queryBook();
}

void ReaderForm::queryBook()
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
               "FROM book WHERE " + type + " (" + search + ") " + order);       //Lấy dữ liệu sách theo danh mục type, tìm kiếm search, thứ tự order
    queryModel.setQuery(query);
    ui->bookTableView->resizeColumnsToContents();
    ui->bookTableView->resizeRowsToContents();
    ui->bookTableView->setColumnWidth(1,250);
    ui->bookTableView->setColumnWidth(2,120);
    ui->bookTableView->setColumnWidth(4,180);
    ui->bookTableView->setColumnWidth(5,180);
}

void ReaderForm::on_sortComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        ui->sortModeComboBox->setDisabled(true);
    else
        ui->sortModeComboBox->setDisabled(false);
    queryBook();
}

void ReaderForm::on_sortModeComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void ReaderForm::on_typeComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void ReaderForm::on_searchComboBox_currentIndexChanged(int index)
{
    queryBook();
}

bool ReaderForm::isPunished()
{
    extern QString userName;
    query.prepare("SELECT COUNT(*) FROM bookBorrow WHERE userName = :userName AND "                 //Kiểm tra số yêu cầu bị phạt
                  "(julianday('now') > julianday(thoiGianHetHan)) AND trangThai = :trangThai");
    query.bindValue(":userName", userName);
    query.bindValue(":trangThai", DANG_MUON);
    query.exec();
    query.first();
    return query.value(0).toInt();
}

bool ReaderForm::checkDeadline()
{
    extern QString userName;
    query.prepare("SELECT COUNT(*) FROM bookBorrow WHERE userName = :userName AND "                         //Kiểm tra số yêu cầu sắp hết hạn
                  "julianday(thoiGianHetHan) - julianday(date('now')) <= 3 AND trangThai = :trangThai");
    query.bindValue(":userName", userName);
    query.bindValue(":trangThai", DANG_MUON);
    query.exec();
    query.first();
    return query.value(0).toInt();
}

void ReaderForm::on_borrowButton_clicked()
{
    QDate::currentDate();
    extern QString userName;
    extern bool isSV;
    int soSachToiDaSV;
    int soSachToiDaGV;
    int soQuyenToiDa;
    bool ok;

    if (ui->bookTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để mượn");
        return;
    }
    if (isPunished())
    {
        QMessageBox::critical(this, "Thông báo", "Bạn đang bị phạt\nVui lòng nộp phạt để tiếp tục sử dụng thư viện");
        return;
    }

    QModelIndex index = ui->bookTableView->selectionModel()->selectedRows().at(0);
    QString maSach = ui->bookTableView->model()->sibling(index.row(),0,index).data().toString();
    query.exec("SELECT choMuon FROM book WHERE maSach = '" + maSach + "'");     //Kiểm tra cho mượn
    query.first();
    if (!query.value(0).toBool())
    {
        QMessageBox::critical(this, "Thông báo", "Sách " + maSach + " không được phép mượn\nLiên hệ thủ thư để biết thêm chi tiết");
        return;
    }
    query.prepare("SELECT COUNT(*) FROM bookBorrow WHERE userName = :userName AND maSach = :maSach AND trangThai < :trangThai");        //Kiểm tra số sách đã lấy
    query.bindValue(":userName", userName);
    query.bindValue(":maSach", maSach);
    query.bindValue(":trangThai", DA_MUON);
    query.exec();
    query.first();
    if (query.value(0).toInt())
    {
        QMessageBox::critical(this, "Thông báo", "Bạn đang mượn cuốn sách này hoặc sách đã có trong giỏ sách của bạn");
        return;
    }

    //Get Parameter
    query.exec("SELECT * FROM parameter");
    query.first();
    soSachToiDaSV = query.value(0).toInt();
    soSachToiDaGV = query.value(1).toInt();
    soQuyenToiDa = query.value(3).toInt();

    int soLuong = QInputDialog::getInt(this, "Mượn sách", "Nhập số lượng sách " + maSach + " bạn muốn mượn:", 1, 1, soQuyenToiDa, 1, &ok);

    if (ok)
    {
        if ( getRemainBook(maSach) >= soLuong)
        {
            if (getBorrowedNum(userName) < isSV*soSachToiDaSV + (!isSV) * soSachToiDaGV)
            {
                query.prepare("INSERT INTO bookBorrow (maSo, userName, maSach, soLuong, trangThai) "                //Thêm yêu cầu giỏ hàng vào cơ sở dữ liệu
                              "SELECT MAX(maSo) + 1, :userName, :maSach, :soLuong, :trangThai FROM bookBorrow");
                query.bindValue(":userName", userName);
                query.bindValue(":maSach", maSach);
                query.bindValue(":soLuong", QString::number(soLuong));
                query.bindValue(":trangThai", GIO_HANG);
                if (query.exec())
                {
                    QMessageBox::information(this, "Thông báo", "Bạn đã bỏ vào giỏ sách thành công cuốn " + maSach);
                    getCart();
                }
                else
                    QMessageBox::critical(this, "Thông báo", "Thao tác thất bại");
            }
            else
            {
                QMessageBox::critical(this,"Thông báo", "Bạn đã mượn quá số sách cho phép");
            }
        }
        else
        {
            QMessageBox::critical(this, "Thông báo", "Sách bạn chọn không đủ số lượng để bạn mượn");
        }
    }
}

void ReaderForm::on_viewMoreButton_clicked()
{
    if (ui->bookTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để xem");
        return;
    }
    QModelIndex index = ui->bookTableView->selectionModel()->selectedRows().at(0);
    QString maSach = ui->bookTableView->model()->sibling(index.row(),0,index).data().toString();
    extern BookForm *bookForm;
    bookForm->start(XEM_THEM, maSach);
}

void ReaderForm::on_deleteCartButton_clicked()
{
    if (ui->cartTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để xóa");
        return;
    }
    QModelIndex index = ui->cartTableView->selectionModel()->selectedRows().at(0);
    QString maSo = ui->cartTableView->model()->sibling(index.row(),0,index).data().toString();
    query.prepare("DELETE FROM bookBorrow WHERE maSo = :maSo");
    query.bindValue(":maSo", maSo);
    if (query.exec())
    {
        QMessageBox::information(this, "Thông báo", "Xóa thành công khỏi giỏ hàng");
        getCart();
    }
}

void ReaderForm::on_requestButton_clicked()
{
    extern QString userName;
    if (ui->cartTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để mượn");
        return;
    }
    if (isPunished())
    {
        QMessageBox::critical(this, "Thông báo", "Bạn đang bị phạt\nVui lòng nộp phạt để tiếp tục sử dụng thư viện");
        return;
    }
    QModelIndex index = ui->cartTableView->selectionModel()->selectedRows().at(0);
    QString maSo = ui->cartTableView->model()->sibling(index.row(),0,index).data().toString();
    QString maSach = ui->cartTableView->model()->sibling(index.row(), 1, index).data().toString();
    int soLuong = ui->cartTableView->model()->sibling(index.row(), 4, index).data().toInt();

    if ( getRemainBook(maSach) >= soLuong)
    {
        query.prepare("UPDATE bookBorrow SET trangThai = :trangThai, thoiGianYeuCau = :thoiGian "       //Cập nhật trạng thái chờ mượn và thời gian yêu cầu
                            "WHERE maSo = :maSo");
        query.bindValue(":trangThai", CHO_MUON);
        query.bindValue(":thoiGian", QDate::currentDate().toString("yyyy-MM-dd"));
        query.bindValue(":maSo", maSo);
        if (query.exec())
        {
            QMessageBox::information(this, "Thông báo", "Yêu cầu mượn thành công sách\n Vui lòng mang thẻ SV/GV đến thư viện để lấy sách");
            query.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon + :soLuong WHERE maSach = :maSach");     //Tăng số sách đã mượn
            query.bindValue(":soLuong", soLuong);
            query.bindValue(":maSach", maSach);
            query.exec();
            getCart();
            queryBook();
            queryBorrow();
        }
        else
            QMessageBox::critical(this, "Thông báo", "Yêu cầu mượn thất bại");
    }
    else
    {
        QMessageBox::critical(this, "Thông báo", "Sách bạn chọn không đủ số lượng để bạn mượn");
    }
}

int ReaderForm::getRemainBook(QString maSach)
{
    query.exec("SELECT * FROM book WHERE maSach = '" + maSach+"'");     //Lấy số sách còn lại
    query.first();
    return query.value(8).toInt() - query.value(9).toInt();
}

int ReaderForm::getBorrowedNum(QString userName)
{
    query.exec("SELECT COUNT(*) FROM bookBorrow WHERE userName = '" + userName + "' "
    "AND (trangThai = " + GIO_HANG + " OR trangThai = " + CHO_MUON + " OR trangThai = " + DANG_MUON + ")");     //Đếm số yêu cầu đã gửi và lấy
    query.first();
    return query.value(0).toInt();
}

void ReaderForm::queryBorrow()
{
    extern QString userName;
    query.prepare("SELECT bookBorrow.maSo, bookBorrow.maSach, book.tenSach, book.tenTacGia, "
                  "bookBorrow.soLuong, thoiGianYeuCau, thoiGianMuon, thoiGianHetHan, "
                  "CASE trangThai "
                  "WHEN -1 THEN 'Chờ mượn' "
                  "WHEN 0 THEN 'Đang mượn' END "
                  "AS trangThaiStr, "
                  "CASE "
                  "WHEN (date('now') > thoiGianHetHan) AND trangThai = :trangThai THEN "
                  "(julianday(date('now')) - julianday(thoiGianHetHan))*(parameter.phiPhatNgay) "
                  "ELSE NULL "
                  "END AS 'Phạt' "
                  "FROM book, bookBorrow, parameter "
                  "WHERE bookBorrow.maSach = book.maSach AND bookBorrow.userName = :userName AND "
                  "(trangThaiStr = 'Chờ mượn' OR trangThaiStr = 'Đang mượn') ORDER BY trangThaiStr");       //Lấy dữ liệu sách đã mượn và yêu cầu
    query.bindValue(":trangThai", DANG_MUON);
    query.bindValue(":userName", userName);
    query.exec();
    queryModelBorrow.setQuery(query);
    ui->borrowTableView->resizeColumnsToContents();
    ui->borrowTableView->resizeRowsToContents();
}

void ReaderForm::on_deleteRequestButton_clicked()
{
    extern QString userName;
    QModelIndex index;
    if (ui->borrowTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::critical(this, "Thông báo", "Không được phép\nChọn 1 bản ghi 'Chờ mượn'");
        return;
    }
    else
    {
        index = ui->borrowTableView->selectionModel()->selectedRows().at(0);
        if (ui->borrowTableView->model()->sibling(index.row(), 8, index).data().toString() != "Chờ mượn")
        {
            QMessageBox::critical(this, "Thông báo", "Không được phép\nChọn 1 bản ghi 'Chờ mượn'");
            return;
        }
    }
    QString maSo = ui->borrowTableView->model()->sibling(index.row(),0,index).data().toString();
    QString maSach = ui->borrowTableView->model()->sibling(index.row(),1,index).data().toString();
    int soLuong = ui->borrowTableView->model()->sibling(index.row(), 4, index).data().toInt();
    query.prepare("DELETE FROM bookBorrow WHERE maSo = :maSo");     //Xóa yêu cầu trong database
    query.bindValue(":maSo", maSo);
    if (query.exec())
    {
        QMessageBox::information(this, "Thông báo", "Xóa yêu cầu thành công");
        query.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon - :soLuong WHERE maSach = :maSach");         //Trả lại sách cho thư viện
        query.bindValue(":soLuong", soLuong);
        query.bindValue(":maSach", maSach);
        query.exec();
        queryBook();
        queryBorrow();
    }
    else
        QMessageBox::critical(this, "Lỗi", "Thao tác thất bại");
}

void ReaderForm::queryHistory()
{
    extern QString userName;
    query.prepare("SELECT bookBorrow.maSo, bookBorrow.maSach, book.tenSach, book.tenTacGia, "
                  "bookBorrow.soLuong, thoiGianYeuCau, thoiGianMuon, thoiGianHetHan, thoiGianTra, "
                  "CASE trangThai "
                  "WHEN 1 THEN 'Đã mượn' "
                  "WHEN 2 THEN 'Đã phạt' END "
                  "AS trangThaiStr, phat, chuThich "
                  "FROM book, bookBorrow "
                  "WHERE bookBorrow.maSach = book.maSach AND bookBorrow.userName = :userName AND "
                  "(trangThaiStr = 'Đã mượn' OR trangThaiStr = 'Đã phạt') ORDER BY thoiGianMuon DESC");     //Lấy dữ liệu lịch sử mượn sách
    query.bindValue(":userName", userName);
    query.exec();
    queryModelHistory.setQuery(query);
}

void ReaderForm::queryNoti()
{
    extern QString userName;
    QString search = ui->searchNotiBox->text();
    query.exec("SELECT maThongBao, fromUser, CASE WHEN toUser IS NULL THEN 'Tất cả' ELSE toUser END AS toUser2, "
               "timeNoti, titleNoti, bodyNoti, CASE daDoc WHEN 1 THEN 'Đã đọc' "
               "WHEN 0 THEN 'Chưa đọc' END AS 'Đã đọc' FROM noti "
               "WHERE (toUser = '" + userName + "' OR toUser IS NULL OR fromUser = '" + userName + "') AND "
               "(maThongBao LIKE '%" + search + "%' OR "
               "fromUser LIKE '%" + search + "%' OR "
               "toUser LIKE '%" + search + "%' OR "
               "toUser2 LIKE '%" + search + "%' OR "
               "timeNoti LIKE '%" + search + "%' OR "
               "titleNoti LIKE '%" + search + "%' OR "
               "bodyNoti LIKE '%" + search + "%') "
               "ORDER BY timeNoti DESC");               //Lấy dữ liệu thông báo
    queryModelNoti.setQuery(query);
    ui->notiTableView->resizeColumnsToContents();
    ui->notiTableView->resizeRowsToContents();
    query.exec("SELECT COUNT(*) FROM noti WHERE daDoc = 0 AND toUser = '" + userName + "'");
    query.first();
    ui->tabWidget->setTabText(3,"Thông báo (" + query.value(0).toString() + ")");
}

void ReaderForm::on_viewNotiButton_clicked()
{
    extern NotiForm *notiForm;
    extern QString userName;
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    notiForm->start(XEM_THONG_BAO, getMaThongBao());
    if (getToUser() == userName)
    {
        query.exec("UPDATE noti SET daDoc = 1 WHERE maThongBao = '" + getMaThongBao() + "'");       //Cập nhật thông báo thành đã đọc
        queryNoti();
    }
}

void ReaderForm::on_deleteNotiButton_clicked()
{
    if (!checkSelectedNoti())
    {
        QMessageBox::critical(this, "Thông báo", "Phải chọn 1 thông báo");
        return;
    }
    if (getToUser() == "Tất cả")
    {
        QMessageBox::critical(this, "Thông báo", "Không thể xóa thông báo gửi tới 'Tất cả'");
        return;
    }
    query.prepare("DELETE FROM noti WHERE maThongBao = :maThongBao");       //Xóa thông báo gửi tới user
    query.bindValue(":maThongBao", getMaThongBao());
    if (query.exec())
        queryNoti();
    else
        QMessageBox::critical(this, "Lỗi", "Thao tác thất bại");
}

QString ReaderForm::getMaThongBao()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),0,index).data().toString();
}

bool ReaderForm::checkSelectedNoti()
{
    return ui->notiTableView->selectionModel()->selectedRows().count();
}

QString ReaderForm::getToUser()
{
    QModelIndex index = ui->notiTableView->selectionModel()->selectedRows().at(0);
    return ui->notiTableView->model()->sibling(index.row(),2,index).data().toString();
}

void ReaderForm::on_hoTroButton_clicked()
{
    extern NotiForm *notiForm;
    extern QString userName;
    notiForm->start(YEU_CAU_HO_TRO, userName);
}

void ReaderForm::on_gopYButton_clicked()
{
    extern NotiForm *notiForm;
    extern QString userName;
    notiForm->start(GOP_Y, userName);
}

void ReaderForm::on_deNghiMuaSachButton_clicked()
{
    extern NotiForm *notiForm;
    extern QString userName;
    notiForm->start(DE_NGHI_MUA_SACH, userName);
}

void ReaderForm::on_searchNotiButton_clicked()
{
    queryNoti();
}
