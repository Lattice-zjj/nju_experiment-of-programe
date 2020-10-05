#include "Handle.h"
char delo[2]=" ";
char delo_xie[2]="/";
string orderalready[12]={"cp","cmp","wc","cat","man","ls","pwd","mkdir","cd","exit","vim","rm"};
vector<string> split(char* strc)
{
    vector<string> resultVec;
    char* tmpStr = strtok(strc, delo);
    while (tmpStr != NULL)
    {
        string st=string(tmpStr);
        resultVec.push_back(st);
        tmpStr = strtok(NULL, delo);
    }
    return resultVec;
}
vector<string> split_xie(char* strc)
{
    vector<string> resultVec;
    char* tmpStr = strtok(strc, delo_xie);
    while (tmpStr != NULL)
    {
        string st=string(tmpStr);
        resultVec.push_back(st);
        tmpStr = strtok(NULL, delo_xie);
    }
    return resultVec;
}
void HADNDLE(char *st){
    if (st[0]=='s'&&st[1]=='h' || st[0]=='.'&&st[1]=='/'){
        char zanPath[PATH_MAX];
        strcpy(zanPath,NowPath);
        vector<string> splitAnswer;
        if (st[0]=='.') {
            string s(st);s.erase(s.begin());s.erase(s.begin());
            splitAnswer.push_back(s);
        }
        else {
            splitAnswer=split(st);
            if (splitAnswer.empty()) return;
            vector<string>::iterator k=splitAnswer.begin();
	        splitAnswer.erase(k);
        }
        for (int i=0;i<splitAnswer.size();i++){
            string ss=splitAnswer.at(i);
            FileHelper f;
            f.HandleWithAddress(ss);
            char t[PATH_MAX];
            if (!f.FindFile(ss.c_str(),t)) {
                printf("sh: 0: Can't open %s\n",splitAnswer.at(i).c_str());
                return;
            }
            ifstream onefile;
            onefile.open(ss.c_str());
            string sw;
            while (getline(onefile,sw)) {
                if (sw!="") HADNDLE((char *)sw.c_str());
            }
            onefile.close();
        }
        strcpy(NowPath,zanPath);
    }
    else HandleString(st);
}
void logout(){
    printf("logout\n\n");
    exit(0);
}
void HandleString(char *st){
    vector<string> splitAnswer=split(st);
    if (splitAnswer.empty()) return;
    string com=splitAnswer[0];
    vector<string>::iterator k=splitAnswer.begin();
	splitAnswer.erase(k);
    Order *order;
    if (com=="exit"){//判断输入的字符串为exit
        logout();
    }
    else if (com=="cp") order=new OrderCp;
    else if (com=="cmp") order=new OrderCmp;
    else if (com=="wc") order=new OrderWc;
    else if (com=="cat") order=new OrderCat;
    else if (com=="man") order=new OrderMan;
    else if (com=="ls") order=new OrderLs;
    else if (com=="pwd") order=new OrderPwd;
    else if (com=="mkdir") order=new OrderMkdir;
    else if (com=="cd") order=new OrderCd;
    else if (com=="echo") order=new OrderEcho;
    else if (com=="rm") order=new OrderRm;
    else if (com=="vim") order=new OrderVim;
    else if (judge(com)) return; //模糊输入
    else {
        printf("%s: command not found\n",com.c_str());
        return;
    }
    order->Address(splitAnswer); //处理已经分割好的字符串组
}

int max_same(string s1,string s2){
    if (s1.empty() || s2.empty()){
        return 0;
    }
    size_t len1=s1.size(),len2=s2.size();
    vector<vector<int>> s(len1+1,vector<int>(len2+1,0));
    for (size_t i=0;i<=len1;i++){
        for (size_t j=0;j<=len2;j++){
            if (i==0 || j==0){
                s[i][j]=0;continue;
            }
            if (s1[i-1]==s2[j-1]){
                s[i][j]=s[i-1][j-1]+1;
            }
            else {
                if (s[i-1][j]>s[i][j-1]){
                    s[i][j]=s[i-1][j];
                }
                else {
                    s[i][j]=s[i][j-1];
                }
            }
        }
    }
    return s[len1][len2];
}
bool judge(string st){
    vector<string> possible;
    for (size_t i=0;i<12;i++){
        if (max_same(st,orderalready[i])==orderalready[i].size()){
            possible.push_back(orderalready[i]);
            
        }
    }
    if (possible.size()==0) return false;
    else{
        printf("No Command \'%s\' found, did you mean:\n",st.c_str());
        for (size_t i=0;i<possible.size();i++){
            printf("  command \'%s\' from package \n",possible[i].c_str());
        }
    }
    printf("%s: command not found\n",st.c_str());
    return true;
}