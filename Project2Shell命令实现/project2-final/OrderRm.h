#include "Order.h"
#include "FileHelper.h"
#include <string>
#include <dirent.h>
#include <algorithm>
#include "Global.h"
class OrderRm:public Order{
public:
    OrderRm(){
        name="rm";
    }
    ~OrderRm(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    bool par_r,par_i;
    void RemoveAddress(string real,string input);
    void RemoveFile(string real,string input);
    bool InputYes();
};