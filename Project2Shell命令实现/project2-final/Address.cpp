#include "Address.h"
void NowAddress(char *st){
    if(!getcwd(st,PATH_SIZE)){
        printf("Get path fail!\n");
	}
}
string BackToPre(char *st){
    string s(st);
    while (s[s.size()-1]!='/') s.pop_back();
    if (s.size()>1) s.pop_back();
    return s;
}