#include "control.h"
Control::Control(){
    
}
void Control::run(){
    readdata();
    while (1){
        init();
        string option;
        cout<<"请输入您的选择：";
        cin>>option;
        if (option=="1") putinto();
        else if (option=="2") calculate();
        else if (option=="3") inversion();
        else if (option=="4") divide();
        else if (option=="5") getright();
        else if (option=="6") putout();
        else if (option=="7") {
            savedata();
            return;
        }
        else cout<<"输入的指令不对呀，是不是输错了呢？重新试试看"<<endl;
        cout<<endl<<endl;
        getchar();
    }
}

void Control::init(){
    cout<<"=========(*^_^*)=============多项式计算器=========O(∩_∩)O============="<<endl;
    cout<<"1.输入多项式 2.混合运算 3.求逆 4.除法/取模运算 5.求根 6.查询多项式 7.退出"<<endl;
    cout<<"====================================================================="<<endl;
}

void Control::putinto(){
    while (1){
        double a[100];
        int num;
        while (1){
            cout<<"请输入多项式的长度：";
            string st;cin>>st;
            bool flag=true;
            for (int i=0;i<st.length();i++) if (!(st[i]>='0' && st[i]<='9')) {flag=false;break;}
            if (!flag) { cout<<"请输入正整数噢！"<<endl;continue;}
            num=int(char_to_num((char *)st.c_str()).second);
            if (num==0) { cout<<"请输入正整数噢！"<<endl;continue;}
            break;
        }
        vector<double> firstdata;
        while (1){
            cout<<"请按照次数从高到低输入系数：（只能输入double型数）\n";
            firstdata.clear();
            bool flag=true;
            while (1){
                char st[255];char *p;
                cin.getline(st,255);
                p=strtok(st," ");
                vector<string> temp;
                while (p){
                    temp.push_back(string(p));
                    p=strtok(NULL," ");
                }
                for (int i=0;i<temp.size();i++) {
                    pair<bool,double> transport=char_to_num((char*)(temp[i].c_str()));
                    bool f=transport.first;
                    if (!f) {flag=false;break;}
                    firstdata.push_back(transport.second);
                }
                if (!flag) break;
                if (firstdata.size()>=num) break; 
            }
            if (!flag) {
                cout<<"您的输入并不符合规范呢>_<"<<endl;continue;
            }
            if (firstdata.size()>num){
                if (!ask("您输入的系数的个数已经超过了您刚刚输入的长度了(*^_^*)呃（输入y/yes代表只取前面有效的数字，n/no代表重输入）？[y/n]：")) continue; 
            } 
            if (firstdata.size()>=1 && firstdata[0]==0.0){
                if (!ask("您输入的最高位的系数为0呢(*^_^*)（输入y/yes代表确实是这样，n/no代表重输入）？[y/n]：")) continue; 
            }
            for (int i=num-1;i>=0;i--) {
                a[i]=firstdata[num-1-i];
            }
            break;
        }
        Polynomial p(a,num);
        cout<<"请输入该多项式的名字：";
        string name;
        while (1){
            cin>>name;
            if (ans.find(name)!=ans.end()){
                cout<<"对不起，这个名字已经被使用过啦(❤ ω ❤)，请重新起个名字叭：";
            }else if (!judgename(name)){
                cout<<"对不起，您起的这个名字不合法呢(*^_^*)，请重新起个名字叭：";
            }else break;
        }
        ans[name]=data.size();
        data.push_back(p);
        cout<<"您刚刚输入的多项式为："<<name<<" = "<<p<<endl;
        cout<<endl;
        cout<<"输入成功！";
        bool flag=IfRepeatDo("输入");
        if (!flag) break;
        cout<<endl<<endl;
    }
}


