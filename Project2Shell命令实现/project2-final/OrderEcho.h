#include <iostream>
#include "Order.h"
#include "FileHelper.h"
class OrderEcho: public Order
{
public:
    OrderEcho(){
		name="echo";
	}
	~OrderEcho() {}
	void Address(vector<string> s);
private:
    FileHelper filehelper;
};