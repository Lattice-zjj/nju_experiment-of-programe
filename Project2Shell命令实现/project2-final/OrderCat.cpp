#include "OrderCat.h"
#include "OrderMan.h"
void OrderCat::Address(vector<string> s){
    Splitoff(s);
    par_n=par_b=par_s=par_v=par_E=par_T=false;
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
            if (pp=='n') par_n=true;
            else if (pp=='b') par_b=true;
            else if (pp=='s') par_s=true;
            else if (pp=='v') par_v=true;
            else if (pp=='E') par_E=true;
            else if (pp=='T') par_T=true;
            else if (pp=='A') {
                par_v=true;par_E=true;par_T=true;
            }
            else if (pp=='e'){
                par_v=true;par_E=true;
            }
            else if (pp=='t'){
                par_v=true;par_T=true;
            }
            else {
                filehelper.ErrorOption(this,pp);
                return;
            }
        }
    }
    for (int i=0;i<file.size();i++){
        HandleWithString(file[i]);
    }
}
void OrderCat::HandleWithString(string st){
    string infile=st;
    filehelper.HandleWithAddress(st);
    if (!filehelper.FindFileError(st.c_str(),this,infile.c_str())) return;
    if (filehelper.Is_a_directoryError(st.c_str(),this,infile.c_str())) return;
    ifstream file(st.c_str());
    string s;
    bool cur_kong,pre_kong=false;
    int num=0;
    while (getline(file, s)) {
        cur_kong=true;
        for (int i=0;i<s.length();i++){
            if (s[i]!=' '&&s[i]!='\t'&&s[i]!='\n') cur_kong=false;
        }
        if (par_s&&cur_kong&&pre_kong) continue;
        if (par_b&&!cur_kong||par_n&&!par_b) {
            num++;
            printf("%6d  ",num);
        }
        for (int i=0;i<s.length();i++){
            if (par_T && s[i]=='\t') printf("^I");
            else printf("%c",s[i]);
        }
        if (par_E) printf("$");
        printf("\n");
        pre_kong = cur_kong;
    }
    file.close();
}
