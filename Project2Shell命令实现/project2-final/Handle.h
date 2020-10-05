#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "FileHelper.h"
#include "Global.h"
#include "Order.h"
#include "OrderCat.h"
#include "OrderCmp.h"
#include "OrderCp.h"
#include "OrderWc.h"
#include "OrderMan.h"
#include "OrderLs.h"
#include "OrderCd.h"
#include "OrderPwd.h"
#include "OrderMkdir.h"
#include "OrderEcho.h"
#include "OrderRm.h"
#include "OrderVim.h"
using namespace std;
void HADNDLE(char *st);
void logout();
void HandleString(char *st);
vector<string> split(char* strc);
vector<string> split_xie(char* strc);
int max_same(string s1,string s2);
bool judge(string);