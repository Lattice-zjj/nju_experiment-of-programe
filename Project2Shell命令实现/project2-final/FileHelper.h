#pragma once
#include "Order.h"
#include "Global.h"
extern string BackToPre(char *st);
using namespace std;
class FileHelper{
public:
    bool Is_a_directory(const char* st); //是否是一个目录
    bool FindFile(const char *s,char* st); //获取绝对路径存到st中
    bool FindFileError(const char *s,Order *t,const char *infile);  //判断是否存在并打印错误信息
    bool Is_a_directoryError(const char* st,Order *t,const char *infile); //判断是否是目录，不是就打印错误信息
    void ErrorOption(Order *t,const char st);  //错误参数打印错误信息
    void HandleWithAddress(string& st); //处理输入的地址
};