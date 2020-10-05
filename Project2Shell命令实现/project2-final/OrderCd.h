#include "Order.h"
#include "Global.h"
#include "FileHelper.h"
#include <iostream>
using namespace std;
class OrderCd: public Order{
public:
    OrderCd(){
        name="cd";
    }
    ~OrderCd(){}
    void Address(vector<string>);
private:
    FileHelper filehelper;
    
};