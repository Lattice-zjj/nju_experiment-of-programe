#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeacherWindow)
{
    ui->setupUi(this);
    //帮助文档
    connect(ui->actiona,&QAction::triggered,
            [=](){ QDesktopServices::openUrl (QUrl::fromLocalFile("./HelpFile/HelpFile.pdf"));
    });
    connect(ui->actionb,&QAction::triggered,
            [=](){ ui->stackedWidget_2->setCurrentWidget(ui->page);
        ui->admin_id->setText(admin->id);ui->admin_pas->setText(admin->password);
    });
    connect(ui->actionc,&QAction::triggered,this,&QMainWindow::close);
    ui->stackedWidget_2->setCurrentWidget(ui->page_1);
    //设置大小固定为640*480
    this->setMaximumSize(640,480);
    this->setMinimumSize(640,480);
    this->setWindowIcon(QIcon("://image/1.png"));
    ui->label_name->setStyleSheet("font-size:12px;color:yellow");
    ui->label_cap->setStyleSheet("font-size:12px;color:black");
    ui->label_sel->setStyleSheet("font-size:12px;color:black");
    ui->label_teacher->setStyleSheet("font-size:12px;color:black");
    ui->label_note->setStyleSheet("font-size:12px;color:black");
    ui->label_typ->setStyleSheet("font-size:12px;color:black");
    ui->label_2->setStyleSheet("font-size:12px;color:black");
    ui->label_4->setStyleSheet("font-size:12px;color:blue");
    ui->label_3->setStyleSheet("font-size:12px;color:black");
    ui->label_5->setStyleSheet("font-size:12px;color:white");
    ui->label_6->setStyleSheet("font-size:12px;color:white");
    ui->label_15->setStyleSheet("font-size:12px;color:white");
    ui->label->setStyleSheet("font-size:12px;color:black");
    ui->label_7->setStyleSheet("font-size:12px;color:black");
    ui->findstudent->setStyleSheet("font-size:12px;color:red");
    ui->findassistant->setStyleSheet("font-size:12px;color:red");
    QFont font ("Microsoft YaHei",1,75);
    ui->label_name->setFont(font);
    ui->label_cap->setFont(font);
    ui->label_sel->setFont(font);
    ui->label_typ->setFont(font);
    ui->label_teacher->setFont(font);
    ui->label_note->setFont(font);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->label_3->setFont(font);
    ui->label_4->setFont(font);
    ui->label_5->setFont(font);
    ui->label_6->setFont(font);
    ui->label_7->setFont(font);
    ui->label_15->setFont(font);
    ui->findstudent->setFont(font);
    ui->findassistant->setFont(font);
    setWindowTitle("学生选课信息管理系统  [管理员模式] ");

    //设置调色板,用于设置titlelabel的颜色
    QPalette p;
    p.setColor(QPalette::WindowText,Qt::blue);
    //设置groupbox，放置两个radiobutton

    ui->search->setPlaceholderText("模糊搜索......");
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
    ui->Logout->setStyleSheet(btnStyle);
    ui->EnterCourses->setStyleSheet(btnStyle);
    ui->InquireCourses->setStyleSheet(btnStyle);
    ui->AddCourses->setStyleSheet(btnStyle);
    ui->DeleteCourses->setStyleSheet(btnStyle);
    ui->ModifyCourses->setStyleSheet(btnStyle);
    ui->CourseInformation->setStyleSheet(btnStyle);
    ui->inquire->setStyleSheet(btnStyle);
    ui->modify->setStyleSheet(btnStyle);
    ui->SureModify->setStyleSheet(btnStyle);
    ui->pushButton->setStyleSheet(btnStyle);
    ui->admin_modify->setStyleSheet(btnStyle);
    ui->idenline->setStyleSheet("font-size:16px;color:yellow");
    ui->timeline->setStyleSheet("font-size:16px;color:blue");
    ui->timeline->setFont(font);
    ui->idenline->setFont(font);
    //设置定时器
    timelinetimer=new QTimer(this);
    timelinetimer->setInterval(100);
    timelinetimer->start();
    connect(timelinetimer,SIGNAL(timeout()),this,SLOT(settimeline()));
    connect(ui->tableWidget1->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(mySortByColumn(int)));
    connect(ui->tableWidget_inquire->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(mySortByColumn3(int)));
}

