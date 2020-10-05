#include "Course.h"

Course::Course() {
    id = 0; selected = 0; capacity = 0;
    name="";teacher="";type="";
    note="";
}
Course::Course(int cid,QString cna,QString cte,int cca,int cse,QString ctp,QString cnote){
    id=cid;name=cna;teacher=cte;capacity=cca;selected=cse;type=ctp;note=cnote;
}
Course::~Course(){

}
