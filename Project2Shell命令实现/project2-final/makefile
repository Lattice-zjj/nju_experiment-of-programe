target=main
source=$(wildcard *.cpp)
OBJS = $(source:%.cpp=%.o)

$(target): $(OBJS)
	g++ -std=c++11 -o $(target) $(OBJS) -l curses -lpthread
%.o:%.cpp
	g++ -std=c++11 -c -o $@ $< -l curses -lpthread

clean:
	rm *.o
	rm main
