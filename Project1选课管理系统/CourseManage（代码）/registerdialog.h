#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H
#include "studentwindow.h"
#include <QMainWindow>
#include <QMessageBox>
#include "Admin.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    void clearAll();
private slots:
    void fromLoginDialog(Admin*,int*,Student*);//从登陆界面接受到的信号
    void loginbtnSetSlot(QString);
    void on_register_s_clicked();//注册键按下
    void on_back_to_clicked();//返回键按下
    void settimeline();//设置时间
protected:
    void paintEvent(QPaintEvent *);//画图虚函数
signals:
    void toStudentWindow(Admin*,int*,Student*,int);//发给学生界面的
    void toLoginDialog();//发出给登陆界面的
private:
    Ui::RegisterDialog *ui;
    Admin *admin;
    int *st_num;
    Student *stud;
    StudentWindow *student;
    QTimer* timelinetimer;//计时器
};

#endif // REGISTERDIALOG_H
