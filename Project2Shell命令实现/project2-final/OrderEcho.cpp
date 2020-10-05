#include "OrderEcho.h"
#include "OrderMan.h"
void OrderEcho::Address(vector<string> s){
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
			filehelper.ErrorOption(this,pp);
			return;
		}
	}
	string ans="";
    for (int i=0;i<s.size();i++) ans=ans+s.at(i)+" ";
    for (int i=0;i<ans.length();i++){
        if (ans[i]=='\"'&&(i==0 ||i>0&& ans[i-1]!='\\' || i==ans.length()-1 )) continue;
        printf("%c",ans[i]);
    }
    printf("\n");
}