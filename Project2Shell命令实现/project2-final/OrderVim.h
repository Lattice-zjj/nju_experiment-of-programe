#pragma once
#include "Order.h"
#include "FileHelper.h"
#include <algorithm>
#include <dirent.h>
#include <curses.h>
#include <stdio.h>
#include "Address.h"
#include "Global.h"
#define BUFFER_SIZE 1024
class OrderVim: public Order{
public:
    OrderVim(){
        name="vim";
    }
    ~OrderVim(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    void CreateFile(string st);
};