void Control::putout(){
    while (1){
        cout<<"您想要查询的多项式名称为：";
        string name;cin>>name;
        if (ans.find(name)==ans.end()){
            cout<<"抱歉！您想要查询的多项式并没有(*^_^*)，请查看是否输入错误！\n";
        }
        else{
            cout<<name<<" = "<<data[ans[name]]<<"\n";
        }
        bool flag=IfRepeatDo("查询");
        if (!flag) break;
        cout<<endl<<endl;
    }

}
void Control::getright(){
    while (1){
        cout<<"您想要求根的多项式名称为：";
        string name;cin>>name;
        if (ans.find(name)==ans.end()){
            cout<<"抱歉！您想要求根的多项式并没有(*^_^*)，请查看是否输入错误！\n";
        }
        else{
            pair<bool,double> t=data[ans[name]].get();
            if (t.first==true){
                cout<<data[ans[name]]<<" = 0 "<<"方程的根为"<<t.second<<endl;
            }else{
                cout<<data[ans[name]]<<" = 0 "<<"暂时还找不到根哦(oﾟvﾟ)ノ"<<endl;
            }
        }
        bool flag=IfRepeatDo("求根");
        if (!flag) break;
        cout<<endl<<endl;
    }
}
void Control::divide(){
    while (1){
        cout<<"请输入表达式(形如F/G)：";
        string st;cin>>st;
        int num=count(st.begin(),st.end(),'/');
        if (num!=1) {
            cout<<"您输入的表达式格式有问题(oﾟvﾟ)ノ，请查看是否输入错误！"<<endl;
        }
        else{
            char str[200];
            char *p;
            strcpy(str,st.c_str());
            string s1,s2;
            vector<string> temp;
            p=strtok(str,"/");
            while (p){
                temp.push_back(string(p));
                p=strtok(NULL,"/");
            }
            if (temp.size()!=2) {
                cout<<"您输入的表达式格式有问题(ง •_•)ง，请查看是否输入错误！"<<endl;
            }
            else{
                s1=temp[0];s2=temp[1];
                if (helpexist(s1,"求除法/取模运算") && helpexist(s2,"除法/取模运算") ){
                    Polynomial a1=data[ans[s1]],a2=data[ans[s2]];
                    pair<Polynomial,Polynomial> t=a1.divide(a2);
                    cout<<"商为："<<t.first<<"  余数为："<<t.second<<endl;
                    saveoutcome(t.first);saveoutcome(t.second);
                }
            }
        }

        bool flag=IfRepeatDo("除法/取模运算");
        if (!flag) break;
        cout<<endl<<endl;
    }
}
void Control::inversion(){
    while (1){
        cout<<"您想要求逆元的多项式名称为：";
        string name;cin>>name;
        if (helpexist(name,"求逆元")){
            Polynomial t=data[ans[name]];
            pair<bool,Polynomial> outcome=t.inversion();
            if (outcome.first==false) cout<<"该多项式并没有逆元哦U•ェ•*U"<<endl;else {
                cout<<"该多项式的逆元为："<<outcome.second<<endl;
                saveoutcome(outcome.second);
            }
        }
        bool flag=IfRepeatDo("求逆元");
        if (!flag) break;
        cout<<endl<<endl;
    }
}

bool Control::helpexist(string name,string help){
    if (ans.find(name)==ans.end()){
        cout<<"抱歉！您想要"<<help<<"的多项式并没有(～﹃～)~zZ，请查看是否输入错误！\n";
        return false;
    }
    return true;
}
void Control::calculate(){
    while (1){
        cout<<"请输入您要计算的表达式：";
        string st;cin>>st;
        int t=manage(st);
        if (t==0) {
            cout<<"您输入的表达式格式有问题(+_+)?，请查看是否输入错误！"<<endl;
        }else if (t==2){
            cout<<"您输入的表达式中含有未知的代号(+_+)?，请查看是否输入错误"<<endl;
        }else{
            saveoutcome(outcome_p);
        }
        bool flag=IfRepeatDo("计算");
        if (!flag) break;
        cout<<endl<<endl;
    }
}

bool Control::IfRepeatDo(string oper){
    bool flag;
    while (1){
        cout<<"是否继续"<<oper<<"[y/n]：";
        string option;
        cin>>option;
        transform(option.begin(),option.end(),option.begin(),::tolower);
        if (option=="yes" || option=="y") {
            flag=true;break;
        }
        else if (option=="no" || option=="n"){
            flag=false;break;
        }
    }
    return flag;
}

