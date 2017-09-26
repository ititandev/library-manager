#include "loginform.h"
#include "ui_loginform.h"


LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    start();    //Khởi chạy
    queryModel.setHeaderData(0, Qt::Horizontal, "Mã sách");         //Gán tiêu đề cột
    queryModel.setHeaderData(1, Qt::Horizontal, "Tên sách");
    queryModel.setHeaderData(2, Qt::Horizontal, "Danh mục");
    queryModel.setHeaderData(3, Qt::Horizontal, "Chủ đề");
    queryModel.setHeaderData(4, Qt::Horizontal, "Tên tác giả");
    queryModel.setHeaderData(5, Qt::Horizontal, "Nhà xuất bản");
    queryModel.setHeaderData(6, Qt::Horizontal, "Xuất bản");
    queryModel.setHeaderData(7, Qt::Horizontal, "Đã mượn");
    queryModel.setHeaderData(8, Qt::Horizontal, "Cho mượn");

    ui->bookTableView->setModel(&queryModel);   //Gán Model với TableView
    ui->bookTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bookTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bookTableView->resizeColumnsToContents();       //Điều chỉnh kích thước cột hàng
    ui->bookTableView->resizeRowsToContents();

    ui->bookTableView->setColumnWidth(1,200);   //Set độ rộng cột
    ui->bookTableView->setColumnWidth(2,80);
    ui->bookTableView->setColumnWidth(4,130);
    ui->bookTableView->setColumnWidth(5,120);

}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::preQuery()
{

    QSqlQuery localQuery;
    //Trả lại sách chờ mượn quá hạn
    query.prepare("SELECT bookBorrow.soLuong, bookBorrow.maSach FROM bookBorrow "
                  "WHERE julianday(date('now')) - julianday(bookBorrow.thoiGianYeuCau) > "
                  "(SELECT parameter.soNgayChoMuonToiDa FROM parameter) AND "
                  "bookBorrow.trangThai = :trangThai");
    query.bindValue(":trangThai", CHO_MUON);
    query.exec();
    while (query.next())
    {
    localQuery.prepare("UPDATE book SET soSachDaMuon = soSachDaMuon - :soLuong WHERE maSach = :maSach");
    localQuery.bindValue(":soLuong", query.value(0).toInt());
    localQuery.bindValue(":maSach", query.value(1).toString());
    localQuery.exec();
    }

    //Xóa chờ mượn quá hạn
    query.prepare("DELETE FROM bookBorrow "
                  "WHERE julianday(date('now')) - julianday(bookBorrow.thoiGianYeuCau) > "
                  "(SELECT parameter.soNgayChoMuonToiDa FROM parameter) AND "
                  "bookBorrow.trangThai = :trangThai");
    query.bindValue(":trangThai", CHO_MUON);
    query.exec();

    //Khóa người dùng quá hạn trả sách
    query.prepare("SELECT bookBorrow.userName FROM bookBorrow, parameter "
                  "WHERE (julianday(date('now')) - julianday(thoiGianHetHan)) "
                  "> parameter.soNgayQuaHanToiDa AND bookBorrow.trangThai = :trangThai");
    query.bindValue(":trangThai", DANG_MUON);
    query.exec();
    while (query.next())
    {
    localQuery.prepare("UPDATE user SET trangThai = :trangThai WHERE userName = :userName");
    localQuery.bindValue(":trangThai", BI_KHOA);
    localQuery.bindValue(":userName", query.value(0).toInt());
    localQuery.exec();
    }
}

void LoginForm::start()
{
    show();
    ui->userNameEdit->clear();      //Chuẩn bị form
    ui->passWordEdit->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    ui->sortComboBox->setCurrentIndex(0);
    ui->sortModeComboBox->setCurrentIndex(1);
    ui->sortModeComboBox->setDisabled(true);
    preQuery();     //Truy xuất đầu chương trình
    queryBook();    //Lấy dữ liệu bảng sách

    extern QString userName;
    userName = QString();
}

QString LoginForm::selectColumn(int choice)
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



