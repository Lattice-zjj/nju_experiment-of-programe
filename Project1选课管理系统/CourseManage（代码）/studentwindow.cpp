#include "studentwindow.h"
#include "ui_studentwindow.h"

StudentWindow::StudentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudentWindow)
{
    ui->setupUi(this);
    //设置大小固定为640*480
    this->setMaximumSize(640,480);
    this->setMinimumSize(640,480);
    this->setWindowIcon(QIcon("://image/1.png"));
    setWindowTitle("学生选课信息管理系统 [ 学生模式 ] ");
    connect(ui->actiona,&QAction::triggered,
            [=](){ QDesktopServices::openUrl (QUrl::fromLocalFile("./HelpFile/HelpFile.pdf"));
    });
    connect(ui->actionb,&QAction::triggered,
            [=](){ ui->stackedWidget_3->setCurrentWidget(ui->page_s_2);
    });
    connect(ui->actionc,&QAction::triggered,this,&QMainWindow::close);
    ui->label_10->setStyleSheet("font-size:12px;color:black");
    ui->label_11->setStyleSheet("font-size:12px;color:black");
    ui->label_12->setStyleSheet("font-size:12px;color:black");
    ui->label_13->setStyleSheet("font-size:12px;color:black");
    ui->label_14->setStyleSheet("font-size:12px;color:white");
    ui->label->setStyleSheet("font-size:12px;color:white");
    ui->label_2->setStyleSheet("font-size:12px;color:white");
    ui->display_ass->setStyleSheet("font-size:12px;color:black");
    QFont font ("Microsoft YaHei",1,75);
    ui->label_10->setFont(font);
    ui->label_11->setFont(font);
    ui->label_12->setFont(font);
    ui->label_13->setFont(font);
    ui->label_14->setFont(font);
    ui->label->setFont(font);
    ui->label_2->setFont(font);
    ui->display_ass->setFont(font);
    ui->search_2->setPlaceholderText("模糊搜索......");
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
    ui->stu_chooseass->setStyleSheet(btnStyle);
    ui->stu_choosecourse->setStyleSheet(btnStyle);
    ui->stu_dele->setStyleSheet(btnStyle);
    ui->stu_inq->setStyleSheet(btnStyle);
    ui->stu_logout->setStyleSheet(btnStyle);
    ui->stu_person->setStyleSheet(btnStyle);
    ui->stu_signforass->setStyleSheet(btnStyle);
    ui->modify_pas->setStyleSheet(btnStyle);
    ui->detele_s->setStyleSheet(btnStyle);
    ui->signforass->setStyleSheet(btnStyle);
    ui->search_ass->setStyleSheet(btnStyle);
    ui->choose_ass->setStyleSheet(btnStyle);
    ui->tableWidget_s_1->clearContents();
    ui->idenline->setStyleSheet("font-size:16px;color:yellow");
    ui->timeline->setStyleSheet("font-size:16px;color:blue");
    ui->timeline->setFont(font);
    ui->idenline->setFont(font);

    ui->pass_past->setPlaceholderText("请输入初始密码");
    ui->pass_now->setPlaceholderText("请输入要修改成的密码");
    ui->pass_nowa->setPlaceholderText("请输入要修改成的密码");//设置lineedit提示语句
    ui->pass_now->setEchoMode(QLineEdit::Password);  //设置passlineedit显示为密码模式
    ui->pass_nowa->setEchoMode(QLineEdit::Password);  //设置passlineedit显示为密码模式
    //设置定时器
    timelinetimer=new QTimer(this);
    timelinetimer->setInterval(100);
    timelinetimer->start();
    connect(timelinetimer,SIGNAL(timeout()),this,SLOT(settimeline()));

    connect(ui->tableWidget_s_1->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(mySortByColumn2(int)));

}

StudentWindow::~StudentWindow()
{
    delete ui;
}
void StudentWindow::paintEvent(QPaintEvent *){ //不断绘制背景
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("://image/13.png"));
}
void StudentWindow::fromLoginDialog(Admin* a,int* n,Student* s,int x)
{
    admin=a;
    st_num=n;
    stud=s;
    nowstu=x;
    ui->stackedWidget_3->setCurrentWidget(ui->page_s_1);
    this->clearAll();
    printStuAllCourse_s();
}


