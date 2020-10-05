#include "OrderLs.h"
#include "OrderMan.h"
void OrderLs::Address(vector<string> s){
	Splitoff(s);
	par_a=false;
	for (int i=0;i<parameter.size();i++){
		if (parameter[i]=="--help"){
			OrderMan order_now;
            vector<string> st;
            st.push_back(this->GetName());
            order_now.Address(st);
			return;
	    }
		int j=1;
		char pp;
		while (pp=parameter[i][j++]) {
			if (pp=='a') par_a=true;
			else {
				filehelper.ErrorOption(this,pp);
				return;
			}
		}
	}
	if (file.size()==0){
		this->ShowFile(NowPath);
	}
	if (file.size()==1){
		string st=file.at(0);
		filehelper.HandleWithAddress(st);
		if (!filehelper.FindFileError(st.c_str(),this,file.at(0).c_str())) return;
		this->ShowFile(st.c_str());
	}
	else {
		for (int i=0;i<file.size();i++){
			printf("%s:\n",file.at(i).c_str());
			string st=file.at(i);
			filehelper.HandleWithAddress(st);
			if (!filehelper.FindFileError(st.c_str(),this,file.at(i).c_str())) continue;
			this->ShowFile(st.c_str());
			printf("\n\n");
		}
	}

}


void OrderLs::ShowFile(const char *st){
	DIR *dir=opendir(st);
	dirent *filehelp;
	int max=0;
	while (filehelp=readdir(dir)) if (strlen(filehelp->d_name)>max) max=strlen(filehelp->d_name);
	dir=opendir(st);
	dirent *file;
	int num=0;
    while (file=readdir(dir)) {
		if (file->d_name[0]!='.'&& par_a==false || par_a==true){
			if (file->d_type==DT_DIR) printf("\033[1m\033[34m%-*s\033[0m",max+2,file->d_name);
			else printf("%-*s",max+2,file->d_name);
			num++;
			if (num==5){printf("\n");num=0;}
		}
    }
	printf("\n");
    closedir(dir);
}
