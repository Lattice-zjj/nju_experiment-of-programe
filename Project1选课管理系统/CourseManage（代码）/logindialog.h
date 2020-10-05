#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include "teacherwindow.h"
#include "studentwindow.h"
#include "Admin.h"
#include "registerdialog.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QCompleter>
#include <QAction>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QToolBar>
#include <QButtonGroup>
#include <QPainter>
#include <time.h>
#include <QMessageBox>
#include <QDesktopServices>
#include <QString>
#include <QVector>
#include <QUrl>
#define cout qDebug()<<"["<<__FILE__<<":"<<__LINE__<<"]"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    bool judgeEmpty();//判断lineedit中是否为空
    void clearAll();//将所有的lineedit中清空
    void UpInfor();//从文件中读取数据到内存中

private slots: //所有槽函数
    void on_yanzhenglabel_clicked(); //对验证码的切换
    void loginbtnSetSlot(QString); //登陆键按下的槽函数
    void on_loginbtn_clicked(); //登陆键被按下
    void on_userline_textEdited(const QString &arg1); //userline内容发生改变时的操作
    void settimeline(); //设置时间
    void unlock(); //解锁
    void on_registerbtn_clicked(); //注册的按钮按下

protected:
    void paintEvent(QPaintEvent * );

signals:
    void toStudentWindow(Admin*,int*,Student*,int);
    void toTeacherWindow(Admin*,int*,Student*);
    void toRegisterWindow(Admin*,int*,Student*);

private:
    Ui::LoginDialog *ui;
    int num_yan;
    QButtonGroup *radiogroup;
    QVector<QString> history;
    QTimer *timelinetimer;
    QTimer *timer;
    TeacherWindow *teacher;
    StudentWindow *student;
    RegisterDialog *reg;
    int num_admin_login;
    Admin admin;
    int st_num;//学生的数量
    Student stud[100];//存放学生信息的数组
    int nowstu;//表示当前是哪一个学生

};

#endif // LOGINDIALOG_H
