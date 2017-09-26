#include "mainwindow.h"
#include "ui_mainwindow.h"

LoginForm *loginForm;
ReaderForm *readerForm;
LibrarianForm *librarianForm;
UserManagerForm *userManagerForm;
UserForm *userForm;
BookForm *bookForm;
NotiForm *notiForm;
ReturnBookForm *returnBookForm;

QString userName;
bool isSV;

MainWindow::MainWindow()
{
    //Open Database
    libProDB = QSqlDatabase::addDatabase("QSQLITE");
    //libProDB.setDatabaseName("C:/Users/ITitan/Desktop/build-LibPro-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/debug/LibProDB.db");
    libProDB.setDatabaseName("LibProDB.db");    //Mở database
    if (!libProDB.open())   //Kiểm tra đã mở
    {
        QMessageBox::critical(NULL,"Lỗi Kết nối","Kết nối thất bại đến cơ sở dữ liệu \n"
                                 "Vui lòng kiểm tra lại file dữ liệu và đặt cùng phần mềm\n"
                                 "Phần mềm sẽ tự thoát");
        exit(1);
    }
    LoginForm::writeHistory("Khởi động phần mềm");

    //Tạo các form
    extern LoginForm *loginForm;
    loginForm = new LoginForm;
    loginForm->start();
    extern ReaderForm *readerForm;
    readerForm = new ReaderForm;
    extern LibrarianForm *librarianForm;
    librarianForm = new LibrarianForm;
    extern UserManagerForm *userManagerForm;
    userManagerForm = new UserManagerForm;
    extern UserForm *userForm;
    userForm = new UserForm;
    extern BookForm *bookForm;
    bookForm = new BookForm;
    extern NotiForm *notiForm;
    notiForm = new NotiForm;
    extern ReturnBookForm *returnBookForm;
    returnBookForm = new ReturnBookForm;
}

MainWindow::~MainWindow()
{
    LoginForm::writeHistory("Thoát phần mềm");      //Ghi lại lịch sử
    libProDB.close();       //Đóng database
    //libProDB.removeDatabase(QSqlDatabase::defaultConnection);

    extern LoginForm *loginForm;        //Xóa các form
    delete loginForm;
    extern ReaderForm *readerForm;
    delete readerForm;
    extern LibrarianForm *librarianForm;
    delete librarianForm;
    extern UserManagerForm *userManagerForm;
    delete userManagerForm;
    extern UserForm *userForm;
    delete userForm;
    extern BookForm *bookForm;
    delete bookForm;
    extern NotiForm *notiForm;
    delete notiForm;
    extern ReturnBookForm *returnBookForm;
    delete returnBookForm;
}
