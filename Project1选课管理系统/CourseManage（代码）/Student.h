#ifndef STUDENT_H
#define STUDENT_H
#include<QString>
#include<QVector>

class Student
{
public:
    Student();
    ~Student();
    Student(QString cid, QString cpa);
    bool Login(QString cid, QString cpassword); //学生登陆
    bool Logout(); //学生登出
    bool choose(int cid);  //该学生是否选过id为cid的课程
    friend class Admin;
    friend class TeacherWindow;
    friend class LoginDialog;
    friend class StudentWindow;
    friend class RegisterDialog;


private:
    QString id; //该学生的id
    QString password; //该学生的密码
    int num_cho;//已经选择的课程的数量
    QVector<int> cho;//存放选择课程的id的数组
    QVector<QString> ass;//选择课程的对应的助教
    bool ifLogin;//是否登陆
    int zhuannum;//该学生选的专业课的数量
    int nozhuannum;//该学生选的非专业课数量
    int num_ass;//自己报名助教的数量
    int num_of_login;//该学生账号登陆的次数
    bool if_lock;//该学生账号是否被锁住
};

#endif // STUDENT_H
