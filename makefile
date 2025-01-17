CXX = gcc 
CXXFLAGS = -Wall -std=c11 -fsanitize=address -I./include

ifeq ($(shell uname), Darwin)
    # macOS-specific frameworks
    CXXFRAMEWORK = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    LIB = -L./lib/ -lraylib_mac
else
    # Linux-specific libraries
    CXXFRAMEWORK =
    LIB = -L./lib/ -lraylib_lin -lGL -lGLU -lm -lX11
endif

SRCS = $(wildcard src/*.c) 
OBJS = $(SRCS:.c=.o)

TARGET = game 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFRAMEWORK) $(CXXFLAGS) $(OBJS) $(LIB) -o $(TARGET) 

# compile source files into object files
%.o: %.c
	$(CXX) $(CXXFRAMEWORK) $(CXXFLAGS)   -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all 

q: re
	./game
