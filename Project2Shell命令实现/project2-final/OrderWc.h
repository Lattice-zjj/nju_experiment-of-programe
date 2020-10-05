#pragma once
#include "Global.h"
#include "Order.h"
#include "FileHelper.h"
using namespace std;
class OrderWc: public Order{
public:
    OrderWc(){
        name="wc";
    }
    ~OrderWc(){}
    void Address(vector<string> s);
    int HandleWc_C(const char *st);
    int HandleWc_W(const char *st);
    int HandleWc_L(const char *st);
private:
    FileHelper filehelper;
};