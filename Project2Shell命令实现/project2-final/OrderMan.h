#pragma once
#include "Order.h"
#include "Global.h"
#include "FileHelper.h"
#include <unistd.h>
#include <curses.h>
#include <string>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <iostream>
#include <map>
using namespace std;
class OrderMan: public Order{
public:
    OrderMan(){
        name="man";
    }
    ~OrderMan(){
    }
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    
};
