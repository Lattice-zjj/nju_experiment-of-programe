#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include "Admin.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidget>
#include <QFileDialog>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QButtonGroup>
#include <QDesktopServices>
#define cout qDebug()<<"["<<__FILE__<<":"<<__LINE__<<"]"

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeacherWindow(QWidget *parent = nullptr);
    ~TeacherWindow();
    void notclear_search(const QString &arg1,QTableWidget *p);
    void UpInfor();
    void UpAlldata();
    void printAllCourse();
    int printoneCourse(int cid);
    void clearAll();


private slots:
    void fromLoginDialog(Admin*,int*,Student*);//与登陆界面相接的槽函数
    void on_Logout_clicked();//按钮槽函数
    void on_search_textChanged(const QString &arg1);//lineedit的内容改变的槽函数
    void on_modifyID_textChanged(const QString &arg1);//lineedit的内容改变的槽函数
    void on_pushButton_clicked();//按钮槽函数
    void on_EnterCourses_clicked();//按钮槽函数
    void on_InquireCourses_clicked();//按钮槽函数
    void on_AddCourses_clicked();//按钮槽函数
    void on_SureModify_clicked();//按钮槽函数
    void on_DeleteCourses_clicked();//按钮槽函数
    void on_modify_clicked();//按钮槽函数
    void on_ModifyCourses_clicked();//按钮槽函数
    void on_CourseInformation_clicked();//按钮槽函数
    void on_inquire_clicked();//按钮槽函数
    void mySortByColumn(int column);//用于表头排序的槽函数
    void mySortByColumn3(int column);//用于表头排序的槽函数
    void settimeline();//设置时间
    void on_admin_modify_clicked();//按钮槽函数

protected:
    void paintEvent(QPaintEvent * );
signals:
    void toLoginDialog();
private:
    Ui::TeacherWindow *ui;
    QString Admin_name,Admin_password;
    Admin *admin;
    int *st_num;
    Student *stud;
    QButtonGroup * radiogroup1;
    QTimer* timelinetimer;//计时器
};

#endif // TEACHERWINDOW_H
