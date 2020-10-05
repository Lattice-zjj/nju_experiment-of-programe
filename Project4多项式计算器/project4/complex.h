#ifndef __COMPLEX__
#define __COMPLEX__

#include<iostream>
#include<cmath>
using namespace std;
class complex;
complex&
  __doapl (complex* ths,const complex& r);

class complex
{
public://函数绝大部分 
	complex(double r = 0, double i = 0)   //构造函数
		: re(r), im(i)              //初始化 
	{}
	complex& operator += (const complex&);//传回reference 
	double real() const { return re; }//不改变值尽量加const 
	double imag() const { return im; }

private://数据存放的地方 
	double re, im;
	friend complex& __doapl(complex*, const complex&);
};

inline complex&
__doapl(complex* ths, const complex& r)
{
	ths->re += r.re;
	ths->im += r.im;
	return *ths;
}

inline complex&
complex::operator += (const complex & r)
{
	return __doapl(this, r);
}

//虚部
inline double
imag(const complex& x)
{
	return x.imag();
}
//实部
inline double
real(const complex& x)
{
	return x.real();
}
//+重载
inline complex
operator + (const complex& x, const complex& y)
{
	return complex(real(x) + real(y), imag(x) + imag(y));
}

inline complex
operator + (const complex& x, double y)
{
	return complex(real(x) + y, imag(x));
}

inline complex
operator + (double x, const complex& y)
{
	return complex(x + real(y), imag(y));
}
//-重载
inline complex
operator - (const complex& x, const complex& y)
{
	return complex(real(x) - real(y), imag(x) - imag(y));
}

inline complex
operator - (const complex& x, double y)
{
	return complex(real(x) - y, imag(x));
}

inline complex
operator - (double x, const complex& y)
{
	return complex(x - real(y), imag(y));
}
//*重载
inline complex
operator * (const complex& x, const complex& y)
{
	return complex(real(x) * real(y) - imag(x) * imag(y), real(x) * imag(y) + real(y) * imag(x));
}

inline complex
operator * (const complex& x, double y)
{
	return complex(real(x) * y, imag(x) * y);
}

inline complex
operator * (double x, const complex& y)
{
	return complex(x * real(y), x * imag(y));
}
// /重载
inline complex
operator / (const complex& x, const complex& y)
{
	return complex((real(x) * real(y) + imag(x) * imag(y)) / (real(y) * real(y) + imag(y) * imag(y)), (imag(x) * real(y) - imag(y) * real(x)) / (real(y) * real(y) + imag(y) * imag(y)));
}

inline complex
operator / (const complex& x, double y)
{
	return complex(real(x) / y, imag(x) / y);
}

inline complex
operator / (double x, const complex& y)
{
	return complex((x * real(y) / (real(y) * real(y) + imag(y) * imag(y))), (-x * imag(y) / (real(y) * real(y) + imag(y) * imag(y))));
}
//正号重载
inline complex
operator + (const complex& x)
{
	return x;
}
//负号重载
inline complex
operator - (const complex& x)
{
	return complex(-real(x), -imag(x));
}
//==重载
inline bool
operator == (const complex& x, const complex& y)
{
	return real(x) == real(y) && imag(x) == imag(y);
}

inline bool
operator == (const complex& x, double y)
{
	return real(x) == y && imag(x) == 0;
}

inline bool
operator == (double x, const complex& y)
{
	return x == real(y) && imag(y) == 0;
}
//!=重载
inline bool
operator != (const complex& x, const complex& y)
{
	return real(x) != real(y) || imag(x) != imag(y);
}

inline bool
operator != (const complex& x, double y)
{
	return real(x) != y || imag(x) != 0;
}

inline bool
operator != (double x, const complex& y)
{
	return x != real(y) || imag(y) != 0;
}
//共轭
inline complex
operator ~(const complex& x)
{
	return complex(real(x), -imag(x));
}



#endif