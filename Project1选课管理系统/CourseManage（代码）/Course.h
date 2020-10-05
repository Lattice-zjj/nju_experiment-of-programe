#ifndef COURSE_H
#define COURSE_H
#include <QString>

class Course
{
public:
    Course();
    Course(int cid,QString cna,QString cte,int cca,int cse,QString ctp,QString cnote);
    ~Course();
    friend class TeacherWindow;
    friend class Admin;
    friend class LoginDialog;
    friend class StudentWindow;

private:
    int id; //该课程的id
    QString name; //该课程的名字
    QString teacher; //该课程的老师名字
    int capacity; //该课程的可容纳的最多人数
    int selected; //该课程的已选人数
    QString type; //该课程的类型
    QString note; //该课程的备注
};

#endif // COURSE_H
