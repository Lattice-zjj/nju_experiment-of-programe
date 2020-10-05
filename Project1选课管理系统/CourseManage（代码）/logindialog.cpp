#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    //帮助文档
    connect(ui->actiona,&QAction::triggered,
            [=](){ QDesktopServices::openUrl (QUrl::fromLocalFile("./HelpFile/HelpFile.pdf"));
    });

    //ui界面的各种控件的样式初始化
    this->setMaximumSize(640,480);
    this->setMinimumSize(640,480);
    this->setWindowIcon(QIcon("://image/1.png"));
    setWindowTitle("学生选课信息管理系统 ");
    ui->userlabel->setStyleSheet("font-size:12px;color:black");
    ui->passwordlabel->setStyleSheet("font-size:12px;color:black");
    ui->yanzheng->setStyleSheet("font-size:12px;color:black");
    ui->timeline->setStyleSheet("font-size:16px;color:blue");
    ui->idenline->setStyleSheet("font-size:16px;color:yellow");
    ui->timeline->setStyleSheet("font-size:16px;color:blue");
    QFont font ("Microsoft YaHei",1,75);
    ui->userlabel->setFont(font);
    ui->passwordlabel->setFont(font);
    ui->yanzheng->setFont(font);
    ui->timeline->setFont(font);
    ui->idenline->setFont(font);
    ui->timeline->setFont(font);
    //设置调色板,用于设置titlelabel的颜色
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::blue);
    ui->titlelabel->setStyleSheet("font-size:40px;color:blue");
    ui->titlelabel->setPalette(pal);
    ui->titlelabel->setFont(font);
    //设置groupbox，放置两个radiobutton
    radiogroup=new QButtonGroup(this);
    radiogroup->addButton(ui->studentradio,0);
    radiogroup->addButton(ui->teacherradio,1);
    ui->studentradio->setStyleSheet("color:white");
    ui->teacherradio->setStyleSheet("color:white");
    //logo加载
    ui->logo_nju->setPixmap(QPixmap("://image/logo_nju.png").scaled(ui->logo_nju->size()));
    ui->loginbtn->setEnabled(false);//设置登录按钮不可用
    ui->userline->setPlaceholderText("请输入用户名");
    ui->passwordline->setPlaceholderText("请输入密码");
    ui->yanEdit->setPlaceholderText("请输入验证码");//设置lineedit提示语句
    ui->passwordline->setEchoMode(QLineEdit::Password);  //设置passlineedit显示为密码模式
    //设置按钮的样式
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
    ui->loginbtn->setStyleSheet(btnStyle);
    ui->registerbtn->setStyleSheet(btnStyle);
    ui->exitbtn->setStyleSheet(btnStyle);



    //设置定时器
    timelinetimer=new QTimer(this);
    timelinetimer->setInterval(100);
    timelinetimer->start();
    connect(timelinetimer,SIGNAL(timeout()),this,SLOT(settimeline()));

    //设置登录按钮可用
    connect(ui->userline,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSetSlot(QString)));
    connect(ui->passwordline,SIGNAL(textChanged(QString)),this,SLOT(loginbtnSetSlot(QString)));

    //设置验证码
    QTime t;
    t=QTime::currentTime();
    qsrand(uint(t.msec()+t.second()*1000));
    this->on_yanzhenglabel_clicked();

    st_num=0;
    UpInfor();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_yanzhenglabel_clicked()
{
    num_yan=qrand()%10000;
    while(num_yan<1000) num_yan=qrand()%10000;
    int a=qrand()%256,b=qrand()%256,c=qrand()%256;
    QString st=QString::number(a)+","+QString::number(b)+","+QString::number(c);
    ui->yanzhenglabel->setText(QString::number(num_yan));
    ui->yanzhenglabel->setStyleSheet("background-color: rgb("+st+");");
}


void LoginDialog::loginbtnSetSlot(QString)
{
    ui->loginbtn->setEnabled(true);
}

