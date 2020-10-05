#pragma once
#include <iostream>
#include <algorithm>
#include "polynomial.h"
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <ctype.h>
#include <cstdio>
#include <string.h>
using namespace std;
class Control{
public:
    Control();
    void run();
private:
    map<string,int> ans;
    vector<Polynomial> data;
    Polynomial outcome_p;
    void init();
    bool IfRepeatDo(string oper); //询问是否重复做当下的事情 oper为当下要做的事的中文
    int manage(string st);
    void putinto(); //输入多项式
    void calculate(); //对输入的表达式进行计算
    void putout(); //输出多项式
    void divide();//求多项式除法的
    void inversion();//求多项式逆元的
    bool helpexist(string name,string help);//判断这个name存不存在并输出对应help的错误信息
    void getright();//得到多项式的根
    bool judgest(string);//判断是否满足$[a,b]的形式
    pair<double,double> dealst(char *str); //对传入的$[a,b]型分解
    void saveoutcome(Polynomial s);
    bool ask(string st);
    pair<bool,double> char_to_num(char* s);//将字符型数据转换成double型数据的函数
    bool judgename(string);
    void savedata();
    void readdata();
};