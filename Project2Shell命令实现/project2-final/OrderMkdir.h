#include "Order.h"
#include "Global.h"
#include "FileHelper.h"
#include <iostream>
using namespace std;
extern string BackToPre(char *st);
class OrderMkdir: public Order {
public:
    OrderMkdir(){ name="mkdir";}
    ~OrderMkdir(){}
    void Address(vector<string> s);
private:
    FileHelper filehelper;
    bool par_p;
    bool CreateManyDir(char *st);
};