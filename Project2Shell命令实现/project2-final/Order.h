#pragma once
#include<vector>
#include <iostream>
using namespace std;
class Order{
protected:
    string name;
    vector<string> parameter;
    vector<string> file;
    void Splitoff(vector<string> st){
        for (int i=0;i<st.size();i++){
            if (st[i][0]=='-') parameter.push_back(st[i]);else file.push_back(st[i]);
        }
    }
public:
    Order(){}
    ~Order(){}
    string GetName(){
        return name;
    }
    virtual void Address(vector<string>)=0;
};