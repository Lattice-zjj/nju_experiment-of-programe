#pragma once
#include "Order.h"
#include "FileHelper.h"
#include <algorithm>
#include <dirent.h>
#include "Address.h"
#include "Global.h"
#define BUFFER_SIZE 1024
class OrderCp: public Order{
public:
    OrderCp(){
        name="cp";
    }
    ~OrderCp(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    bool par_r,par_i;
    void CopyFile(const char *source,const char *outcome);//复制文件
    void CopyAddress(const char* source,const char *outcome);//复制目录
    vector<string> split(char* strc){
        char sst[PATH_MAX];
        strcpy(sst,strc);
        vector<string> resultVec;
        char* tmpStr = strtok(sst, "/");
        while (tmpStr != NULL)
        {
            string st=string(tmpStr);
            resultVec.push_back(st);
            tmpStr = strtok(NULL, "/");
        }
        return resultVec;
    }
};
