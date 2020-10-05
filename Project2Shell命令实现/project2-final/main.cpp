#include <iostream>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Address.h"
#include "Handle.h"
using namespace std;
char NowPath[PATH_MAX]="";
char UserName[PATH_MAX]="";
int main(){
    NowAddress(NowPath); //获取当前的路径
    char t[PATH_MAX];
    while(1){
        string out(NowPath);
        strcpy(t,NowPath);
        vector<string> ww=split_xie(t); //以斜杠为分割符
        if (ww.size()>=2) {
            //防止用户在不同的用户名之间切换，实时获取当前的用户
            strcpy(UserName,ww.at(1).c_str()); 
            //替换/home/用户名为~与linux实现无缝对接
            out.replace(0,out.find(string(UserName))+strlen(UserName),"~"); 
        }
        //带颜色打印提示信息
        printf("\033[1m\033[32m%s@zjj\033[37m:\033[34m%s\033[37m$ \033[0m",UserName,out.c_str()); 
        char st[PATH_MAX];
        cin.getline(st,PATH_MAX); //获取字符串
        HADNDLE(st); //处理字符串
    }
}
