#pragma once
#include "Order.h"
#include "FileHelper.h"
#include "Global.h"
#include <dirent.h>
#include <iostream>
#include <sys/io.h>
class OrderLs: public Order{
public:
	OrderLs(){
		name="cat";
	}
	~OrderLs() {}
	void Address(vector<string> s);
private:
	FileHelper filehelper;
	bool par_a,par_l,par_r,par_t,par_A,par_F,par_R;
	void ShowFile(const char *);

};
