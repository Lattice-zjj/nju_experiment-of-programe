#pragma once
#include "Order.h"
#include "Global.h"
#include "FileHelper.h"
class OrderCmp: public Order{
public:
    OrderCmp(){
        name="cmp";
    }
    ~OrderCmp(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    int tran(int x){
        int ans=0;int k=1;
        while (x) {
            ans+=(x%8)*k;k=k*10;x=x/8;
        }
        return ans;
    }
    bool par_b,par_l;
};