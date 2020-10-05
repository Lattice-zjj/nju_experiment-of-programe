#pragma once
#include <limits.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "Global.h"
#define PATH_SIZE 255
using namespace std;
void NowAddress(char *st); //获取当前的路径
string BackToPre(char *st); //返回到上一级目录
