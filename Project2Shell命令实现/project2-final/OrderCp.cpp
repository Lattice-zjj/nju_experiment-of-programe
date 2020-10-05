#include "OrderCp.h"
#include "OrderMan.h"
void OrderCp::Address(vector<string> s){
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
    else if (file.size()==1){
        printf("%s: missing destination file operand after \'%s\'\n",name.c_str(),file.at(0).c_str());
        printf("Try \'%s --help\' for more information.\n",name.c_str());
    }
    if (file.size()==2){
        string s1=file.at(0);
        string s2=file.at(1);
        filehelper.HandleWithAddress(s1);
        filehelper.HandleWithAddress(s2);
        char t[PATH_MAX];
        if (!filehelper.FindFile(s1.c_str(),t)) {
            printf("%s: cannot stat \'%s\': No such file or directory\n",name.c_str(),file.at(0).c_str());
            return;
        }
        string s2pre=BackToPre((char *)s2.c_str());
        if (!filehelper.FindFile(s1.c_str(),t)){
            printf("%s: cannot create regular file \'%s\': No such file or directory\n",name.c_str(),file.at(1).c_str());
        }
        if (!filehelper.Is_a_directory(s1.c_str())){
            if (!filehelper.Is_a_directory(s2.c_str())){
                CopyFile(s1.c_str(),s2.c_str());
            } 
            else{
                vector<string> stt=split((char *)s1.c_str());
                string filename=stt.at(stt.size()-1);
                filename=s2+"/"+filename;
                CopyFile(s1.c_str(),filename.c_str());
            }
        }
        else {
            if (!par_r){
                printf("%s: -r not specified; omitting directory \'%s\'\n",name.c_str(),file.at(0).c_str());
                return;
            }
            if (!filehelper.Is_a_directory(s2.c_str())){
                printf("%s: cannot overwrite non-directory \'%s\' with directory \'%s\'\n",name.c_str(),file.at(1).c_str(),file.at(0).c_str());
                return;
            }
            else {
                char tt[PATH_MAX];
                if (filehelper.FindFile(s2.c_str(),tt)){
                    vector<string> stt=split((char *)s1.c_str());
                    string filename=stt.at(stt.size()-1);
                    string outfilename=s2+"/"+filename;
                    char helper1[PATH_MAX];
                    char helper2[PATH_MAX];
                    filehelper.FindFile(s1.c_str(),helper1);
                    filehelper.FindFile(outfilename.c_str(),helper2);
                    string sh1(helper1),sh2(helper2);
                    if (sh1==sh2){
                        printf("%s: \'%s\' and \'%s\' are the same file\n",this->name.c_str(),file.at(0).c_str(),(file.at(1)).c_str());
                        return;
                    }
                    if (sh2.find(sh1)==0){
                        if (file.at(1).at(file.at(1).size()-1)!='/') filename="/"+filename;
                        printf("%s: cannot copy a directory \'%s\', into itself, \'%s\'\n",name.c_str(),file.at(0).c_str(),(file.at(1)+filename).c_str());
                        return;
                    }
                    DIR *dir = opendir(outfilename.c_str());
                    if (dir == NULL) {
                        if (mkdir(outfilename.c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO)) {
                            printf("%s: cannot create directory \'%s\': No such file or directory\n",name.c_str(),file.at(1).c_str());
                            return;
                        }
                    }
                    CopyAddress(s1.c_str(),outfilename.c_str());
                }
                else{
                    DIR *dir = opendir(s2.c_str());
                    if (dir == NULL) {
                        if (mkdir(s2.c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO)) {
                            printf("%s: cannot create directory \'%s\': No such file or directory\n",name.c_str(),file.at(1).c_str());
                            return;
                        }
                    }
                    CopyAddress(s1.c_str(),s2.c_str());
                }
            }
        }
    }else {
        string st=file.at(file.size()-1);
        filehelper.HandleWithAddress(st);
        if (!filehelper.Is_a_directory(st.c_str())){
            printf("%s: target \'%s\' is not a directory\n",name.c_str(),file.at(file.size()-1).c_str());
            return;
        }
        for (int i=0;i<file.size()-1;i++){
            string s1=file.at(i);
            filehelper.HandleWithAddress(s1);
            vector<string> stt=split((char *)s1.c_str());
            string filename=stt.at(stt.size()-1);
            string outfilename=file.at(file.size()-1)+"/"+filename;
            CopyFile(s1.c_str(),outfilename.c_str());
        }
    }

}
void OrderCp::CopyFile(const char *source,const char *outcome){
    if (!strcmp(source,outcome)){
        printf("%s: \'%s\' and \'%s\' are the same file\n",this->name.c_str(),file.at(0).c_str(),(file.at(1)).c_str());
        return;
    }
    int from_fd,to_fd;
    int bytes_read,bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;
    char t[PATH_MAX];
    if (filehelper.FindFile(outcome,t)) {
        if (par_i){
            printf("%s: overwrite \'",name.c_str());
            for (int i = strlen(NowPath)+1; i < strlen(outcome); ++i) { //这边等待进一步处理
                cout << outcome[i];
            }
            cout << "\'? ";
            string instring;
            getline(cin, instring);
            transform(instring.begin(), instring.end(), instring.begin(), ::tolower);
            if (!(instring.size() == 1 && instring[0] == 'y' || (instring=="yes"))) {
                return;
            }
        }
    }
    fclose(fopen(outcome, "w"));
    if((from_fd=open(source,O_RDONLY))==-1){
        fprintf(stderr,"Open %s Error:%s\n",source,strerror(errno));
        exit(1);
    }
    if((to_fd=open(outcome,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1){
        fprintf(stderr,"Open %s Error:%s\n",outcome,strerror(errno));
        exit(1);
    }
    while(bytes_read=read(from_fd,buffer,BUFFER_SIZE)){
        if((bytes_read==-1)&&(errno!=EINTR)) break;
        else if(bytes_read>0){
            ptr=buffer;
            while(bytes_write=write(to_fd,ptr,bytes_read)){
                if((bytes_write==-1)&&(errno!=EINTR))break;
                else if(bytes_write==bytes_read) break;
                else if(bytes_write>0){
                    ptr+=bytes_write;
                    bytes_read-=bytes_write;
                }
            }
            if(bytes_write==-1)break;
        }
    }
    close(from_fd);
    close(to_fd);
}
void OrderCp::CopyAddress(const char* source,const char *outcome){
    DIR *dir=opendir(source);
    dirent *point;
    while (point=readdir(dir)) {
        if (point->d_type==DT_DIR&&strcmp(point->d_name,".") && strcmp(point->d_name,"..")) {
            string pathOut=string(outcome)+"/"+point->d_name;
            DIR *dirCur=opendir(pathOut.c_str());
            if (dirCur==NULL) {
                if (mkdir(pathOut.c_str(),S_IRUSR|S_IWUSR|S_IXUSR|S_IRWXG|S_IRWXO)) {
                    printf("Create directory failed\n");
                    return;
                }
            }
            string sw=string(source)+"/"+point->d_name;
            CopyAddress(sw.c_str(),pathOut.c_str());
        }
        if (point->d_type==DT_REG) {
            string sw=string(source)+"/"+point->d_name;
            string sto=string(outcome)+"/"+point->d_name;
            CopyFile(sw.c_str(),sto.c_str());
        }
    }
    closedir(dir);
}
