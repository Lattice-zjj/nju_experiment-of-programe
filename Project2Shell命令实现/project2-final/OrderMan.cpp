#include"OrderMan.h"
struct thread_data{
    string st;
    int num_all;
    string name;
};
std::map<string,int> memory;
void * makewin(void * s);
void OrderMan::Address(vector<string> s){
    for (int i=0;i<s.size();i++){
        string infile=s.at(i);
        string st="./man/"+s.at(i)+".txt";
        filehelper.HandleWithAddress(st);
        char t[PATH_MAX];
        if (!filehelper.FindFile(st.c_str(),t)){
            printf("No manual entry for %s\n",infile.c_str());
            continue;
        }
        int num_all_row=0;
        FILE* fp=fopen(st.c_str(),"r");
        ifstream file(st.c_str());
        while (!feof(fp)) {
            char sw[255];
            num_all_row++;
            fgets(sw,255,fp);
        }
        fclose(fp);
        thread_data sp;
        sp.num_all=num_all_row;sp.st=st;sp.name=s.at(i);
        pthread_t child_id;
        pthread_create(&child_id,NULL,makewin,(void *)&sp);
        pthread_join(child_id,NULL);
        
    }
}

void* makewin(void * s){
    thread_data* data=(thread_data *)s;
    WINDOW *w;
    refresh();
    w=initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr,true);
    refresh();
    int ch;
    int first=1;
    map<string, int>::iterator iter = memory.find(data->name);
    if (memory.end()!=iter) first=memory[data->name];else first=1;
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_WHITE);
    init_pair(2,COLOR_WHITE,COLOR_BLACK);
    while (1){
        int high_row=w->_maxy;
        int num_row=0;
        bool f=false;
        FILE* fp=fopen(data->st.c_str(),"r");
        //attron(COLOR_PAIR(2));
        while (!feof(fp)) {
            char sw[255];
            num_row++;
            if (num_row-first>high_row) break;
            fgets(sw,255,fp);
            if (num_row>=first) addstr(sw);
            refresh();
        }
        //attroff(COLOR_PAIR(2));
        fclose(fp);
        attron(COLOR_PAIR(1));
        mvwprintw(w,high_row,0,"Manual page %s(1) line %d/%d %d% (press q to quit)",data->name.c_str(),first,data->num_all,first*100/data->num_all);
        attroff(COLOR_PAIR(1));
        refresh();
        ch=getch();
        switch(ch){
            case KEY_UP:
                if (first==1) {clear();continue;}
                if (first>1) first--;
                break;
            case KEY_DOWN: 
                if (first==num_row-high_row) {clear();continue;}
                if (first<num_row-high_row) first++;
                break;
            case KEY_BACKSPACE: 
                if (first<=num_row-17) first+=17;else first=num_row;
                break;
            case 'q':
                f=true;
                clear();
                move(0, 0);
                break;
            default:
                break;
        }
        if (f) break;
        clear();
        refresh();
        move(0, 0);
    }
    memory[data->name]=first;
    refresh();
	endwin();
}