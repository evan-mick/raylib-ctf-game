CXX = gcc 
CXXFLAGS = -Wall -std=c11 -Iinclude -fsanitize=address
CXXFRAMEWORK = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL  

LIB = -L./lib/ -lraylib
SRCS = $(wildcard src/*.c) 
OBJS = $(SRCS:.c=.o)

TARGET = game 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFRAMEWORK) $(CXXFLAGS) $(LIB) $(OBJS) -o $(TARGET) 

# compile source files into object files
%.o: %.c
	$(CXX) $(CXXFRAMEWORK) $(CXXFLAGS)  -I./include -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all 

q: re
	./game
