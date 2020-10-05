#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "complex.h"
using namespace std;
class Polynomial
{
public:
	Polynomial();
	Polynomial(double p){num=1;a[0]=p;}
	Polynomial(const Polynomial& p);//拷贝构造函数
	Polynomial(const double w[], const int n);//传入n及w函数的构造函数
	Polynomial operator+(const Polynomial& p)const;//对+进行重载
	Polynomial operator-(const Polynomial& p)const;//对+进行重载
	Polynomial operator*(const Polynomial& p);//对*进行重载
	Polynomial derivative() const;//求导
	Polynomial integrate(double a,double b) const;//积分
	pair<bool,Polynomial> inversion();//求逆元
	pair<Polynomial,Polynomial> divide(Polynomial& p);//除法
	pair<bool,double> get();//获得根，第一个数字代表是否有根，第二代表根是啥
	double operator()(double x) const;//重载()直接计算f(x)
	friend Polynomial operator*(const double x,const Polynomial& p);//数字和多项式的乘法
    friend ostream& operator <<(ostream &os, Polynomial& p); //重载<<便于输出
	int getnum() const{return num;}
	vector<double> getxi() { vector<double> t;for (int i=0;i<num;i++) t.push_back(a[i]);return t;}
private:
	Polynomial integrate() const;
	void fft(int limit,complex *a,int type);
	Polynomial reverse() const;
	void simplify();
	Polynomial mod(int n) const;
	double a[200];
	int num;
};
