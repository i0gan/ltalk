# This is a makefile for complie server
# Create time: 2020-04-20
# Author: i0gan

CC = g++ 
TARGET = ltalks
CFLAGS = -O2 -std=c++11   
LDFLAGS = -lmysqlclient -lpthread   
RM = rm -f 
CP = cp -r
MKDIR = mkdir

BUILD_PATH = ./build

# install path
INSTALL_PATH = /usr/share/ltalks

# src path
CORE_PATH   = ./src/core
DB_PATH     = ./src/db
CRYPTO_PATH = ./src/crypto
XML_PATH    = ./src/xml
MAIN_PATH   = ./src


OBJS =

# main
MAIN_SRC = $(wildcard $(MAIN_PATH)/*.cpp)  
MAIN_OBJ = $(patsubst %.cpp, %.o, $(MAIN_SRC)) 
OBJS += $(MAIN_OBJ)

# core src
CORE_SRC = $(wildcard $(CORE_PATH)/*.cpp)  
CORE_OBJ = $(patsubst %.cpp, %.o, $(CORE_SRC)) 
OBJS += $(CORE_OBJ)


# complie
$(TARGET):$(OBJS)
	$(CC) $^ -o $(BUILD_PATH)/$@ $(LDFLAGS) $(CFLAGS) 


$(OBJS):%.o:%.cpp
	$(CC) -c $^ -o $@ $(LDFLAGS) $(CFLAGS)


print:
	@echo $(OBJS)	


clean:
	$(RM) $(BUILD_PATH)/$(TARGET) $(OBJS)

install:
	@sudo $(MKDIR) $(INSTALL_PATH)
	@sudo $(CP) $(BUILD_PATH)/* $(INSTALL_PATH)
	@ln -s $(INSTALL_PATH)/$(TARGET) /usr/bin/$(TARGET)
	@echo 'install complete'