void LoginForm::on_searchButton_clicked()
{
    queryBook();
}

void LoginForm::on_viewAllButton_clicked()
{
    ui->searchBox->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->searchComboBox->setCurrentIndex(0);
    queryBook();
}

void LoginForm::queryBook()
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
    ui->bookTableView->setColumnWidth(1,0.35*ui->bookTableView->size().width());
    ui->bookTableView->setColumnWidth(2,100);
    ui->bookTableView->setColumnWidth(4,0.15*ui->bookTableView->size().width());
    ui->bookTableView->setColumnWidth(5,0.1*ui->bookTableView->size().width());
    ui->viewMoreButton->setDisabled(true);
}

void LoginForm::on_typeComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void LoginForm::on_searchComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void LoginForm::on_sortComboBox_currentIndexChanged(int index)
{
    if (index == 0)
        ui->sortModeComboBox->setDisabled(true);
    else
        ui->sortModeComboBox->setDisabled(false);
    queryBook();
}

void LoginForm::on_sortModeComboBox_currentIndexChanged(int index)
{
    queryBook();
}

void LoginForm::on_loginButton_clicked()
{
    QString userNameEdit, passWordEdit;
    int role;
    extern ReaderForm *readerForm;
    extern LibrarianForm *librarianForm;
    extern UserManagerForm *userManagerForm;
    userNameEdit = ui->userNameEdit->text();
    passWordEdit = ui->passWordEdit->text();
    passWordEdit = QString(QCryptographicHash::hash(passWordEdit.toUtf8(),QCryptographicHash::Sha256).toHex());
    if (query.exec("SELECT * FROM user WHERE userName='"+userNameEdit+
                        "' and passWord = '"+passWordEdit+"'"))         //Kiểm tra người dùng có userName và passWord đã nhập
    {
        if (query.first()  && (!query.next()))      //Chỉ có duy nhất 1 người dùng như vậy
        {
            query.first();
            switch (query.value(0).toInt())
            {
            case CHO_XET_DUYET:
                QMessageBox::information(this, "Thông báo", "Tài khoản đã được đăng ký và đang trong quá trình xét duyệt");
                break;
            case BI_KHOA:
                QMessageBox::information(this, "Thông báo", "Tài khoản của bạn đã bị khóa");
                break;
            default:
                role = query.value(1).toInt();
                readerForm->active = false;
                librarianForm->active = false;
                userManagerForm->active = false;
                readerForm->setRole(role);
                librarianForm->setRole(role);
                userManagerForm->setRole(role);
                extern QString userName;
                extern bool isSV;
                userName = query.value(2).toString();
                (query.value(13) == "SV")?(isSV = true):(isSV = false);
                if (role & DOC_GIA)
                {
                    readerForm->start();
                    hide();
                    break;
                }
                else if (role & THU_THU)
                {
                    librarianForm->start();
                    hide();
                    break;
                }
                else if (role & QUAN_LY_NGUOI_DUNG)
                {
                    userManagerForm->start();
                    hide();
                    break;
                }
                break;
            }
        }
        else
            QMessageBox::information(this,"login", "Đăng nhập thất bại\nKiểm tra lại tài khoản & mật khẩu");
    }
}

void LoginForm::on_signUpButton_clicked()
{
    extern UserForm *userForm;
    userForm->start(DANG_KY_TK);    //Đăng ký tài khoản
}