void LoginDialog::on_loginbtn_clicked()
{
    cout<<st_num;
    if(!this->judgeEmpty()) {
        ui->passwordline->clear();
        return;
    }
    QString getyan=ui->yanEdit->text();
    if (getyan!=QString::number(num_yan)) {
        QMessageBox::information(this,"提示","验证码输入错误",QMessageBox::Yes);
        ui->passwordline->clear();
        this->on_yanzhenglabel_clicked();
        return;
    }
    //用于记录历史写入的账号
    QString now_user_name=ui->userline->text();
    QVector<QString>::iterator iter;
    for(iter=history.begin();iter!=history.end();iter++){
        if((*iter)==now_user_name){ history.erase(iter);break;}
    }
    history.push_back(ui->userline->text());
    //开始判断选的是哪个身份
    if(radiogroup->checkedId()==1){//选择是管理员登陆
        QString cid=ui->userline->text(),cpa=ui->passwordline->text();//获取输入的用户名与密码
        if (admin.if_locked) { //如果管理员的账号被锁住了
            QMessageBox::information(this,"提示","管理员账号封锁中....",QMessageBox::Yes);
            this->clearAll();
            return;
        }
        if (!admin.Login(cid,cpa)) { //登陆失败
            num_admin_login++;//失败登陆的次数自增
            if (num_admin_login>2){//失败登陆的次数大于2次
                QMessageBox::information(this,"提示","管理员登陆尝试次数超过2次！账号封锁5分钟",QMessageBox::Yes);
                admin.if_locked=true;
                num_admin_login=0;
                timer=new QTimer(this);
                timer->start(1000*300);
                connect(timer,SIGNAL(timeout()),this,SLOT(unlock()));
            }
            else {
                QMessageBox::information(this,"提示","密码或用户名错误！！",QMessageBox::Yes);
                on_yanzhenglabel_clicked();
            }
            this->clearAll();
        }
        else {//登陆成功
            this->clearAll();
            num_admin_login=0;
            QMessageBox::information(this,"提示","管理员 您已经成功登陆！！",QMessageBox::Yes);
            teacher=new TeacherWindow;
            teacher->show();
            connect(this,SIGNAL(toTeacherWindow(Admin*,int*,Student*)),teacher,SLOT(fromLoginDialog(Admin*,int*,Student*)));
            connect(teacher,SIGNAL(toLoginDialog()),this,SLOT(showNormal()));
            emit toTeacherWindow(&admin,&st_num,stud);
            this->hide();
            return;
        }

    }
    else if (radiogroup->checkedId()==0){
        QString cid=ui->userline->text(),cpa=ui->passwordline->text();
        bool f=false;
        for (int i = 0; i < st_num; i++) {
            if (cid==stud[i].id){
                f=true;
                if (stud[i].if_lock){
                    QMessageBox::information(this,"提示","该账号封锁中....",QMessageBox::Yes);
                    this->clearAll();
                    return;
                }
            }
            if (stud[i].Login(cid, cpa)) {
                nowstu = i;
                QString st;
                this->clearAll();
                for (int j = 0; j < stud[nowstu].num_cho; j++) {
                    if (stud[nowstu].ass[j]=="@#$%") {
                        QString strid=QString("%1").arg(stud[nowstu].cho[j],3,10,QLatin1Char('0'));
                        st="你的"+strid+"课程的助教已经退课，请重新选择";
                        QMessageBox::information(this,"提示",st,QMessageBox::Ok);
                        stud[nowstu].ass[j]="Null";
                    }
                }
                QMessageBox::information(this,"提示","学生 ["+cid+"] 您已经成功登陆！！",QMessageBox::Yes);
                student=new StudentWindow;
                student->show();
                connect(this,SIGNAL(toStudentWindow(Admin*,int*,Student*,int)),student,SLOT(fromLoginDialog(Admin*,int*,Student*,int)));
                connect(student,SIGNAL(toLoginDialog()),this,SLOT(showNormal()));
                emit toStudentWindow(&admin,&st_num,stud,nowstu);
                this->hide();

                return;
            }
            else {
                if (cid==stud[i].id){
                    if (stud[i].num_of_login>5){
                        QMessageBox::information(this,"提示","该账号登陆尝试次数超过5次！账号封锁5分钟",QMessageBox::Yes);
                        stud[i].if_lock=true;
                        stud[i].num_of_login=0;
                        timer=new QTimer(this);
                        timer->start( 1000*300 );
                        connect( timer, SIGNAL( timeout() ), this, SLOT( unlock_s() ) );
                        this->clearAll();
                        return;
                    }
                }
            }
        }
        this->clearAll();
        if (f) QMessageBox::information(this,"提示","密码输入有误！！",QMessageBox::Yes);
        else QMessageBox::information(this,"提示","该用户名不存在！！",QMessageBox::Yes);
    }
    else {
        QMessageBox::information(this,"提示","请选择登陆身份",QMessageBox::Yes);
    }
}

bool LoginDialog::judgeEmpty()
{
    if(ui->userline->text().isEmpty()){
        QMessageBox::warning(this,"警告","用户名不能为空");
        return false;
    }
    if(ui->passwordline->text().isEmpty()){
        QMessageBox::warning(this,"警告","密码不能为空");
        return false;
    }
    else return true;
}

void LoginDialog::clearAll()
{
    this->on_yanzhenglabel_clicked();
    ui->userline->clear();
    ui->passwordline->clear();
    ui->yanEdit->clear();
}

void LoginDialog::settimeline()
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

