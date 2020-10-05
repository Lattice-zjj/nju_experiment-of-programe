#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    //设置大小固定为640*480
    this->setMaximumSize(640,480);
    this->setMinimumSize(640,480);
    this->setWindowIcon(QIcon("://image/1.png"));
    setWindowTitle("学生选课信息管理系统 ");
    ui->label_7->setStyleSheet("font-size:12px;color:black");
    ui->label_8->setStyleSheet("font-size:12px;color:black");
    ui->label_9->setStyleSheet("font-size:12px;color:black");
    QFont font ("Microsoft YaHei",1,75);
    ui->label_7->setFont(font);
    ui->label_8->setFont(font);
    ui->label_9->setFont(font);
    ui->idenline->setStyleSheet("font-size:16px;color:yellow");
    ui->timeline->setStyleSheet("font-size:16px;color:blue");
    ui->timeline->setFont(font);
    ui->idenline->setFont(font);

    //设置调色板,用于设置titlelabel的颜色
    QPalette p;
    p.setColor(QPalette::WindowText,Qt::blue);
    ui->title->setStyleSheet("font-size:40px;color:blue");
    ui->title->setFont(font);
    ui->title->setPalette(p);
    ui->lineEdit_s_z_id->setPlaceholderText("请输入用户名");
    ui->lineEdit_s_z_pa->setPlaceholderText("请输入密码");
    ui->lineEdit_s_z_apa->setPlaceholderText("请再一次输入密码");
    ui->lineEdit_s_z_pa->setEchoMode(QLineEdit::Password);
    ui->lineEdit_s_z_apa->setEchoMode(QLineEdit::Password);
    QString btnStyle="QPushButton{\
                color: rgb(255, 255, 255);\
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(166,164,208), stop:0.3 rgb(171,152,230), stop:1 rgb(152,140,220));\
                border:1px;\
                border-radius:5px; \
                padding:2px 4px;  \
            }\
            QPushButton:hover{\
                color: rgb(255, 255, 255); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(130,0,226), stop:0.3 rgb(120,0,230), stop:1 rgb(125,0,226));\
                border:1px;  \
                border-radius:5px; \
                padding:2px 4px; \
            }\
            QPushButton:pressed{    \
                color: rgb(255, 255, 255); \
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgb(240,156,121), stop:0.3 rgb(220,160,140), stop:1 rgb(230,140,120));  \
                border:1px;  \
                border-radius:5px; \
                padding:2px 4px; \
            }";
    ui->back_to->setStyleSheet(btnStyle);
    ui->register_s->setStyleSheet(btnStyle);
    //设置定时器
    timelinetimer=new QTimer(this);
    timelinetimer->setInterval(100);
    timelinetimer->start();
    connect(timelinetimer,SIGNAL(timeout()),this,SLOT(settimeline()));
    ui->register_s->setEnabled(false);//设置注册按钮不可用
    connect(ui->lineEdit_s_z_id,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSetSlot(QString)));
    connect(ui->lineEdit_s_z_pa,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSetSlot(QString)));

}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}
void RegisterDialog::clearAll()
{
    ui->lineEdit_s_z_id->clear();
    ui->lineEdit_s_z_pa->clear();
    ui->lineEdit_s_z_apa->clear();
}
void RegisterDialog::loginbtnSetSlot(QString)
{
    ui->register_s->setEnabled(true);
}
void RegisterDialog::settimeline()
{
    //设置时间
    QDateTime time=QDateTime::currentDateTime();
    QString str=time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->timeline->setText(str);
    //右下角的问候语
    int hour=time.time().hour();
    QString iden="[访客]";
    if (hour>=13&& hour<=18) iden+=" 下午好！";
    else if (hour==12) iden+=" 中午好！";
    else if (hour>=0&&hour<=2) iden+=" 凌晨好！ 早点休息";
    else if (hour<12) iden+=" 上午好！";
    else iden+=" 晚上好！";
    ui->idenline->setText(iden);
}
void RegisterDialog::fromLoginDialog(Admin* a,int* n,Student* s)
{
    admin=a;
    st_num=n;
    stud=s;
}
void RegisterDialog::on_register_s_clicked()
{
    QString str1=ui->lineEdit_s_z_id->text(),str2=ui->lineEdit_s_z_pa->text(),str3=ui->lineEdit_s_z_apa->text();
    if(str1.isEmpty())
    {
        QMessageBox::warning(this,"警告","用户名不能为空");
        return;
    }
    if(str2.isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空");
        return;
    }
    if (str2!=str2){
        QMessageBox::warning(this,"警告","两次密码输入不一致");
        ui->lineEdit_s_z_pa->clear();
        ui->lineEdit_s_z_apa->clear();
    }else{
        int f=0;
        for (int i=0;i<(*st_num);i++){
            if (stud[i].id==str1){
                f=1;break;
            }
        }
        if (f==1) {
            QMessageBox::warning(this,"警告","用户名已经被使用！");
            this->clearAll();
            return;
        }
        student=new StudentWindow;
        student->show();
        QString cid=str1,cpa=str2;
        stud[(*st_num)].id=cid;stud[(*st_num)].password=cpa;
        stud[(*st_num)].Login(cid, cpa);
        (*st_num)++;
        connect(this,SIGNAL(toStudentWindow(Admin*,int*,Student*,int)),student,SLOT(fromLoginDialog(Admin*,int*,Student*,int)));
        connect(student,&StudentWindow::toLoginDialog,
                [=](){emit toLoginDialog();});
        emit toStudentWindow(admin,st_num,stud,(*st_num) - 1);
        this->hide();
        this->clearAll();
        QString stuname;
        stuname="学生"+cid+" 您已经成功注册！";
        QMessageBox::information(this,"提示",stuname,QMessageBox::Yes);
    }
}

void RegisterDialog::on_back_to_clicked()
{
    if(QMessageBox::question(this,"提示","是否返回主界面?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
    {
        emit toLoginDialog();
        delete this;
    }
}
void RegisterDialog::paintEvent(QPaintEvent *){ //不断绘制背景
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("://image/13.png"));
}
