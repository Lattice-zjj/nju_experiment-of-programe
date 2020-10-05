#include "Student.h"

Student::Student() {
    num_cho = 0;
    for (int i = 0; i < 100; i++) {
        cho.push_back(0);ass.push_back("");
    }
    zhuannum = 0;
    nozhuannum = 0;
    num_ass = 0;
    num_of_login=0;
    if_lock=false;
}

Student::~Student() {

}
Student::Student(QString cid, QString cpa) {
    id=cid;password=cpa;
    num_cho = 0;
    for (int i = 0; i < 100; i++) {
        cho.push_back(0);ass.push_back("");
    }
    num_ass = 0;
    zhuannum = 0;
    nozhuannum = 0;
    num_of_login=0;
    if_lock=false;
}
bool Student::Login(QString cid, QString cpassword) {
    if (id==cid && password==cpassword) {
        ifLogin = true;
        return true;
    }
    else {
        if (id==cid){
            num_of_login++;
        }
        return false;
    }
}
bool Student::Logout() {
    if (ifLogin) {
        ifLogin = false;
        return true;
    }
    else return false;
}

bool Student::choose(int cid) {
    for (int i = 0; i < num_cho; i++) {
        if (cho[i] == cid) return true;
    }
    return false;
}
