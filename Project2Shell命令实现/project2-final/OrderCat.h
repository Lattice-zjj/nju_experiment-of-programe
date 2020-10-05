#pragma once
#include "Order.h"
#include "FileHelper.h"
#include "Global.h"
class OrderCat: public Order{
public:
    OrderCat(){
        name="cat";
    }
    ~OrderCat(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    bool par_n,par_b,par_s,par_v,par_E,par_T;
    void HandleWithString(string st);
};
