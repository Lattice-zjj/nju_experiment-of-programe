#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDesktopServices>
#include <QTime>
#include <QTimer>
#include <QPainter>
#include "Admin.h"

namespace Ui {
class StudentWindow;
}

class StudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentWindow(QWidget *parent = nullptr);
    ~StudentWindow();
    void UpAlldata();
    void clearAll();
    void printAllCourse_s();//打印学生界面的全部课程
    void notclear_search(const QString &arg1,QTableWidget *p); //模糊搜索
    void printStuAllCourse_s();//打印学生个人课程
    void printOneCourse_s(int r,int cid);//打印一个课程

private slots:
    void fromLoginDialog(Admin*,int*,Student*,int);//与登陆界面相接的槽函数
    void mySortByColumn2(int column);//用于表头排序的槽函数
    void on_stu_logout_clicked();//登出键
    void on_search_2_textChanged(const QString &arg1); //lineedit的内容改变的槽函数
    void on_stu_inq_clicked(); //按钮槽函数
    void on_stu_choosecourse_clicked();//按钮槽函数
    void on_stu_person_clicked();//按钮槽函数
    void on_stu_dele_clicked();//按钮槽函数
    void on_stu_signforass_clicked();//按钮槽函数
    void on_stu_chooseass_clicked();//按钮槽函数
    void on_search_ass_clicked();//按钮槽函数
    void on_choose_ass_clicked();//按钮槽函数
    void settimeline(); //设置时间
    void on_modify_pas_clicked();//按钮槽函数

protected:
    void paintEvent(QPaintEvent * );
signals:
    void toLoginDialog();

private:
    Ui::StudentWindow *ui;
    Admin *admin;
    int *st_num;
    Student *stud;
    int nowstu;
    QTimer* timelinetimer;
};

#endif // STUDENTWINDOW_H
