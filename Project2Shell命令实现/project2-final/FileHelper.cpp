#include "FileHelper.h"
bool FileHelper::Is_a_directory(const char* st){
    struct stat s;
    stat(st,&s);
    if (S_ISDIR(s.st_mode)) return true; else return false; 
}
bool FileHelper::FindFile(const char *s,char* st){
    if(realpath(s, st)){
        return true;
    }
    else{
        return false;
    }
}
bool FileHelper::FindFileError(const char *s,Order *t,const char *infile){
    char st[PATH_MAX];
    if (!FindFile(s,st)){
        printf("%s: %s: No such file or directory\n",t->GetName().c_str(),infile);
        return false;
    }
    return true;
}
bool FileHelper::Is_a_directoryError(const char* st,Order *t,const char *infile){
    struct stat s;
    stat(st,&s);
    if (S_ISDIR(s.st_mode)) {
        printf("%s: %s: Is a directory\n",t->GetName().c_str(),infile);
        return true; 
    }
    else return false; 
}
void FileHelper::ErrorOption(Order *t,const char st){
    printf("%s: invalid option -- \'%c\'\n",t->GetName().c_str(),st);
    printf("Try \'%s --help\' for more information.\n",t->GetName().c_str());
}
void FileHelper::HandleWithAddress(string& st){
    while (st.length()>1 && st[st.size()-1]=='/' && st[st.size()-2]=='/') st.pop_back();
    if (st[0]=='~') {
        st.erase(st.begin());st="/home/"+string(UserName)+st;
        return;
    }
    if (st[0]!='/' && st[0]!='.') if (!strcmp(NowPath,"/")) st="/"+st;else st=string(NowPath)+"/"+st;
    if (st[0]=='.'&&st[1]!='.'){
        st.erase(st.begin());if (st.length()==1) st.erase(st.begin());st=string(NowPath)+st;
    }
    if (st[0]=='.'&&st[1]=='.'){
        st.erase(st.begin());st.erase(st.begin());
        if (st.length()==1) st.erase(st.begin());
        char p[PATH_MAX];
        strcpy(p,NowPath);
        st=BackToPre(p)+st;
    }
    if (st.length()!=1 && st[st.size()-1]=='/') st.pop_back();
}