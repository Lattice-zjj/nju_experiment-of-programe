#include "OrderVim.h"
#include "OrderMan.h"
#include <math.h>
std::map<string,int> memory_vim;
void OrderVim::Address(vector<string> s){
    Splitoff(s);
    for (int i=0;i<parameter.size();i++){
        if (parameter[i]=="--help"){
            OrderMan order_now;
            vector<string> st;
            st.push_back(this->GetName());
            order_now.Address(st);
            return;
        }
    }
    if (file.size()==0){
        printf("%s: missing file name\n",name.c_str());
        printf("Try \'%s --help\' for more information.\n",name.c_str());
        return;
    }
    for (int i=0;i<file.size();i++){
        string st=file.at(i);
        filehelper.HandleWithAddress(st);
        CreateFile(st);
    }
}

void OrderVim::CreateFile(string st){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr,true);
    refresh();
    char t[PATH_MAX];
    vector<string> buffer;
    if (filehelper.FindFile(st.c_str(),t)){
        ifstream onefile;
        onefile.open(st.c_str());
        string sw;
        while (getline(onefile,sw)) {
            buffer.push_back(sw);
        }
        onefile.close();
    }
    if (buffer.size()==0) buffer.push_back("");
    int x = 3;
    int y = 0;
    int ch;
    int high_row=0;
    bool flag=false;
    int first=0;
    int rfirst=1;
    map<string, int>::iterator iter = memory_vim.find(st);
    if (memory_vim.end()!=iter) rfirst=memory_vim[st];else rfirst=1;
    bool modify=false;
    bool able=false;
    bool save;
    string par="";
    int parx=0;
    while(1) {
        if (high_row!=stdscr->_maxy-1 || rfirst!=first || modify){
            clear();
            high_row=stdscr->_maxy-1;
            for (int i=rfirst-1;i<=min(high_row+rfirst-1,(int)buffer.size()-1);i++){
                mvprintw(i-(rfirst-1),0,"%2s ",to_string(i+1).c_str());
            }
            wmove(stdscr,0,3);
        }
        if (rfirst!=first || modify){
            first=rfirst;
            modify=false;
            int num_row=0;
            while (1) {
                num_row++;
                if (num_row-1>buffer.size()) break;
                if (num_row-first>high_row) break;
                if (num_row>=first) {
                    wmove(stdscr,num_row-first,3);addstr(buffer[num_row-1].c_str());
                }
                refresh();
            }
            mvprintw(high_row+1,stdscr->_maxx-9,"%d,%d   ALL   ",rfirst+y,x-2);
            if (able) mvprintw(high_row+1,0,"--INSERT--");
            move(y,x);
        }
        ch = getch();
        if ((ch=='i' || ch=='I') && !able) {
            able=true;
            if (able) mvprintw(high_row+1,0,"--INSERT--");
            move(y,x);
            par="";parx=0;
            continue;
        }
        if (!able){
            if (par.size()==0){
                bool t=false;
                switch(ch) {
                case KEY_HOME:
                    x=3;
                    t=true;
                    break;
                case KEY_END:
                    x=buffer[y+rfirst-1].size()+3;
                    t=true;
                    break;
                case KEY_UP:
                    if (y>0) --y;else {
                        if (rfirst>1) rfirst--;
                    }
                    if (x>buffer[y+rfirst-1].size()+3) x=buffer[y+rfirst-1].size()+3; 
                    t=true;
                    break;
                case KEY_DOWN: 
                    if (y<high_row && y<buffer.size()-1) {
                        ++y; 
                    }else if (y==high_row) {
                        if (rfirst<buffer.size()-high_row)rfirst++;
                    }
                    if (x>buffer[y+rfirst-1].size()+3) x=buffer[y+rfirst-1].size()+3;
                    t=true;
                    break;
                case KEY_RIGHT: 
                    if (x<buffer[y+rfirst-1].size()+3) ++x; 
                    t=true;
                    break;
                case KEY_LEFT: 
                    if (x>3) --x; 
                    t=true;
                    break;
                }
                if (t){
                    mvprintw(high_row+1,stdscr->_maxx-9,"%d,%d   ALL   ",rfirst+y,x-2);
                    wmove(stdscr,y,x);
                    continue;
                }
            }
            switch (ch)
            {
            case KEY_BACKSPACE:
                if (parx>0){
                    par.erase(parx-1);
                    parx--;
                    mvaddch(high_row+1,parx,' ');
                }
                break;
            case KEY_HOME:
                parx=0;
                break;
            case KEY_END:
                parx=par.length();
                break;
            case KEY_UP:
                break;
            case KEY_DOWN:
                break;
            case KEY_LEFT:
                if (parx>0) parx--; 
                break;
            case KEY_RIGHT:
                if (parx<par.length()) parx++;
                break;
            case ' ':
                break;
            case '\t':
                break;
            case '\n':
                if (par==":wq") {
                    save=true;
                    flag=true;
                }else if (par==":q!"){
                    save=false;
                    flag=true;
                }
                break;
            default:
                par.insert(parx,1,ch);
                parx++;
                mvprintw(high_row+1,0,par.c_str());
                break;
            }
            wmove(stdscr,high_row+1,parx);
            if (flag) break;
            continue;
        }
        switch(ch) {
        case KEY_HOME:
            x=3;
            break;
        case KEY_END:
            x=buffer[y+rfirst-1].size()+3;
            break;
        case KEY_UP:
            if (y>0) --y;else {
                if (rfirst>1) rfirst--;
            }
            if (x>buffer[y+rfirst-1].size()+3) x=buffer[y+rfirst-1].size()+3; 
            break;
        case KEY_DOWN: 
            if (y<high_row && y<buffer.size()-1) {
                ++y; 
            }else if (y==high_row) {
                if (rfirst<buffer.size()-high_row)rfirst++;
            }
            if (x>buffer[y+rfirst-1].size()+3) x=buffer[y+rfirst-1].size()+3; 
            break;
        case KEY_RIGHT: 
            if (x<buffer[y+rfirst-1].size()+3) ++x; 
            break;
        case KEY_LEFT: 
            if (x>3) --x; 
            break;
        case ' ':
            if (able){
                buffer[rfirst+y-1].insert(x-3,1,' ');
                modify=true;
                x++;
            }
            break;
        case '\t':
            if (able) {
                buffer[rfirst+y-1].insert(x-3,"       ");
                x+=7;
                modify=true;
            }
            break;
        case '\n':
            if (able) {
                string t=buffer[rfirst+y-1];
                buffer[rfirst+y-1]=t.substr(0,x-3);
                buffer.insert(buffer.begin()+rfirst+y,t.substr(x-3));
                modify=true;
                if (y<high_row){ y++;x=3;wmove(stdscr,y,x);} else {rfirst++;x=3;}
            }
            break;
        case KEY_BACKSPACE: 
            if (able) {
                if (x>3) {
                    buffer[rfirst+y-1].erase(x-4,1);modify=true;
                    mvaddch(y,--x,' ');
                    refresh();
                }
                else if (y>0) {
                    if (y==high_row && rfirst>1) {
                        rfirst--;
                    }
                    else y--;
                    x=buffer[rfirst+y-1].size()+3;
                    buffer[rfirst+y-1]+=buffer[rfirst+y];
                    buffer.erase(buffer.begin()+rfirst+y);
                    modify=true;
                    refresh();
                }
                else {
                    if (rfirst>1) {
                        rfirst--;
                        x=buffer[rfirst+y-1].size()+3;
                        buffer[rfirst+y-1]+=buffer[rfirst+y];
                        buffer.erase(buffer.begin()+rfirst+y);
                        modify=true;
                    }
                }
            }
            break;
        case 27: 
            able=false;
            mvprintw(high_row+1,0,"          ");
            refresh();
            break;
        default:
            if (able) {
                buffer[rfirst+y-1].insert(x-3,1,ch);
                modify=true;
                x++;
            };
            break;
        }
        mvprintw(high_row+1,stdscr->_maxx-9,"%d,%d   ALL   ",rfirst+y,x-2);
        wmove(stdscr,y,x);
        if (flag) break;
    }
    refresh();
	endwin();
    if (save){
        if (filehelper.FindFile(st.c_str(),t)) fclose(fopen(st.c_str(), "w"));
        ofstream out_file(st,ios::out);
        for (int i=0;i<buffer.size();i++){
            buffer[i]+="\n";
            out_file<<buffer[i];
        }
        out_file.close();
    }
    memory_vim[st]=rfirst;
}