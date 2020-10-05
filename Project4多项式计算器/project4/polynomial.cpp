#include "polynomial.h"
Polynomial::Polynomial() {
	num = 0;
	for (int i = 0; i < 200; i++) a[i] = 0;
}
Polynomial::Polynomial(const Polynomial& p) {
	num = p.num;
	for (int i = 0; i < num; i++) a[i] = p.a[i];
}
Polynomial::Polynomial(const double w[], const int n) {
	num = n;
	for (int i = 0; i < n; i++) a[i] = w[i];
}
Polynomial Polynomial::reverse() const{
	Polynomial t;
	t.num=num;
	for (int i=0;i<num;i++) t.a[i]=a[num-1-i];
	return t;
}
void Polynomial::simplify(){
	for (int i=0;i<num;i++) if (fabs(a[i])<1e-6) a[i]=0.0;
	while (fabs(a[num-1])<1e-6 && num>1) num--;
}
Polynomial Polynomial::mod(int n) const{
    double b[200];
    for (int i = 0; i < n; ++i) b[i] = a[i];
    return Polynomial(b, n);
}
Polynomial Polynomial::operator+(const Polynomial& p)const {
	Polynomial t;
	if (num > p.num) {
		t.num = num;
		for (int i = 0; i < p.num; i++) t.a[i] = p.a[i] + a[i];
		for (int i = p.num; i < t.num; i++) t.a[i] = a[i];
	}
	else {
		t.num = p.num;
		for (int i = 0; i < num; i++) t.a[i] = p.a[i] + a[i];
		for (int i = num; i < t.num; i++) t.a[i] = p.a[i];
	}
	return t;
}
Polynomial Polynomial::operator-(const Polynomial& p)const {
	Polynomial t;
	if (num > p.num) {
		t.num = num;
		for (int i = 0; i < p.num; i++) t.a[i] = a[i] - p.a[i];
		for (int i = p.num; i < t.num; i++) t.a[i] = a[i];
	}
	else {
		t.num = p.num;
		for (int i = 0; i < num; i++) t.a[i] = a[i] - p.a[i];
		for (int i = num; i < t.num; i++) t.a[i] = -p.a[i];
	}
	t.simplify();
	return t;
}
Polynomial Polynomial::operator*(const Polynomial& p) {
	Polynomial t;
	t.num = p.num + num;
	complex aa[200];complex bb[200];
	int limit=1;
	int n=num-1,m=p.num-1;
	for (int i=0;i<=n;i++) {
		aa[i]=complex(this->a[i]);
	}
	cout<<endl;
	for (int i=0;i<=m;i++) {
		bb[i]=complex(p.a[i]);
	}
	while (limit<=n+m) limit<<=1;
	fft(limit,aa,1);
	fft(limit,bb,1);
	for (int i=0;i<=limit;i++) aa[i]=aa[i]*bb[i];
	fft(limit,aa,-1);
	
	for (int i=0;i<=n+m;i++) if (fabs(aa[i].real()/limit)<1e-6) t.a[i]=0;else t.a[i]=aa[i].real()/limit;
	return t;
}
double Polynomial::operator()(double x) const {
	double t = 1, s = 0;
	for (int i = 0; i < num; i++) {
		s = s + t * a[i];
		t = t * x;
	}
	return s;
}
Polynomial Polynomial::derivative() const {
	Polynomial t;
	if (num==1) {
		t.num=1;t.a[0]=0;
		return t;
	}
	t.num=num-1;
	for (int i = 1; i < num; i++) t.a[i-1]=a[i]*i;
	return t;
}

Polynomial Polynomial::integrate() const{
	Polynomial t;
	t.num=num+1;
	for (int i=0;i<num;i++) t.a[i+1]=a[i]/(i+1);
	return t;
}

Polynomial Polynomial::integrate(double a,double b) const{
	Polynomial t=this->integrate();
	Polynomial ans;ans.num=1;ans.a[0]=t(b)-t(a);
	return ans;
}

pair<bool,Polynomial> Polynomial::inversion(){
	Polynomial b;b.num=num;
	if (a[0]==0) return pair<bool,Polynomial>(false,Polynomial());
	//cout<<a[0];
	b.a[0]=1.0/a[0];
	for (int i=1;i<num;i++) {
		for (int j=0;j<=i-1;j++) b.a[i]=b.a[i]+b.a[j]*(-a[i-j]/a[0]);
	}
	return pair<bool,Polynomial>(true,b);
}

pair<Polynomial,Polynomial> Polynomial::divide(Polynomial& p){
	Polynomial p1= reverse(), p2= p.reverse();
    int n = p1.num, m = p2.num;
    Polynomial inv = p2.inversion().second;
    Polynomial shang=(p1*inv).mod(n-m+1).reverse();
	return pair<Polynomial,Polynomial>(shang,(*this)-(shang*p));
}

pair<bool,double> Polynomial::get(){
	Polynomial d=this->derivative();
	bool f=false;
    double guess=0,ans=0;
    for (int k=0;k<10;++k) {
        double t=guess;
        for (int g=0;g<100;++g) {
            double p_value=(*this)(t);
            if (fabs(p_value)<1e-6) {
                ans=t;f=true;
                break;
            }
            double d_value=d(t);
            if (d_value==0) break;
            t=t-p_value/d_value;
        }
        if (f) break;
        if (guess>0) { ++guess;guess=-guess;} else {--guess;guess = -guess;}
    }
    return pair<bool,double>(f,ans);
}
Polynomial operator*(const double x, const Polynomial& p) {
	Polynomial t;
	t.num = p.num;
	for (int i = 0; i < p.num; i++) t.a[i] = p.a[i] * x;
	return t;
}

ostream& operator <<(ostream &os, Polynomial& p){
    int f=1;
    for (int i=p.num-1;i>=0;i--){
        double d=p.a[i];
        if (!i&&f&&!d) os<<"0";
        if (!d) continue;
        if (d<0) {os<<" - ";d=-d;} else if(!f) os<<" + ";
        if (!(fabs(d-1)<1e-6)||!i) os<<d;
        if (i) os<<"x";
        if (i>1) os<<"^"<<i;
        f=0;
    }
	return os;
}

void Polynomial::fft(int limit,complex *ab,int type)
{
	double Pi=acos(-1.0);
    if(limit==1) return ;//只有一个常数项
    complex a1[limit>>1];complex a2[limit>>1];
    for(int i=0;i<=limit;i+=2)//根据下标的奇偶性分类
        a1[i>>1]=ab[i],a2[i>>1]=ab[i+1];
    fft(limit>>1,a1,type);
    fft(limit>>1,a2,type);
    complex Wn=complex(cos(2.0*Pi/limit),type*sin(2.0*Pi/limit)),w=complex(1,0);
    //Wn为单位根，w表示幂
    for(int i=0;i<(limit>>1);i++,w=w*Wn) {
        ab[i]=a1[i]+w*a2[i];
        ab[i+(limit>>1)]=a1[i]-w*a2[i];//利用单位根的性质，O(1)得到另一部分 
	}
}

