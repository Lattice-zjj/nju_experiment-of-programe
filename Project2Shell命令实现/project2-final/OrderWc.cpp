#include "OrderWc.h"
#include "OrderMan.h"
void OrderWc::Address(vector<string> s){
    if (s[0]=="--help"){
        OrderMan order_now;
        vector<string> st;
        st.push_back(this->GetName());
        order_now.Address(st);
        return;
    }
    else {
        string st;
        int t=-1;
        int sum=0;
        vector<int> total;
        int num_of_he=0;
        int sum_ac=0;
        for (int j=0;j<s.size();j++) if (s[j][0]=='-') num_of_he++;
        for (int i=0;i<s.size();i++){
            if (s[i][0]!='-') {
                st=s[i];t=i;sum_ac++;
                filehelper.HandleWithAddress(st); //存放绝对路径 s[t]表示输入的路径
                char tt[PATH_MAX]; //存放实际路径
                if (!filehelper.FindFile(st.c_str(),tt)) { 
                    printf("wc: %s: No such file or directory\n",s[t].c_str());
                    continue;
                }
                if (filehelper.Is_a_directory(st.c_str())){
                    printf("wc: %s: Is a directory\n",s[t].c_str());
                    printf("0 %s\n",s[t].c_str());
                    continue;
                }
                std::vector<int> num;
                for (int j=0;j<s.size();j++){
                    if (s[j][0]=='-'){
                        int i=1;
                        while (s[j][i]){
                            if (s[j][i]=='c'){
                                num.push_back(HandleWc_C(st.c_str()));
                            }
                            else if (s[j][i]=='w'){
                                num.push_back(HandleWc_W(st.c_str()));
                            }
                            else if (s[j][i]=='l'){
                                num.push_back(HandleWc_L(st.c_str()));
                            }
                            else { //出现未识别传入
                                printf("wc: invalid option -- \'%c\'\n",s[j][i]);
                                printf("Try 'wc --help' for more information.\n");
                                return;
                            }
                            i++;
                        }
                    }
                }
                if (num.size()==0){
                    num.push_back(HandleWc_L(st.c_str()));
                    num.push_back(HandleWc_W(st.c_str()));
                    num.push_back(HandleWc_C(st.c_str()));
                }
                for (int i=0;i<num.size();i++){
                    printf("%d ",num.at(i));
                    if (sum==0){
                        total.push_back(num.at(i));
                    }
                    else total[i]+=num.at(i);
                }
                sum++;
                printf("%s\n",s[t].c_str());
            }
        }
        if (sum_ac==1) return;
        if (sum!=0) {
            for (int i=0;i<total.size();i++){
                printf("%d ",total.at(i));
            }
            printf("total\n");
            return;
        }
        if (t==-1){ while(1); } //无输入路径
        else {
            if (num_of_he==0) num_of_he=3;
            for (int i=0;i<num_of_he;i++){
                printf("0 ");
            }
            printf("total\n");
        }
    }
}




/*处理wc的命令*/
int OrderWc::HandleWc_C(const char *st){
    struct stat s;
    stat(st,&s);
    return (int)s.st_size;
}
int OrderWc::HandleWc_W(const char *st){
    int read_fd;
    if((read_fd=open(st,O_RDONLY))==-1){
        fprintf(stderr,"Open %s Error:%s\n",st,strerror(errno));
        exit(1);
    }
    int byte_read;
    char r[2];
    int sum=0;
    int flag=0;
    while (byte_read=read(read_fd,r,1)){
        if (strcmp(r," ")&&strcmp(r,"\t")&&strcmp(r,"\n")&&strcmp(r,"\r")&&strcmp(r,"\v")&&strcmp(r,"\f")&&!flag) {
            sum++;
            flag=1;
        }
        if (!strcmp(r," ") ||!strcmp(r,"\t")||!strcmp(r,"\n")||!strcmp(r,"\r")||!strcmp(r,"\v")||!strcmp(r,"\f")) flag=0;
    }
    close(read_fd);
    return sum;
}
int OrderWc::HandleWc_L(const char *st){
    int read_fd;
    if((read_fd=open(st,O_RDONLY))==-1){
        fprintf(stderr,"Open %s Error:%s\n",st,strerror(errno));
        exit(1);
    }
    int byte_read;
    char buffer[2];
    int sum=0;
    while (byte_read=read(read_fd,buffer,1)){
        if (!strcmp(buffer,"\n")) sum++;
    }
    close(read_fd);
    return sum;
}
/*结束处理wc的命令*/