TeacherWindow::~TeacherWindow()
{
    delete ui;
}
void TeacherWindow::fromLoginDialog(Admin* a,int* n,Student* s)
{
    admin=a;
    st_num=n;
    stud=s;
    //UpInfor();
    ui->stackedWidget_2->setCurrentWidget(ui->page_1);
    this->clearAll();
    printAllCourse();
}
void TeacherWindow::paintEvent(QPaintEvent *){ //不断绘制背景
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("://image/13.png"));
}
void TeacherWindow::on_Logout_clicked()
{
    if(QMessageBox::question(this,"提示","是否注销用户?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
    {
        emit toLoginDialog();
        delete this;
    }
}
void TeacherWindow::notclear_search(const QString &arg1,QTableWidget *p){//模糊搜索的通用函数
    if (arg1.isEmpty()){
        int rowCount=p->rowCount();
        for (int row=0;row<rowCount;row++){
            p->showRow(row);
        }
        return;
    }

    int rowCount=p->rowCount();
    int columnCount=p->columnCount();
    for (int row=0;row<rowCount;row++){
        QString rowData;
        for (int column=0;column<columnCount;column++){
            if (p->item(row,column)){
                rowData+=p->item(row,column)->text();
            }
        }
        if (!rowData.isEmpty()){
            int f=0;
            for (int i=0;i<arg1.length();i++){
                if (rowData.contains(arg1.at(i))) f=1;
            }
            if (f==1){
                p->showRow(row);
            }else{
                p->hideRow(row);
            }
        }
        else{
           p->hideRow(row);
        }
    }
}

void TeacherWindow::on_search_textChanged(const QString &arg1)
{
    notclear_search(arg1,ui->tableWidget1);
}

void TeacherWindow::printAllCourse(){
    qDebug()<<admin->nc;
    ui->stackedWidget_2->setCurrentWidget(ui->page_1);
    ui->tableWidget1->setRowCount(admin->nc);
    ui->tableWidget1->setColumnCount(7);
    QStringList header;
    header<<"课程ID"<<"课程名称"<<"授课老师"<<"上限人数"<<"目前已选"<<"课程类型"<<"备注";
    ui->tableWidget1->setHorizontalHeaderLabels(header);
    ui->tableWidget1->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget1->setAlternatingRowColors(true);
    ui->tableWidget1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QHeaderView *headerView = ui->tableWidget1->verticalHeader();
    headerView->setHidden(true);
    for (int i=0;i<admin->nc;i++){
        QString strid=QString("%1").arg(admin->c[i].id,3,10,QLatin1Char('0'));
        ui->tableWidget1->setItem(i,0,new QTableWidgetItem(strid));
        ui->tableWidget1->setItem(i,1,new QTableWidgetItem(admin->c[i].name));
        ui->tableWidget1->setItem(i,2,new QTableWidgetItem(admin->c[i].teacher));
        ui->tableWidget1->setItem(i,3,new QTableWidgetItem(QString::number(admin->c[i].capacity)));
        ui->tableWidget1->setItem(i,4,new QTableWidgetItem(QString::number(admin->c[i].selected)));
        ui->tableWidget1->setItem(i,5,new QTableWidgetItem(admin->c[i].type));
        ui->tableWidget1->setItem(i,6,new QTableWidgetItem(admin->c[i].note));
        for (int j=0;j<7;j++){
            ui->tableWidget1->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}


int TeacherWindow::printoneCourse(int cid){
    ui->tableWidget_inquire->setRowCount(1);
    ui->tableWidget_inquire->setColumnCount(7);
    QStringList header;
    header<<"课程ID"<<"课程名称"<<"授课老师"<<"上限人数"<<"目前已选"<<"课程类型"<<"备注";
    ui->tableWidget_inquire->setHorizontalHeaderLabels(header);
    ui->tableWidget_inquire->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_inquire->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_inquire->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_inquire->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget_inquire->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget_inquire->setAlternatingRowColors(true);
    ui->tableWidget_inquire->horizontalHeader()->setStretchLastSection(true);
    QHeaderView *headerView = ui->tableWidget_inquire->verticalHeader();
    headerView->setHidden(true);
    for (int i=0;i<admin->nc;i++){
        if (admin->c[i].id==cid){
            QString strid=QString("%1").arg(admin->c[i].id,3,10,QLatin1Char('0'));
            ui->tableWidget_inquire->setItem(0,0,new QTableWidgetItem(strid));
            ui->tableWidget_inquire->setItem(0,1,new QTableWidgetItem(admin->c[i].name));
            ui->tableWidget_inquire->setItem(0,2,new QTableWidgetItem(admin->c[i].teacher));
            ui->tableWidget_inquire->setItem(0,3,new QTableWidgetItem(QString::number(admin->c[i].capacity)));
            ui->tableWidget_inquire->setItem(0,4,new QTableWidgetItem(QString::number(admin->c[i].selected)));
            ui->tableWidget_inquire->setItem(0,5,new QTableWidgetItem(admin->c[i].type));
            ui->tableWidget_inquire->setItem(0,6,new QTableWidgetItem(admin->c[i].note));
            for (int j=0;j<7;j++){
                ui->tableWidget_inquire->item(0, j)->setTextAlignment(Qt::AlignCenter);
            }
            return i;
        }
    }
    return -1;
}

void TeacherWindow::on_modifyID_textChanged(const QString &arg1)
{
    int f=0;
    for (int i=0;i<admin->nc;i++){
        if (admin->c[i].id==arg1.toInt()){
            f=1;
            ui->modifycap->setText(QString::number(admin->c[i].capacity));
            ui->modifyteacher->setText(admin->c[i].teacher);
            ui->modifynote->setText(admin->c[i].note);
        }
    }
    if (f==0) {
        ui->modifycap->clear();
        ui->modifyteacher->clear();
        ui->modifynote->clear();
    }
}

/*点击表头进行排序的函数*/
void TeacherWindow::mySortByColumn(int column){
    static bool f=true;
    ui->tableWidget1->sortByColumn(column,f? Qt::AscendingOrder:Qt::DescendingOrder);
    f=!f;
}


void TeacherWindow::mySortByColumn3(int column){
    static bool f=true;
    ui->tableWidget_inquire->sortByColumn(column,f? Qt::AscendingOrder:Qt::DescendingOrder);
    f=!f;
}
/*文件操作*/
void TeacherWindow::UpInfor(){ //用于从文件中读取所有的数据（主要是用于将上一次保留的信息拷贝到内存中
    QFile file;
    file.setFileName("./data/currentcourse.txt");
    bool isOK=file.open(QIODevice::ReadOnly);
    if (isOK){
        admin->isFirst=false;
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
                admin->c[admin->nc++] = course;
            }

        }
        file.close();
    }

    QFile file2;
    file2.setFileName("./data/student.txt");
    bool isOK2=file2.open(QIODevice::ReadOnly);
    if (isOK2&&admin->isFirst==false){
        QTextStream stream(&file2);
        stream.setCodec("UTF-8");
        while (stream.atEnd()==false){
            QString st1;
            stream>>st1;
            if (st1!=""){
                QList<QString> stt;
                stt=st1.split(',');
                stud[(*st_num)++].id=stt.at(0);
                stud[(*st_num) - 1].password=stt.at(1);
            }
        }
        cout<<*st_num;
        file2.close();
    }else {
        cout<<"打开失败！";
    }
    QFile file1;
    file1.setFileName("./data/assistant.txt");
    bool isOK1=file1.open(QIODevice::ReadOnly);
    if (isOK1&&admin->isFirst==false){
        QTextStream stream(&file1);
        stream.setCodec("UTF-8");
        while (stream.atEnd()==false){
            QString str,st1;
            stream>>str>>st1;
            int cinid = str.toInt(),cid=0;
            for (int i = 0; i < admin->nc; i++) {
                 if (admin->c[i].id == cinid) { cid = i; break; }
            }
            QList<QString> stt;
            stt=st1.split(',');
            for (int i=0;i<stt.size();i++){
                admin->ass[cid][admin->nuass[cid]++]=stt.at(i);
                for (int j=0;j<(*st_num);j++){
                    if (stud[j].id==stt.at(i)) stud[j].num_ass++;
                }
            }
        }
        file1.close();
    }else {
        cout<<"打开失败！";
    }
    for (int i = 0; i < (*st_num); i++) {
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
        if (isOK&&admin->isFirst==false){
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
                    for (int j = 0; j < admin->nc; j++) {
                        if (cid == admin->c[j].id) { t = j; break; }
                    }
                    if (admin->c[t].type== "专业课") stud[i].zhuannum++; else stud[i].nozhuannum++;
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

void TeacherWindow::UpAlldata(){ //用于已选人数的自动更新（即使所给课程中的数据不正确），助教的名单删除，并将信息写入文件中（实时更新）
    for (int i = 0; i < admin->nc; i++) {
        int x = 0;
        for (int j = 0; j < *st_num; j++) {
            int f = 0;
            for (int k = 0; k < stud[j].num_cho; k++)
                if (stud[j].cho[k] == admin->c[i].id) { f = 1; break; }
            if (f == 1) {
                x++;
            }
        }
        admin->c[i].selected = x;
    }

    for (int i = 0; i < admin->nc; i++) {
        if (admin->nuass[i] != 0) {
            while (1) {
                int p = -1;
                for (int j = 0; j < admin->nuass[i]; j++) {
                    bool ischoose = false;
                    for (int k = 0; k < *st_num; k++) {
                        if (admin->ass[i][j]==stud[k].id) {
                            ischoose = stud[k].choose(admin->c[i].id); break;
                        }
                    }
                    if (!ischoose) { p = j; break; }
                }
                if (p >= 0) {
                    for (int j = p; j < admin->nuass[i] - 1; j++) {
                        admin->ass[i][j]= admin->ass[i][j + 1];
                    }
                    admin->nuass[i]--;
                }
                else break;
            }
        }
    }

    QFile filer;
    filer.setFileName("./data/currentcourse.txt");
    bool isOKr=filer.open(QIODevice::ReadOnly);
    if (isOKr != false) {
        QFile filew;
        filew.setFileName("./data/currentcourse.txt");
        bool isOKw=filew.open(QIODevice::WriteOnly);
        if (isOKw == false) {
            QMessageBox::warning(this,"提示","打开失败！");
        }
        else {
            admin->isFirst = false;
            QTextStream streamw(&filew);
            streamw.setCodec("UTF-8");
            Course course;
            for (int i = 0; i < admin->nc; i++) {
                course = admin->c[i];
                QString str=QString("%1").arg(course.id,3,10,QLatin1Char('0'));
                QString st1=course.name,st2=course.teacher,st3=course.type,st4=course.note;
                int y=course.capacity,z=course.selected;
                streamw<<str<<"\t"<<st1<<"\t"<<st2<<"\t"<<y<<"\t"<<z<<"\t"<<st3<<"\t"<<st4<<"\r\n";
            }
            filew.close();
        }
        filer.close();
    }
    QFile file1;
    file1.setFileName("./data/assistant.txt");
    bool isOK1=file1.open(QIODevice::WriteOnly);
    if (isOK1 == false || admin->isFirst) {
        cout<<"打开失败！";
    }
    else {
        QTextStream stream1(&file1);
        stream1.setCodec("UTF-8");
        for (int i = 0; i < admin->nc; i++) {
            cout<<admin->nuass[i];
            if (admin->nuass[i] != 0) {
                QString str1=QString("%1").arg(admin->c[i].id,3,10,QLatin1Char('0'));
                QString str="";
                for (int j = 0; j < admin->nuass[i]; j++) {
                    if (j == 0) str=str+admin->ass[i][j];else str=str+","+admin->ass[i][j];
                }
                stream1<<str1<<"\t"<<str<<"\r\n";
            }
        }
        file1.close();
    }
    QFile file2;
    file2.setFileName("./data/student.txt");
    bool isOK2=file2.open(QIODevice::WriteOnly);
    if (isOK2 == false|| admin->isFirst) {
        cout<<"打开失败！";
    }
    else {
        QTextStream stream2(&file2);
        stream2.setCodec("UTF-8");
        for (int i = 0; i < *st_num; i++) {
            QString str;
            str=stud[i].id+","+stud[i].password;
            stream2<<str<<"\r\n";
        }
        file2.close();
    }
    for (int i = 0; i < *st_num; i++) {
        QString st = "./data/student", s1 = "";
        int t = i + 1;
        while (t) {
            s1 = char(t % 10 + 48) + s1;
            t = t / 10;
        }
        st = st + s1 + ".txt";
        QFile file3;
        file3.setFileName(st);
        bool isOK3=file3.open(QIODevice::WriteOnly);
        if (isOK3 == false|| admin->isFirst) {
            cout<<"打开失败！";
        }
        else {
            QTextStream stream3(&file3);
            stream3.setCodec("UTF-8");
            for (int j = 0; j < stud[i].num_cho; j++) {
                QString str=QString("%1").arg(stud[i].cho[j],3,10,QLatin1Char('0'));
                str=str+"\t"+stud[i].ass[j]+"\r\n";
                stream3<<str;
            }
            file3.close();
        }
    }

}
void TeacherWindow::on_pushButton_clicked()
{
    QString path=QFileDialog::getSaveFileName(this,"save","../","CSV(*.csv)");
    if (!path.isEmpty()){
        QFile filew;
        filew.setFileName(path);
        bool isOK=filew.open(QIODevice::WriteOnly);
        if (isOK == false) {
            QMessageBox::warning(this,"提示","打开失败！");
        }
        else {
            QTextStream streamw(&filew);
            streamw.setCodec("UTF-8");
            Course course;
            QString s1="课程ID",s2="课程名称",s3="授课老师",s4="上限人数",s5="目前已选",s6="课程类型",s7="备注";
            streamw<<s1<<","<<s2 <<","<<s3<<","<<s4<<","<<s5<<","<<s6<<","<<s7<<"\r\n";
            for (int i = 0; i < admin->nc; i++) {
                course = admin->c[i];
                QString str=QString("%1").arg(course.id,3,10,QLatin1Char('0'));
                QString st1=course.name,st2=course.teacher,st3=course.type,st4=course.note;
                int y=course.capacity,z=course.selected;
                streamw<<str<<","<<st1<<","<<st2<<","<<y<<","<<z<<","<<st3<<","<<st4<<"\r\n";
            }
            filew.close();
            QMessageBox::warning(this,"提示","导出成功！");
        }
    }
}

void TeacherWindow::on_EnterCourses_clicked()
{
    if (!admin->isFirst){
        QMessageBox::warning(this,"警告","对不起，您已经将课程录入过了！");
        printAllCourse();
        return;
    }
    QString filename=QFileDialog::getOpenFileName(this,"open","../","TEXT(*.txt);;all(*.*)");
    qDebug()<<filename;
    if (filename=="") return;
    QString back=admin->EnterCourses(filename);
    QMessageBox::warning(this,"提示",back);
    UpInfor();
    cout<<1;
    UpAlldata();
    cout<<1;
    printAllCourse();
}

void TeacherWindow::on_InquireCourses_clicked()
{
    printAllCourse();
}

void TeacherWindow::on_AddCourses_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_2);
}

void TeacherWindow::on_SureModify_clicked()
{
    QString cname=ui->lineEdit_name->text(),cteacher=ui->lineEdit_teacher->text(),ctype=ui->lineEdit_typ->text(),cnote=ui->lineEdit_note->text();
    int cca=ui->lineEdit_cap->text().toInt(),cse=ui->lineEdit_sel->text().toInt();
    if (cname==""){
        int ret=QMessageBox::question(this,"确定","确定要添加这个课程名字为空的课程？",QMessageBox::Yes,QMessageBox::Cancel);
        if (ret==QMessageBox::Yes){
            Course c(0,cname,cteacher,cca,cse,ctype,cnote);
            QString st=admin->AddCourses(c);
            QMessageBox::warning(this,"提示",st);
            this->clearAll();
            printAllCourse();
        }
    } else if (cteacher==""){
        int ret=QMessageBox::question(this,"确定","确定要添加这个老师名字为空的课程？",QMessageBox::Yes,QMessageBox::Cancel);
        if (ret==QMessageBox::Yes){
            Course c(0,cname,cteacher,cca,cse,ctype,cnote);
            QString st=admin->AddCourses(c);
            QMessageBox::warning(this,"提示",st);
            this->clearAll();
            printAllCourse();
        }
    } else if (ctype==""){
        int ret=QMessageBox::question(this,"确定","确定要添加这个课程类型为空的课程？（默认为专业课哦）",QMessageBox::Yes,QMessageBox::Cancel);
        if (ret==QMessageBox::Yes){
            ctype="专业课";
            Course c(0,cname,cteacher,cca,cse,ctype,cnote);
            QString st=admin->AddCourses(c);
            QMessageBox::warning(this,"提示",st);
            this->clearAll();
            printAllCourse();
        }
    }else {
        int ret=QMessageBox::question(this,"确定","确定要添加此课程？",QMessageBox::Yes,QMessageBox::Cancel);
        if (ret==QMessageBox::Yes){
            Course c(0,cname,cteacher,cca,cse,ctype,cnote);
            QString st=admin->AddCourses(c);
            QMessageBox::warning(this,"提示",st);
            this->clearAll();
            printAllCourse();
        }
    }
}

void TeacherWindow::on_DeleteCourses_clicked()
{
    //ui->stackedWidget_2->setCurrentWidget(ui->page);
    QList<QTableWidgetSelectionRange> ranges=ui->tableWidget1->selectedRanges();
    int count=ranges.count();
    QString help;
    for(int i=0;i<count;i++){
        int topRow=ranges.at(i).topRow();
        int bottomRow=ranges.at(i).bottomRow();
        for(int j=topRow;j<=bottomRow;j++){
            int cid=ui->tableWidget1->item(j,0)->text().toInt();
            QString st=admin->DeleteCourses(cid);
            help=help+ui->tableWidget1->item(j,0)->text()+st+"\n";
        }
    }
    if (help=="") help="你还未选中课程哦";
    QMessageBox::warning(this,"提示",help);
    this->clearAll();
    on_search_textChanged(ui->search->text());
    printAllCourse();
}

void TeacherWindow::on_modify_clicked()
{
    int cid=ui->modifyID->text().toInt();
    QString cte=ui->modifyteacher->text(),cca=ui->modifycap->text();
    QString cno=ui->modifynote->text();
    QString st;
    st=admin->ModifyCourses(cid,cte,cca,cno,3);
    QMessageBox::warning(this,"提示",st);
    this->clearAll();
    printAllCourse();
}

void TeacherWindow::on_ModifyCourses_clicked()
{
    ui->stackedWidget_2->setCurrentWidget(ui->page_modify);
}

void TeacherWindow::on_CourseInformation_clicked()
{
    printoneCourse(-1);
    ui->outcome->setTextBackgroundColor(QColor(0,255,120));
    ui->outcome->setTextColor(QColor(0,0,255));
    ui->stackedWidget_2->setCurrentWidget(ui->page_inquire);
}

void TeacherWindow::on_inquire_clicked()
{
    cout<<1;
    QString st;
    radiogroup1=new QButtonGroup(this);
    radiogroup1->addButton(ui->findstudent,0);
    radiogroup1->addButton(ui->findassistant,1);
    int x=ui->id_inquire->text().toInt();
    cout<<x;
    int ci=printoneCourse(x);
    if (ci<0){
        QMessageBox::warning(this,"警告","无此课程");
        ui->tableWidget_inquire->clearContents();
        ui->outcome->clear();
        this->clearAll();
    }
    else{
        cout<<ci;
        x=radiogroup1->checkedId();
        cout<<admin->c[ci].selected;
        if (x == 0) {
            QString st1="目前选择该门的学生有：";
            int i = 0;
            for (int j = 0; j < *st_num; j++) {
                int f = 0;
                for (int k=0;k<stud[j].num_cho;k++)
                    if (stud[j].cho[k] == admin->c[ci].id) { f = 1; break; }
                if (f == 1) {
                    if (i == admin->c[ci].selected - 1) st1=st1+stud[j].id; else st1=st1+stud[j].id+",";
                    i++;
                }
            }
            ui->outcome->setText(st1);
            cout<<ci;
            cout<<admin->c[ci].selected;
            st.sprintf("共计有%d名学生，距离满课还有%d人！", admin->c[ci].selected,admin->c[ci].capacity - admin->c[ci].selected);
            QMessageBox::warning(this,"提示",st);
        }
        else if (x==1){
            QString st2="目前报名该门的助教有：";
            for (int i = 0; i < admin->nuass[ci]; i++) {
                if (i == admin->nuass[ci] - 1) st2=st2+admin->ass[ci][i]; else st2=st2+admin->ass[ci][i]+",";
            }
            ui->outcome->setText(st2);
        }
        this->clearAll();
    }
}


void TeacherWindow::clearAll(){
    ui->lineEdit_cap->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_sel->clear();
    ui->lineEdit_teacher->clear();
    ui->lineEdit_typ->clear();
    ui->modifycap->clear();
    ui->modifyteacher->clear();
    ui->modifycap->clear();
    ui->modifyID->clear();
    ui->id_inquire->clear();
    if (admin->isFirst==false) UpAlldata();
}

void TeacherWindow::settimeline()
{
    //设置时间
    QDateTime time=QDateTime::currentDateTime();
    QString str=time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->timeline->setText(str);
    //右下角的问候语
    int hour=time.time().hour();
    QString iden="["+admin->id+"]";
    if (hour>=13&& hour<=18) iden+=" 下午好！";
    else if (hour==12) iden+=" 中午好！";
    else if (hour>=0&&hour<=2) iden+=" 凌晨好！ 早点休息";
    else if (hour<12) iden+=" 上午好！";
    else iden+=" 晚上好！";
    ui->idenline->setText(iden);
}

void TeacherWindow::on_admin_modify_clicked()
{
    QString st1=ui->admin_id->text(),st2=ui->admin_pas->text();
    admin->id=st1;admin->password=st2;
    printAllCourse();
}