int Control::manage(string st){
    Polynomial val1,val2,val;
    stack<Polynomial> ist;
    stack<string> cst;
    int i=0;
    string start,ch;
    while (i!=st.length()){
        if (isalpha(st[i])){
            start="";
            while (i!=st.length() && isalpha(st[i])){
                start=start+string(1,st[i]);
                i++;
            }
            if (ans.find(start)==ans.end()) return 2;
            ist.push(data[ans[start]]);
        }
        else if (st[i]=='('){
            cst.push(string(1,st[i]));
            i++;
        }
        else if (st[i]=='$'){
            start="";
            while (i!=st.length() && st[i]!=']'){
                if (!(st[i]>='0' && st[i]<='9' || st[i]=='.' || st[i]==','|| st[i]=='[' ||st[i]=='$' || st[i]=='-')) return 0;
                start=start+string(1,st[i]);
                i++;
            }
            if (i==st.length()) return 0;
            start=start+string(1,st[i]);
            i++;
            if (cst.empty()){
                cst.push(start);
            }else{
                ch=cst.top();
                if (ch=="+"){
					cst.push(start);
                }else if (ch=="*"){
                    cst.push(start);
                } else if (ch=="("){
                    cst.push(start);
                } else if (ch[0]=='$'){
                    return 0;
                }
            }
        }
        else if (st[i]==')'){
            if (cst.size()==0) return 0;
            ch=cst.top();
			while(ch!="(") {
				cst.pop();
                if (ch=="+"){
                    if (ist.size()<2) return 0;
				    val1=ist.top();ist.pop();
				    val2=ist.top();ist.pop();
                    val=val2+val1;
                }else if (ch=="*"){
                    if (ist.size()<2) return 0;
                    val1=ist.top();ist.pop();
				    val2=ist.top();ist.pop();
                    val=val2*val1;
                }else if (ch[0]=='$'){
                    if (!judgest(ch)) return 0;
                    pair<double,double> t=dealst((char *)ch.c_str());
                    if (ist.size()==0) return 0;
                    val1=ist.top();ist.pop();
                    val=val1.integrate(t.first,t.second);
                }
                if (cst.size()==0) return 0;
				ch=cst.top();
				ist.push(val);
			}
			cst.pop();
			i++;
        }
        else if (st[i]=='+') {
            if (cst.empty()){
                cst.push(string(1,st[i]));
            }else{
                ch=cst.top();
                if (ch=="+"){
                    cst.pop();
                    if (ist.size()<2) return 0;
					val1=ist.top();ist.pop();
					val2=ist.top();ist.pop();
					val=val2+val1;
					ist.push(val);
					cst.push(string(1,st[i]));
                }else if (ch=="*"){
                    cst.pop();
                    if (ist.size()<2) return 0;
					val1=ist.top();ist.pop();
					val2=ist.top();ist.pop();
					val=val2*val1;
					ist.push(val);
					cst.push(string(1,st[i]));
                }else if (ch=="("){
                    cst.push(string(1,st[i]));
                }else if (ch[0]=='$'){
                    cst.pop();
                    if (!judgest(ch)) return 0;
                    pair<double,double> t=dealst((char *)ch.c_str());
                    if (ist.size()==0) return 0;
                    val1=ist.top();ist.pop();
                    val=val1.integrate(t.first,t.second);
                    ist.push(val);
                    cst.push(string(1,st[i]));
                }
            }
            i++;
        } else if (st[i]=='*'){
            if (cst.empty()){
                cst.push(string(1,st[i]));
            }else{
                ch=cst.top();
                if (ch=="+"){
					cst.push(string(1,st[i]));
                }else if (ch=="*"){
                    cst.pop();
                    if (ist.size()<2) return 0;
					val1=ist.top();ist.pop();
					val2=ist.top();ist.pop();
					val=val2*val1;
					ist.push(val);
					cst.push(string(1,st[i]));
                } else if (ch=="("){
                    cst.push(string(1,st[i]));
                } else if (ch[0]=='$'){
                    cst.pop();
                    if (!judgest(ch)) return 0;
                    pair<double,double> t=dealst((char *)ch.c_str());
                    if (ist.size()==0) return 0;
                    val1=ist.top();ist.pop();
                    val=val1.integrate(t.first,t.second);
                    ist.push(val);
                    cst.push(string(1,st[i]));
                }
            }
            i++;
        } else if (st[i]=='!'){
            if ((i==0)||(i>0 && !(isalpha(st[i-1]) || st[i-1]==')'))) return 0;  
            if (ist.size()==0) return 0;
            val1=ist.top();ist.pop();
            val=val1.derivative();
            ist.push(val);
            i++;
        } else return 0;
    }
    while (!cst.empty()){
        ch=cst.top();
		cst.pop();
        if (ch=="(") return 0;
        if (ch=="+"){
            if (ist.size()<2) return 0;
		    val1=ist.top();ist.pop();
		    val2=ist.top();ist.pop();
            val=val2+val1;
        }else if (ch=="*"){
            if (ist.size()<2) return 0;
		    val1=ist.top();ist.pop();
		    val2=ist.top();ist.pop();
            val=val2*val1;
        } else if (ch[0]=='$'){
            if (ist.size()==0) return 0;
		    val1=ist.top();ist.pop();
            if (!judgest(ch)) return 0;
            pair<double,double> t=dealst((char *)ch.c_str());
            val=val1.integrate(t.first,t.second);
        }
        ist.push(val);
    }
    if (ist.size()!=1) return 0;
    val=ist.top();
    outcome_p=val;
    cout<<st<<"的结果是："<<val<<endl;
    return 1;
}

