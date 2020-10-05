#ifndef ADMIN_H
#define ADMIN_H
#include "Course.h"
#include "Student.h"
#include <iostream>
#include <cstdio>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QVector>
#include <QMessageBox>
#include <QTextStream>
using namespace std;
#define cout qDebug()<<"["<<__FILE__<<":"<<__LINE__<<"]"
class Admin
{
public:
    Admin();
    ~Admin();
    Admin(QString cid, QString cpassword);
    bool Login(const QString cid, const QString cpassword); //管理员登陆
    bool Logout(); //管理员登出
    QString EnterCourses(QString filename);//导入课程的函数
    QString AddCourses(Course cour); //加入课程
    QString DeleteCourses(int cid); //删除课程
    QString ModifyCourses(int cid,QString teacher,QString cca,QString cno,int y); //改变课程
    friend class LoginDialog;
    friend class TeacherWindow;
    friend class StudentWindow;

private:
    QString id; //存放管理员id
    QString password; //存放管理员密码
    bool isFirst;  //判断管理员是否是第一次登陆
    bool ifLogin;  //判断管理员是否登陆
    int nc;  //课程的数量
    QVector<Course> c;  //存放课程信息
    QVector<int> nuass;//每门课程的助教的数量
    QVector<QVector<QString>> ass;//每个课程的相对应的助教
    bool if_locked; //判断管理员账号是否锁住
};

#endif // ADMIN_H
