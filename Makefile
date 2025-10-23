#define source file location
SRC_DIRS = ./source


#define location of the lib dir
LIB_DIRS = /usr/include/glad

INCLUDE_DIR = ./source/includes


#Recursively find all source file and lib files
CPP_SRCS :=  $(shell find $(SRC_DIRS) -type f -name "*.cpp")
LIB_SRCS :=   $(shell find $(LIB_DIRS) -type f -name "*.c")


INCLUDE_CPP :=  $(shell find $(INCLUDE_DIR) -type f -name "*.cpp")
INCLUDE_C :=  $(shell find $(INCLUDE_DIR) -type f -name "*.c")


SRCS	:= $(CPP_SRCS) ./source/libportaudio.a /usr/local/lib/libfftw3.a  $(LIB_SRCS) $(INCLUDE_C) $(INCLUDE_CPP)

FLAGS	=  -lGL
FLAGS	+= -lglfw
FLAGS	+= -I./source/includes
FLAGS   += -lrt 
FLAGS   += -lm 
FLAGS   += -lasound 
FLAGS   += -pthread

APP_NAME	= OPEN_GL_APP
OUT_DIR		= ./build/
OUT		:= $(OUT_DIR)$(APP_NAME)

all:
	$(CXX) $(SRCS) -o $(OUT) $(OPEN_GL_APP) $(FLAGS)
