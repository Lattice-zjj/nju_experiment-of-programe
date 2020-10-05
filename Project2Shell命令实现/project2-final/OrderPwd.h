#include "Global.h"
#include "Order.h"
#include "FileHelper.h"
#include "OrderMan.h"
class OrderPwd: public Order{
public:
	OrderPwd(){
		name="pwd";
	}
	~OrderPwd() {}
	void Address(vector<string> s){
        Splitoff(s);
        par_L=false;par_P=false;
        for (int i=0;i<parameter.size();i++){
		    if (parameter[i]=="--help"){
				OrderMan order_now;
           		std::vector<string> st;
            	st.push_back(this->GetName());
            	order_now.Address(st);
				return;
	        }
		    int j=1;
		    char pp;
		    while (pp=parameter[i][j++]) {
			    if (pp=='L') par_L=true;
			    else if (pp=='P') par_P=true;
			    else {
				    filehelper.ErrorOption(this,pp);
				    return;
			    }
		    }
	    }
        printf("%s\n",NowPath);
    }
private:
    bool par_P,par_L;
    FileHelper filehelper;
};