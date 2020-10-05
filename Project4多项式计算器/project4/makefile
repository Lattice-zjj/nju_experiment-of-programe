target=main
source=$(wildcard *.cpp)
OBJS = $(source:%.cpp=%.o)

$(target): $(OBJS)
	g++ -std=c++11 -o $(target) $(OBJS) 
%.o:%.cpp
	g++ -std=c++11 -c -o $@ $< 

clean:
	rm *.o
	rm main
