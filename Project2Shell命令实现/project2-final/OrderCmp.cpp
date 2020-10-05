#include "OrderCmp.h"
#include "OrderMan.h"
using namespace std;
void OrderCmp::Address(vector<string> s){
    par_b=false;par_l=false;
    Splitoff(s);
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
            if (pp=='b') par_b=true;
            else if (pp=='l') par_l=true;
            else {
                filehelper.ErrorOption(this,pp);
                return;
            }
        }
    }
    if (file.size()<2){
        printf("the number of value is invalid\n");
        return;
    }
    else if (file.size()>2){
        printf("%s: invalid --ingnore-initial value \'%s\'\n",name.c_str(),file.at(2).c_str());
        printf("%s: Try \'%s --help\' for more information.\n",name.c_str(),name.c_str());
    }
    string s1,s2;
    s1=file.at(0);s2=file.at(1);
    filehelper.HandleWithAddress(s1);
    filehelper.HandleWithAddress(s2);
    if (!filehelper.FindFileError(s1.c_str(),this,file.at(0).c_str())) return;
    if (!filehelper.FindFileError(s2.c_str(),this,file.at(1).c_str())) return;
    if (filehelper.Is_a_directoryError(s1.c_str(),this,file.at(0).c_str())) return;
    if (filehelper.Is_a_directoryError(s2.c_str(),this,file.at(1).c_str())) return;
    int read1_fd,read2_fd;
    if((read1_fd=open(s1.c_str(),O_RDONLY))==-1){
        fprintf(stderr,"Open %s Error:%s\n",s1.c_str(),strerror(errno));
        exit(1);
    }
    if((read2_fd=open(s2.c_str(),O_RDONLY))==-1){
        fprintf(stderr,"Open %s Error:%s\n",s2.c_str(),strerror(errno));
        exit(1);
    }
    int byte_read1,byte_read2;
    char r1[2],r2[2];
    int num=0,line=1;
    while (1){
        byte_read1=read(read1_fd,r1,1);
        byte_read2=read(read2_fd,r2,1);
        if (byte_read1==0) break;
        if (byte_read1!=byte_read2) break;
        num++;
        if (r1[0]=='\n' && r2[0]=='\n') line++;
        if (r1[0]!=r2[0]) {
            int t1 = tran(r1[0]),t2=tran(r2[0]);
            string s1(r1);string s2(r2);
            if (r1[0]=='\n') s1="^J";else s1[1]='\0';
            if (r2[0]=='\n') s2="^J";else s2[1]='\0';
            if (par_b) {
                if (par_l){
                    printf("%-4d %-4d%-3s %-4d%-3s\n",num,t1,s1.c_str(),t2,s2.c_str());
                }
                else{
                    printf("%s %s differ: byte %d, line %d is  %d %s  %d %s\n",file.at(0).c_str(),file.at(1).c_str(),num,line,t1,s1.c_str(),t2,s2.c_str());
                    break;
                }
            } 
            else {
                if (par_l){
                    printf("%-4d %-4d %-4d\n",num,t1,t2);
                }
                else{
                    printf("%s %s differ: byte %d, line %d\n",file.at(0).c_str(),file.at(1).c_str(),num,line);
                    break;
                }
            }
        }
    }
    if (byte_read1!=byte_read2){
        if (byte_read1==0){
            if (r2[0]=='\n') line--;
            printf("%s: EOF on %s after byte %d, in line %d\n",name.c_str(),file.at(0).c_str(),num,line);
        }else{
            if (r1[0]=='\n') line--;
            printf("%s: EOF on %s after byte %d, in line %d\n",name.c_str(),file.at(1).c_str(),num,line);
        }
    }
    close(read1_fd);
    close(read2_fd);
}