pair<bool,double> Control::char_to_num(char* s) //将字符型数据转换成double型数据
{
	double d = 0;
	int i=0, t = 0;
    if (s[0]=='-') i=1;
    bool flag=true;
	for (; s[i] != 0; i++) {
		if (s[i] == '.') {
            if (!flag) return pair<bool,double>(false,0.0);
			t = 10;
            flag=false;
		}
		else if (s[i]>='0' && s[i]<='9'){
			if (t == 0) {
				d = d * 10 + (double(s[i]) - 48);
			}
			else {
				d = d + (double)(double(s[i]) - 48) / t;
				t *= 10;
			}
		}
        else return pair<bool,double>(false,0.0);
	}
    if (s[0]=='-') return pair<bool,double>(true,-d); else return pair<bool,double>(true,d);
}

pair<double,double> Control::dealst(char *str){
    string s1,s2;
    s1=string(strtok(str,","));
    s2=string(strtok(NULL,","));
    s1.erase(s1.begin());s1.erase(s1.begin());s2.pop_back();
    return pair<double,double>(char_to_num((char *)s1.c_str()).second,char_to_num((char *)s2.c_str()).second);
}

bool Control::judgest(string st){
    st.erase(st.begin());
    if (st[0]!='[') return false;
    if (st[st.length()-1]!=']') return false;
    st.pop_back();
    st.erase(st.begin());
    int num=count(st.begin(),st.end(),',');
    if (num!=1) return false;
    char str[200];
    char *p;
    strcpy(str,st.c_str());
    string s1,s2;
    vector<string> temp;
    p=strtok(str,",");
    while (p){
        temp.push_back(string(p));
        p=strtok(NULL,",");
    }
    if (temp.size()!=2) return false;
    s1=temp[0];s2=temp[1];
    for (int i=0;i<s1.length();i++) {
        if (!(s1[i]>='0' && s1[i]<='9' || s1[i]=='.' || s1[i]=='-')) return false;
    }
    for (int i=0;i<s2.length();i++) {
        if (!(s2[i]>='0' && s2[i]<='9' || s2[i]=='.' || s2[i]=='-')) return false;
    }
    bool f1=char_to_num((char *)s1.c_str()).first,f2=char_to_num((char *)s2.c_str()).first;
    if (f1&&f2) return true;else return false;
}
void Control::saveoutcome(Polynomial s){
    bool flag;
    while (1){
        cout<<"请问是否将  "<<s<<"  存储起来（＞人＜；）？[y/n]：";
        string option;
        cin>>option;
        transform(option.begin(),option.end(),option.begin(),::tolower);
        if (option=="yes" || option=="y") {
            flag=true;break;
        }
        else if (option=="no" || option=="n"){
            flag=false;break;
        }
    }
    if (flag==false) return;
    else{
        cout<<"请输入该多项式的名字：";
        string name;
        while (1){
            cin>>name;
            if (ans.find(name)!=ans.end()){
                cout<<"对不起，这个名字已经被使用过啦(*^_^*)，请重新起个名字叭：";
            }else if (!judgename(name)){
                cout<<"对不起，您起的这个名字不合法呢(+_+)?，请重新起个名字叭：";
            }else break;
        }
        ans[name]=data.size();
        data.push_back(s);
    }
}

bool Control::judgename(string st){
    for (int i=0;i<st.length();i++)
        if (!isalpha(st[i])) return false;
    return true;
}

void Control::savedata(){
    bool flag=ask("请问您是否愿意将所有您现有的多项式存储到文件中供下次使用(￣▽￣)\"？[y/n]：");
    if (!flag) return;
    map<string,int>::iterator it;
    ofstream out_file("data.dat",ios::out);
    if (!out_file) exit(-1);
    for (it=ans.begin();it!=ans.end();it++){
        out_file<<it->first<<" "<<data[it->second].getnum()<<" ";
        vector<double> t=data[it->second].getxi();
        for (int i=0;i<t.size();i++) out_file<<t[i]<<" ";
        out_file<<endl;
    }
    out_file.close();
}


void Control::readdata(){
    ifstream in_file("data.dat",ios::in);
    if (!in_file) return;
    bool flag=ask("请问您是否愿意将您上一次保存的数据读取到内存中供当次使用O(∩_∩)O？[y/n]：");
    if (!flag) return;
    string name;
    in_file>>name;
    while (!in_file.eof()){
        int num;in_file>>num;
        double w[200];
        for (int i=0;i<num;i++) in_file>>w[i];
        Polynomial p(w,num);
        ans[name]=data.size();
        data.push_back(p);
        in_file>>name;
    }
}

bool Control::ask(string st){
    bool flag;
    while (1){
        cout<<st;
        string option;
        cin>>option;
        transform(option.begin(),option.end(),option.begin(),::tolower);
        if (option=="yes" || option=="y") {
            flag=true;break;
        }
        else if (option=="no" || option=="n"){
            flag=false;break;
        }
    }
    return flag;
}