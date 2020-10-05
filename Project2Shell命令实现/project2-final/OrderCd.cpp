#include "OrderCd.h"
#include "OrderMan.h"
void OrderCd::Address(vector<string> s){
    Splitoff(s);
    for (int i=0;i<parameter.size();i++){
        if (parameter[i]=="--help"){
            OrderMan order_now;
            vector<string> st;
            st.push_back(this->GetName());
            order_now.Address(st);
            return;
        }
        else printf("bash: cd: -%c: invalid option",parameter[i][1]);
        return;
    }
    if (file.size()==0){
        string user="/home/"+string(UserName);
        strcpy(NowPath,user.c_str());
        return;
    }
    if (file.size()>1){
        printf("bash: %s: too many arguments\n",this->name.c_str());
        return;
    }
    string st=file.at(0);
    filehelper.HandleWithAddress(st);
    if (!filehelper.FindFileError(st.c_str(),this,file.at(0).c_str())) return;
    strcpy(NowPath,st.c_str());
}