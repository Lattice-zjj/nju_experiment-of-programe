#include "Admin.h"

Admin::Admin() //初始化
{
    id="Admin";password="admin";
    isFirst = true;
    ifLogin = false;
    nc = 0;
    Course s;
    for (int i = 0; i < 100; i++) {
        nuass.push_back(0);c.push_back(s);
    }
    QVector<QString> help;
    for (int i=0;i<100;i++) help.push_back("");
    for (int i=0;i<100;i++) ass.push_back(help);
    if_locked=false;
}
Admin::~Admin()
{

}
Admin::Admin(const QString cid, const QString cpassword) { //初始化
    id=cid;password=cpassword;
    isFirst = true;
    ifLogin = false;
    if_locked=false;
    nc = 0;
    Course s;
    for (int i = 0; i < 100; i++) {
        nuass.push_back(0);c.push_back(s);
    }
    QVector<QString> help;
    for (int i=0;i<100;i++) help.push_back("");
    for (int i=0;i<100;i++) ass.push_back(help);
}
bool Admin::Login(const QString cid, const QString cpassword) {
    if (if_locked) return false;//锁住
    qDebug()<<cid<<id;
    qDebug()<<cpassword<<password;
    if ((id==cid) && (password==cpassword)) {
        ifLogin = true;
        return true;
    }
    else {
        return false;
    }
}
bool Admin::Logout() {
    if (ifLogin) {
        ifLogin = false;
        return true;
    }
    else return false;
}
QString Admin::EnterCourses(QString filename) {
    QFile fpr(filename);
    QFile fpw;
    fpw.setFileName("./data/currentcourse.txt");
    bool isOkr=fpr.open(QIODevice::ReadOnly);//只读模式打开
    bool isOKw=fpw.open(QIODevice::WriteOnly);//只写模式打开
    qDebug()<<isOkr<<isOKw;
    if (isOkr&&isOKw){
        QTextStream streamr(&fpr);
        QTextStream streamw(&fpw);
        streamr.setCodec("GB18030");//读的模式以GB18030的编码形式打开
        streamw.setCodec("UTF-8");//写以UTF-8的编码形式
        while (streamr.atEnd()==false){
            QString str,st1,st2,st3,st4;
            int y,z;
            QString stt=streamr.readLine(256);
            QList<QString> l=stt.split("\t");
            str=l.at(0);st1=l.at(1);st2=l.at(2);y=l.at(3).toInt();z=l.at(4).toInt();
            st3=l.at(5);
            if (l.size()==6) st4="";else st4=l.at(6);//判断是否有备注
            if (str!=""&&st1!=""&&st2!=""&&st3!=""){
                qDebug()<<st1<<st2<<st3;
                streamw<<str<<"\t"<<st1<<"\t"<<st2<<"\t"<<y<<"\t"<<z<<"\t"<<st3<<"\t"<<st4<<"\r\n";
            }
        }
    }
    fpr.close();
    fpw.close();
    isFirst = false;
    return "导入成功";
}

QString Admin::AddCourses(Course cour) {
    for (int i = 0; i < nc; i++) {
        if (c[i].name==cour.name) {
            return"与已有的课程名称冲突！！添加失败！！";
        }
    }
    cour.id = c[nc - 1].id + 1; c[nc++] = cour;
    return "添加成功！！";
}

QString Admin::DeleteCourses(int cid) {
    int x=-1;
    for (int i=0;i<nc;i++){
        if (c[i].id==cid){
            x=i;break;
        }
    }
    if (x==-1){
        return "并无此课程";
    }
    if (c[x].selected > 0) {
        return "课程删除失败，目前已有学生选课！";
    }
    else {
        for (int i = x; i < nc; i++) {//删除后的移动
            c[i] = c[i + 1]; nuass[i] = nuass[i + 1];
            for (int j = 0; j < nuass[i]; j++) ass[i][j]=ass[i + 1][j];
        }
        nc--;
        return "删除成功！";
    }
}

QString Admin::ModifyCourses(int cid,QString teacher,QString cca,QString cno,int y) {
    int f=0;
    for (int i = 0; i < nc; i++) {
        if (c[i].id == cid) {f=1;break;}
    }
    if (f==0) return "无此课程";
    if (y == 1) {
        int x=cid;
        for (int i = 0; i < nc; i++) {
            if (c[i].id == x) c[i].teacher= teacher;
        }
        return "修改成功！！";
    }
    else if (y==2){
        int num=cca.toInt();
        for (int i = 0; i < nc; i++) {
            if (c[i].id == cid) {
                if (c[i].selected >= num) {
                    return "修改失败！因为修改后的容纳人数小于目前已选的人数";
                }
                else {
                    c[i].capacity = num;
                    return "修改成功！！";
                }
            }
        }
    }else {
        QString st;
        int x=cid;
        for (int i = 0; i < nc; i++) {
            if (c[i].id == x) {
                c[i].teacher= teacher;
                c[i].note=cno;
            }
        }
        st="老师的信息修改成功！！\r\n";
        int num=cca.toInt();
        for (int i = 0; i < nc; i++) {
            if (c[i].id == x) {
                if (c[i].selected >= num) {
                    st=st+ "容纳修改失败！因为修改后的容纳人数小于目前已选的人数"+"\r\n";
                    break;
                }
                else {
                    c[i].capacity = num;
                    st=st+ "容纳人数修改成功！！"+"\r\n";
                    break;
                }
            }
        }
        st=st+"备注修改成功！！";
        return st;
    }
    return "非法操作";
}

