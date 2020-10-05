#include "OrderMkdir.h"
#include "OrderMan.h"
void OrderMkdir::Address(vector<string> s){
    Splitoff(s);
    par_p=false;
    for (int i=0;i<parameter.size();i++){
        if (parameter[i]=="--help"){
            OrderMan order_now;
            vector<string> st;
            st.push_back(this->GetName());
            order_now.Address(st);
            return;
        }
        int j=1;
        char pp;
        while (pp=parameter[i][j++]) {
            if (pp=='p') par_p=true;
            else {
                filehelper.ErrorOption(this,pp);
                return;
            }
        }
    }
    for (int i=0;i<file.size();i++){
        string st=file.at(i);
        filehelper.HandleWithAddress(st);
        char t[PATH_MAX];
        if (filehelper.FindFile(st.c_str(),t)) {
            printf("%s: cannot create directory \'%s\': File exists\n",this->name.c_str(),file.at(i).c_str());
            continue;
        }
        if (par_p==false){
            string pre=BackToPre((char *)st.c_str());
            if (!filehelper.FindFileError(pre.c_str(),this,file.at(i).c_str())) continue;
            if (mkdir(st.c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO)) {
                printf("Create directory failed\n");
                return;
            }
        }
        else {
            if (!CreateManyDir((char *)st.c_str())) continue;
        }
    }
}

bool OrderMkdir::CreateManyDir(char *st){
    int Path_b=0,Path_e=0;
    string Path="";
    if(st[0]!='/'){
        Path=getcwd(nullptr, 0);
        Path_b=Path.size();
        Path=Path+"/"+string(st);      
    }
    else {
        Path=string(st);
        Path_b=1;
    }
    if (Path[Path.size()-1]!='/') Path+="/";
    Path_e=Path.size();
    for (int i=Path_b;i<Path_e;i++){
        if ('/'==Path[i]) {
            string curPath=Path.substr(0,i);
            if (access(curPath.c_str(),F_OK)!=0){
                if (mkdir(curPath.c_str(),S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH) == -1){
                    printf("mkdir(%s) failed(%s)\n",curPath.c_str(),strerror(errno));
                    return false;
                }
            }
        }
    }
    return true;
}