void StudentWindow::on_stu_logout_clicked()
{
    if(QMessageBox::question(this,"提示","是否注销用户?",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
    {
        emit toLoginDialog();
        delete this;
    }
}
void StudentWindow::mySortByColumn2(int column){
    static bool f=true;
    ui->tableWidget_s_1->sortByColumn(column,f? Qt::AscendingOrder:Qt::DescendingOrder);
    f=!f;
}
void StudentWindow::clearAll()
{
    ui->ass_s->clear();
    ui->pass_past->clear();
    ui->id_s_detele->clear();
    ui->Id_ass->clear();
    ui->name_ass->clear();
    ui->display_ass->clear();
    if (admin->isFirst==false) UpAlldata();
}
/*文件操作*/
void StudentWindow::UpAlldata(){ //用于已选人数的自动更新（即使所给课程中的数据不正确），助教的名单删除，并将信息写入文件中（实时更新）
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

void StudentWindow::printAllCourse_s(){
    ui->stackedWidget_3->setCurrentWidget(ui->page_s_1);
    ui->tableWidget_s_1->setRowCount(admin->nc);
    ui->tableWidget_s_1->setColumnCount(7);
    QStringList header;
    header<<"课程ID"<<"课程名称"<<"授课老师"<<"上限人数"<<"目前已选"<<"课程类型"<<"备注";
    ui->tableWidget_s_1->setHorizontalHeaderLabels(header);
    ui->tableWidget_s_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_s_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_s_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_s_1->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget_s_1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget_s_1->setAlternatingRowColors(true);
    ui->tableWidget_s_1->horizontalHeader()->setStretchLastSection(true);
    QHeaderView *headerView = ui->tableWidget_s_1->verticalHeader();
    headerView->setHidden(true);
    for (int i=0;i<admin->nc;i++){
        QString strid=QString("%1").arg(admin->c[i].id,3,10,QLatin1Char('0'));
        ui->tableWidget_s_1->setItem(i,0,new QTableWidgetItem(strid));
        ui->tableWidget_s_1->setItem(i,1,new QTableWidgetItem(admin->c[i].name));
        ui->tableWidget_s_1->setItem(i,2,new QTableWidgetItem(admin->c[i].teacher));
        ui->tableWidget_s_1->setItem(i,3,new QTableWidgetItem(QString::number(admin->c[i].capacity)));
        ui->tableWidget_s_1->setItem(i,4,new QTableWidgetItem(QString::number(admin->c[i].selected)));
        ui->tableWidget_s_1->setItem(i,5,new QTableWidgetItem(admin->c[i].type));
        ui->tableWidget_s_1->setItem(i,6,new QTableWidgetItem(admin->c[i].note));
        for (int j=0;j<7;j++){
            ui->tableWidget_s_1->item(i, j)->setTextAlignment(Qt::AlignCenter);
            if (stud[nowstu].choose(admin->c[i].id)){
                ui->tableWidget_s_1->item(i, j)->setForeground(QBrush(QColor(255, 0, 0)));
                //将字体加粗
                ui->tableWidget_s_1->item(i, j)->setFont( QFont( "Times", 10, QFont::Black ) );
            }
        }

    }
}
void StudentWindow::notclear_search(const QString &arg1,QTableWidget *p){//模糊搜索的通用函数
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
            if (rowData.contains(arg1)){
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
void StudentWindow::on_search_2_textChanged(const QString &arg1)
{
    notclear_search(arg1,ui->tableWidget_s_1);
}

void StudentWindow::printStuAllCourse_s(){
    ui->stackedWidget_3->setCurrentWidget(ui->page_s_1);
    ui->tableWidget_s_1->setRowCount(stud[nowstu].num_cho);
    ui->tableWidget_s_1->setColumnCount(6);
    QStringList header;
    header<<"课程ID"<<"课程名称"<<"授课老师"<<"课程类型"<<"备注"<<"个人助教";
    ui->tableWidget_s_1->setHorizontalHeaderLabels(header);
    ui->tableWidget_s_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_s_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_s_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_s_1->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableWidget_s_1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableWidget_s_1->setAlternatingRowColors(true);
    ui->tableWidget_s_1->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_s_1->setColumnWidth(1,150);
    QHeaderView *headerView = ui->tableWidget_s_1->verticalHeader();
    headerView->setHidden(true);
    for (int i = 0; i < stud[nowstu].num_cho; i++) {
        printOneCourse_s(i,stud[nowstu].cho[i]);
        ui->tableWidget_s_1->setItem(i,5,new QTableWidgetItem(stud[nowstu].ass[i]));
        for (int j=0;j<6;j++){
            ui->tableWidget_s_1->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
void StudentWindow::printOneCourse_s(int r,int cid){
    for (int i=0;i<admin->nc;i++){
        if (cid==admin->c[i].id){
            QString strid=QString("%1").arg(admin->c[i].id,3,10,QLatin1Char('0'));
            ui->tableWidget_s_1->setItem(r,0,new QTableWidgetItem(strid));
            ui->tableWidget_s_1->setItem(r,1,new QTableWidgetItem(admin->c[i].name));
            ui->tableWidget_s_1->setItem(r,2,new QTableWidgetItem(admin->c[i].teacher));
            ui->tableWidget_s_1->setItem(r,3,new QTableWidgetItem(admin->c[i].type));
            ui->tableWidget_s_1->setItem(r,4,new QTableWidgetItem(admin->c[i].note));
            return;
        }
    }
}
void StudentWindow::on_stu_inq_clicked()
{
    printAllCourse_s();
    this->clearAll();
}

void StudentWindow::on_stu_choosecourse_clicked()
{
    QList<QTableWidgetSelectionRange> ranges=ui->tableWidget_s_1->selectedRanges();
        int count=ranges.count();
        QString help="";
        for(int i=0;i<count;i++){
            int topRow=ranges.at(i).topRow();
            int bottomRow=ranges.at(i).bottomRow();
            for(int j=topRow;j<=bottomRow;j++){
                int x=ui->tableWidget_s_1->item(j,0)->text().toInt();
                QString st;
                bool flag=true;
                for (int i = 0; i < stud[nowstu].num_cho; i++) {
                    if (stud[nowstu].cho[i] == x) {
                        st="课程您已经选过！！";
                        help=help+ui->tableWidget_s_1->item(j,0)->text()+st+"\n";
                        flag=false;break;
                    }
                }
                if (flag==false) continue;
                bool able_to_choose=false;
                int f = 0;
                for (int i = 0; i < admin->nc; i++) {
                    if (x == admin->c[i].id) {
                        f = 1;
                        able_to_choose =( admin->c[i].capacity> admin->c[i].selected);
                        if (able_to_choose) {
                            admin->c[i].selected++;
                            if (admin->c[i].type== "专业课") {
                                stud[nowstu].zhuannum++;
                            }
                            else stud[nowstu].nozhuannum++;
                        }
                        break;
                    }
                }
                if (f == 0) {
                    st="课程无此！！";
                    continue;
                }
                if (stud[nowstu].num_cho == 10) {
                    QMessageBox::warning(this,"警告","您选的课的节数不能超过10节！！");
                    return;
                }
                if (able_to_choose) {
                    stud[nowstu].cho[stud[nowstu].num_cho++] = x;
                    stud[nowstu].ass[stud[nowstu].num_cho - 1]= "Null";
                    st="课程已加入个人课程表中";
                    this->clearAll();
                    ui->stackedWidget_3->setCurrentWidget(ui->page_s_1);

                }
                else {
                    st="课程人数已满！！";
                    help=help+ui->tableWidget_s_1->item(j,0)->text()+st+"\n";
                    continue;
                }

                help=help+ui->tableWidget_s_1->item(j,0)->text()+st+"\n";
            }
        }
        if (help=="") help="你还未选中课程哦";
        QMessageBox::warning(this,"提示",help);
        for (int i = 0; i < stud[nowstu].num_cho-1; i++) {
            for (int j = i + 1; j < stud[nowstu].num_cho; j++) {
                if (stud[nowstu].cho[i] > stud[nowstu].cho[j]) {
                    int t = stud[nowstu].cho[i]; stud[nowstu].cho[i] = stud[nowstu].cho[j]; stud[nowstu].cho[j] = t;
                    QString st=stud[nowstu].ass[i];
                    stud[nowstu].ass[i]=stud[nowstu].ass[j];
                    stud[nowstu].ass[j]= st;
                }
            }
        }
        printStuAllCourse_s();
}

void StudentWindow::on_stu_person_clicked()
{
    printStuAllCourse_s();
    this->clearAll();
}

void StudentWindow::on_stu_dele_clicked()
{
    cout<<1;
    QList<QTableWidgetSelectionRange> ranges=ui->tableWidget_s_1->selectedRanges();
    int count=ranges.count();
    QString help="";
    for(int i=0;i<count;i++){
        int topRow=ranges.at(i).topRow();
        int bottomRow=ranges.at(i).bottomRow();
        for(int j=topRow;j<=bottomRow;j++){
            QString st;
            int x=ui->tableWidget_s_1->item(j,0)->text().toInt();
            int f = 0;
            int cid = 0;
            for (int i = 0; i < stud[nowstu].num_cho; i++) {
                if (stud[nowstu].cho[i] == x) {
                    f = 1; cid = i;
                }
            }
            if (f == 0) {
                st="课程您没有选过！！";
            }
            else{
               for (int i = cid; i < stud[nowstu].num_cho; i++) {
                    int j = i + 1;
                    int t = stud[nowstu].cho[i]; stud[nowstu].cho[i] = stud[nowstu].cho[j]; stud[nowstu].cho[j] = t;
                    QString st=stud[nowstu].ass[i];
                    stud[nowstu].ass[i]=stud[nowstu].ass[j];
                    stud[nowstu].ass[j]= st;
               }
                stud[nowstu].num_cho--;
                st="课程删除成功！！";
                for (int l = 0; l < *st_num; l++) {
                    for (int j = 0; j < stud[l].num_cho; j++) {
                        if (stud[l].ass[j]!="Null") {
                            int xk = 0;
                            for (int k = 0; k < *st_num; k++) {
                                if (stud[l].ass[j]==stud[k].id) {
                                    xk = k; break;
                                }
                            }
                            if (!stud[xk].choose(stud[l].cho[j])) {
                                stud[l].ass[j]="@#$%";
                            }
                        }
                    }
                }
                for (int i = 0; i < admin->nc; i++) {
                    if (x == admin->c[i].id) {
                        admin->c[i].selected--;
                        if (admin->c[i].type== "专业课") {
                            stud[nowstu].zhuannum--;
                        }
                        else stud[nowstu].nozhuannum--;
                        break;
                    }
                }
            }
            help=help+ui->tableWidget_s_1->item(j,0)->text()+st+"\n";
        }
    }
    if (help=="") help="你还未选中课程哦";
    QMessageBox::warning(this,"提示",help);

    printStuAllCourse_s();
    QString st1;
    if (stud[nowstu].zhuannum < 4 && stud[nowstu].nozhuannum < 2) {
        st1.sprintf("根据学院要求：目前所选的课程不达标！仍差%d门专业课，以及%d门非专业课", 4 - stud[nowstu].zhuannum, 2 - stud[nowstu].nozhuannum);
    }
    else if (stud[nowstu].zhuannum < 4) {
        st1.sprintf("根据学院要求：目前所选的课程不达标！仍差%d门专业课", 4 - stud[nowstu].zhuannum);
    }
    else if (stud[nowstu].nozhuannum < 2) {
        st1.sprintf("根据学院要求：目前所选的课程不达标！仍差%d门非专业课", 2 - stud[nowstu].nozhuannum);
    }
    if (st1=="") st1="根据学院要求：目前所选的课程达标啦，恭喜你！";
    QMessageBox::warning(this,"提示",st1);
    //ui->stackedWidget_3->setCurrentWidget(ui->page_s_3);
    this->clearAll();
}

void StudentWindow::on_stu_signforass_clicked()
{
    QList<QTableWidgetSelectionRange> ranges=ui->tableWidget_s_1->selectedRanges();
    int count=ranges.count();
    QString help="";
    for(int i=0;i<count;i++){
        int topRow=ranges.at(i).topRow();
        int bottomRow=ranges.at(i).bottomRow();
        for(int j=topRow;j<=bottomRow;j++){
            QString st;
            int x=ui->tableWidget_s_1->item(j,0)->text().toInt();
            int courid = -1;
            for (int i = 0; i <= admin->nc; i++) {
                if (x == admin->c[i].id) {
                    courid = i; break;
                }
            }
            if (courid == -1) {
                QMessageBox::warning(this,"提示","无此课程！！");
            }
            else {
                int f = 0;
                for (int i = 0; i < stud[nowstu].num_cho; i++) {
                    if (stud[nowstu].cho[i] == x) {
                        f = 1; break;
                    }
                }
                if (f == 0) {
                    help=help+ui->tableWidget_s_1->item(j,0)->text()+"课程你并未选！！"+"\r\n";
                }
                else {
                    bool alr=false;
                    for (int i=0;i<admin->nuass[courid];i++){
                        if (stud[nowstu].id==admin->ass[courid][i]){
                            alr=true;break;
                        }
                    }
                    if (alr){
                        help=help+ui->tableWidget_s_1->item(j,0)->text()+"课程你已经报名过助教啦！！"+"\r\n";
                        this->clearAll();
                        continue;
                    }
                    if (stud[nowstu].num_ass == 2) {
                        help=help+"你已经报名2门课程的助教了，请量力而行！"+"\r\n";
                        this->clearAll();
                        break;
                    }
                    admin->ass[courid][admin->nuass[courid]++]=stud[nowstu].id;
                    help=help+ui->tableWidget_s_1->item(j,0)->text()+"课程报名助教成功！！"+"\r\n";
                    stud[nowstu].num_ass++;
                    this->clearAll();
                    printStuAllCourse_s();
                }
            }
            this->clearAll();
        }
    }
    if (help=="") help="你还未选中课程哦";
    QMessageBox::warning(this,"提示",help);
}

void StudentWindow::on_stu_chooseass_clicked()
{
    ui->stackedWidget_3->setCurrentWidget(ui->page_s_5);
    this->clearAll();
}


void StudentWindow::on_search_ass_clicked()
{
    QString st=ui->Id_ass->text();
    int x=st.toInt();
    int f = 0;
    for (int i = 0; i < stud[nowstu].num_cho; i++) {
        if (stud[nowstu].cho[i] == x) {
            if (stud[nowstu].ass[i]== "Null") {
                f = 1; break;
            }
            else {
                QMessageBox::warning(this,"提示","该课程的个人助教你已经选过！");
                this->clearAll();
                return;
            }
        }
    }
    if (f == 0) {
        QMessageBox::warning(this,"提示","你并未选此课程！！！");
        this->clearAll();
        return;
    }
    QString str="目前这门课的助教有：";
    for (int i = 0; i <= admin->nc;i++) {
        if (x == admin->c[i].id) {
            for (int j = 0; j < admin->nuass[i]; j++) {
                if (j!= admin->nuass[i]-1)str=str+ admin->ass[i][j]+",";else str=str+ admin->ass[i][j];
            }
        }
    }
    ui->display_ass->setText(str);
}

void StudentWindow::on_choose_ass_clicked()
{
    QString st1=ui->Id_ass->text();
    int x=st1.toInt();
    QString st=ui->name_ass->text();
    int cid = 0;
    int f=0;
    for (int i = 0; i < stud[nowstu].num_cho; i++) {
        if (stud[nowstu].cho[i] == x) {
            if (stud[nowstu].ass[i]== "Null") {
                cid = i; f = 1; break;
            }
            else {
                QMessageBox::warning(this,"提示","该课程的个人助教你已经选过！");
                this->clearAll();
                return;
            }
        }
    }
    if (f == 0) {
        QMessageBox::warning(this,"提示","你并未选此课程！！！");
        this->clearAll();
        return;
    }
    int courid = 0;
    for (int i = 0; i <= admin->nc;i++) {
        if (x == admin->c[i].id) {
            courid = i;
        }
    }
    f = 0;
    for (int j = 0; j < admin->nuass[courid]; j++) {
        if (admin->ass[courid][j]==st) {
            f = 1; break;
        }
    }
    if (f == 1) {
        if (st==stud[nowstu].id) {
            QMessageBox::warning(this,"提示","不能选自己！！");
            ui->name_ass->clear();
            return;
        }
        stud[nowstu].ass[cid]= st;
        QMessageBox::warning(this,"提示","已完成个人助教的选择！！");
        this->clearAll();
        printStuAllCourse_s();
    }
    else {
        QMessageBox::warning(this,"提示","并无此助教！！");
    }
    this->clearAll();
}
void StudentWindow::settimeline()
{
    //设置时间
    QDateTime time=QDateTime::currentDateTime();
    QString str=time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->timeline->setText(str);
    //右下角的问候语
    int hour=time.time().hour();
    QString iden="["+stud[nowstu].id+"]";
    if (hour>=13&& hour<=18) iden+=" 下午好！";
    else if (hour==12) iden+=" 中午好！";
    else if (hour>=0&&hour<=2) iden+=" 凌晨好！ 早点休息";
    else if (hour<12) iden+=" 上午好！";
    else iden+=" 晚上好！";
    ui->idenline->setText(iden);
}

void StudentWindow::on_modify_pas_clicked()
{
    QString str1=ui->pass_past->text(),str2=ui->pass_now->text(),str3=ui->pass_nowa->text();
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
        ui->pass_now->clear();
        ui->pass_nowa->clear();
    }else if (str1!=stud[nowstu].password){
        QMessageBox::warning(this,"警告","原始密码不正确");
        ui->pass_past->clear();
        ui->pass_now->clear();
        ui->pass_nowa->clear();
    } else {
        stud[nowstu].password=str2;
        QMessageBox::warning(this,"提示","密码修改成功！");
        ui->stackedWidget_3->setCurrentWidget(ui->page_s_1);
        UpAlldata();
    }
}
