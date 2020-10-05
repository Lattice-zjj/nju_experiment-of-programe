#include "OrderRm.h"
#include "OrderMan.h"
void OrderRm::Address(vector<string> s){
    Splitoff(s);
    par_i=false;
    par_r=false;
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
            if (pp=='r') par_r=true;
            else if (pp=='i') par_i=true;
            else if (pp=='f') par_i=false;
            else {
                filehelper.ErrorOption(this,pp);
                return;
            }
        }
    }
    if (file.size()==0){
        printf("%s: missing file operand\n",name.c_str());
        printf("Try \'%s --help\' for more information.\n",name.c_str());
    }
    for (int i=0;i<file.size();i++){
        string st=file.at(i);
        filehelper.HandleWithAddress(st);
        char t[PATH_MAX];
        if (!filehelper.FindFile(st.c_str(),t)) {
            printf("%s: cannot remove \'%s\': No such file or directory\n",name.c_str(),file.at(i).c_str());
            return;
        }
        if (filehelper.Is_a_directory(st.c_str())){
            if (par_r){
                if (par_i){
                    printf("%s: descend into directory \'%s\'? ",this->name.c_str(),file.at(i).c_str());
                    if (!InputYes()) continue;
                }
                RemoveAddress(st,file.at(i));
            }
            else printf("%s: cannot remove \'%s\': Is a directory\n",this->name.c_str(),file.at(i).c_str());
        }else {
            RemoveFile(st,file.at(i));
        }
    }
}

void OrderRm::RemoveAddress(string real,string input){
    DIR *dir=opendir(real.c_str());
    if (dir!=NULL){
        dirent *file;
        while (file=readdir(dir)) {
            if (file->d_type==DT_DIR) {
                if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {
                    RemoveAddress(real+"/"+file->d_name,input+"/"+file->d_name);
                }
            }
            if (file->d_type==DT_REG) {
                RemoveFile(real+"/"+file->d_name,input+"/"+file->d_name);
            }
        }
        closedir(dir);
    }
    dir=opendir(real.c_str());
    if (dir!=NULL){
        dirent *file;
        bool flag=true;
        while (file=readdir(dir)) {
            if (file->d_type==DT_DIR) {
                if (strcmp(file->d_name, ".") && strcmp(file->d_name, "..")) {
                    flag=false;
                }
            }
            if (file->d_type==DT_REG) flag=false;
        }
        if (flag) {
            closedir(dir);
            if (par_i) {
                printf("%s: remove directory \'%s\'? ",this->name.c_str(),(input).c_str());
                if (!InputYes()) return;
            }
            remove(real.c_str());
        }
    }
}

void OrderRm::RemoveFile(string real,string input){
    if (!par_i){
        remove(real.c_str());
    }else{
        printf("%s: remove file \'%s\'? ",name.c_str(),input.c_str());
        if (!InputYes()) return;
        remove(real.c_str());
    }
}

bool OrderRm::InputYes(){
    string instring;
    getline(cin, instring);
    transform(instring.begin(), instring.end(), instring.begin(), ::tolower);
    if (!(instring.size() == 1 && instring[0] == 'y' || (instring=="yes"))) {
        return false;
    }
    return true;
}