void LoginForm::on_noiQuyButton_clicked()
{
    QMessageBox::information(this, "Nội quy thư viện", "\t\t\t\tNỘI QUY THƯ VIỆN\n\n\n"
                             "\t1.Đối tượng được mượn sách về nhà bao gồm: Sinh viên, học viên cao học, giảng viên, CBVC đã đăng ký và được Thư viện cấp quyền sử dụng.\n\n"
                             "\t2.Bạn đọc sử dụng thẻ (thẻ SV, học viên, Giảng viên, CBVC) của trường (nơi bạn đọc đang học tập, giảng dạy, công tác,…) cấp và đã được Thư viện ĐHBK cấp quyền sử dụng để làm thủ tục mượn và trả sách tại phòng đọc và mượn sách về nhà. Nghiêm cấm sử dụng thẻ của người khác để mượn sách, hoặc cho người khác mượn thẻ.\n\n"
                             "\t4.Bạn đọc được mượn tối đa là 04 tên/04 cuốn cho mỗi lần mượn. Thời gian mượn tối đa là 02 tuần và được phép gia hạn thêm 02 tuần (đối với Giảng viên là 05 tên/05 cuốn/3 tuần). Quá thời hạn mượn theo Quy định, bạn đọc phải nộp tiền phạt 1000đ/1cuốn/ngày (trừ ngày nghỉ Lễ, Tết)\n\n"
                             "\t5.Bạn đọc cần kiểm tra kỹ tình trạng của sách và báo lại cho thủ thư trước khi mang sách ra khỏi phòng mượn. Nếu thấy sách có dấu hiệu bị rách, mất trang, mất mã vạch, bị ghi chép,… mà không báo thì vẫn bị phạt theo Quy định của Thư viện.\n\n"
                             "\t6.Đề nghị bạn đọc mượn sách về nhà giữ gìn cẩn thận. Trường hợp vi phạm: làm rách, làm ướt, mất trang, mất mã vạch, ghi chép lên sách,… bị xử phạt theo Quy định của Thư viện (nộp phạt chi phí phục hồi, tái tạo hoặc đền lại sách mới)\n\n"
                             "\t7.Trường hợp đánh mất sách, bạn đọc phải mua đền lại sách bị mất hoặc đền bù 3 lần giá sách ghi ở bìa sau.\n\n"
                             "\t8.Nếu bạn đọc mượn sách quá hạn từ 10 ngày trở lên, Thư viện sẽ dán danh sách tại phòng mượn. Đầu mỗi học kỳ, Thư viện sẽ thống kê những bạn đọc quá hạn từ 16 tuần trở lên và gửi danh sách đề nghị P. Đào Tạo không cấp phiếu ĐKMH của học kỳ tiếp theo.\n\n"
                             "\t9.Bạn đọc phải hoàn tất việc thanh toán các khoản nợ với Thư viện trước khi ra trường, chuyển trường,…(P. Đào tạo sẽ gửi danh sách để Thư viện xác nhận)\n\n"
                             "\t10.Thời gian mượn trả sách: tất cả các ngày trong tuần từ thứ hai đến thứ Bảy (trừ  ngày Chủ nhật và những ngày nghỉ Lễ)\n\n"
                             "<Tham khảo nội quy thư viện Đại học Bách Khoa Thành Phố Hồ Chí Minh>");
}

void LoginForm::on_viewMoreButton_clicked()
{
    if (ui->bookTableView->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::information(this, "Thông báo", "Bạn phải chọn sách để xem");
        return;
    }
    QModelIndex index = ui->bookTableView->selectionModel()->selectedRows().at(0);
    QString maSach = ui->bookTableView->model()->sibling(index.row(),0,index).data().toString();
    extern BookForm *bookForm;
    bookForm->start(XEM_THEM, maSach);      //Xem thông tin chi tiết sách
}

void LoginForm::on_bookTableView_pressed(const QModelIndex &index)
{
   if (ui->bookTableView->selectionModel()->selectedRows().count() == 1)
       ui->viewMoreButton->setDisabled(false);
   else
       ui->viewMoreButton->setDisabled(true);
}

void LoginForm::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
        LoginForm::on_loginButton_clicked();    //Sự kiện nhấn phím enter -> đăng nhập
}

void LoginForm::on_gopYButton_clicked()
{
    extern NotiForm *notiForm;
    notiForm->start(GOP_Y);     //Góp ý
}

void LoginForm::on_muaTaiLieuButton_clicked()
{
    extern NotiForm *notiForm;
    notiForm->start(DE_NGHI_MUA_SACH);  //Đền nghị mua sách
}