void LoginDialog::on_userline_textEdited(const QString &arg1)
{
    QStringList wordList;
    for (int i=0;i<history.size();i++)
        if (history[i].contains(arg1)) wordList<<history[i];
    QCompleter *completer = new QCompleter(wordList,this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->userline->setCompleter(completer);
}

void LoginDialog::paintEvent(QPaintEvent *){ //不断绘制背景
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("://image/13.png"));
}
void LoginDialog::unlock(){
    admin.if_locked=false;
    timer->stop();
}
void LoginDialog::UpInfor(){ //用于从文件中读取所有的数据（主要是用于将上一次保留的信息拷贝到内存中
    cout<<1;
    QFile file;
    file.setFileName("./data/currentcourse.txt");
    bool isOK=file.open(QIODevice::ReadOnly);
    if (isOK){
        admin.isFirst=false;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        while (stream.atEnd()==false){
            QString str,st1,st2,st3,st4;
            int y,z;
            QString stt=stream.readLine(256);
            QList<QString> l=stt.split("\t");
            str=l.at(0);st1=l.at(1);st2=l.at(2);y=l.at(3).toInt();z=l.at(4).toInt();
            st3=l.at(5);
            if (l.size()==6) st4="";else st4=l.at(6);
            if (str!=""&&st1!=""&&st2!=""&&st3!=""){
                cout<<str<<st1<<st2<<y<<z<<st3;
                Course course=Course(str.toInt(),st1,st2,y,z,st3,st4);
                admin.c[admin.nc++] = course;
            }

        }
        file.close();
    }
cout<<1;
    QFile file2;
    file2.setFileName("./data/student.txt");
    bool isOK2=file2.open(QIODevice::ReadOnly);
    if (isOK2&&admin.isFirst==false){
        QTextStream stream(&file2);
        stream.setCodec("UTF-8");
        while (stream.atEnd()==false){
            QString st1;
            stream>>st1;
            if (st1!=""){
                QList<QString> stt;
                stt=st1.split(',');
                stud[st_num++].id=stt.at(0);
                stud[st_num - 1].password=stt.at(1);
            }
        }
        cout<<st_num;
        file2.close();
    }else {
        cout<<"打开失败！";
    }
    QFile file1;
    file1.setFileName("./data/assistant.txt");
    bool isOK1=file1.open(QIODevice::ReadOnly);
    if (isOK1&&admin.isFirst==false){
        QTextStream stream(&file1);
        stream.setCodec("UTF-8");
        while (stream.atEnd()==false){
            QString str,st1;
            stream>>str>>st1;
            int cinid = str.toInt(),cid=0;
            for (int i = 0; i < admin.nc; i++) {
                 if (admin.c[i].id == cinid) { cid = i; break; }
            }
            QList<QString> stt;
            stt=st1.split(',');
            for (int i=0;i<stt.size();i++){
                admin.ass[cid][admin.nuass[cid]++]=stt.at(i);
                for (int j=0;j<(st_num);j++){
                    if (stud[j].id==stt.at(i)) stud[j].num_ass++;
                }
            }
        }
        file1.close();
    }else {
        cout<<"打开失败！";
    }
    for (int i = 0; i < (st_num); i++) {
        QString st = "./data/student", s1 = "";
        int t = i + 1;
        while (t) {
            s1 = char(t % 10 + 48) + s1;
            t = t / 10;
        }
        st = st + s1 + ".txt";
        QFile file3;
        file3.setFileName(st);
        bool isOK=file3.open(QIODevice::ReadOnly);
        if (isOK&&admin.isFirst==false){
            QTextStream stream(&file3);
            stream.setCodec("UTF-8");
            while (stream.atEnd()==false){
                QString str,st1;
                stream>>str>>st1;
                int cid = 0;;
                cid=str.toInt();
                QString spa=st1;
                if (cid) {
                    int t = 0;
                    for (int j = 0; j < admin.nc; j++) {
                        if (cid == admin.c[j].id) { t = j; break; }
                    }
                    if (admin.c[t].type== "专业课") stud[i].zhuannum++; else stud[i].nozhuannum++;
                    stud[i].cho[stud[i].num_cho++] = cid;
                    stud[i].ass[stud[i].num_cho - 1]=spa;
                }
            }
            file3.close();
        }else {
            cout<<"打开失败！";
        }
    }
}

void LoginDialog::on_registerbtn_clicked()
{
    reg=new RegisterDialog;
    reg->show();
    connect(this,SIGNAL(toRegisterWindow(Admin*,int*,Student*)),reg,SLOT(fromLoginDialog(Admin*,int*,Student*)));
    connect(reg,SIGNAL(toLoginDialog()),this,SLOT(showNormal()));
    emit toRegisterWindow(&admin,&st_num,stud);
    this->hide();
}
