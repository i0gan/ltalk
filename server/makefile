#采用的编译器
CC = g++ 
#导出的项目名称
TARGET = IM-Server   
#使用O2优化, c++11标准
CFLAGS = -O2 -std=c++11   
#使用mysql库,和多线程库所需要的依赖
LDFLAGS = -lmysqlclient -lpthread   
#清除
RM = rm -f 

#获取src下所有的.cpp文件
SRC = $(wildcard ./src/*.cpp)  
#将所有的src对应的文件命名为.o文件
OBJ = $(patsubst %.cpp, %.o, $(SRC)) 
#链接所有.o文件
$(TARGET):$(OBJ) 
	$(CC) $^ -o $@ $(LDFLAGS) $(CFLAGS) 
#编译所有.cpp文件为.o文件
$(OBJ):%.o:%.cpp
	$(CC) -c $^ -o $@ $(LDFLAGS) $(CFLAGS)

#测试 SRC 与 OBJ的输出
print:
	@echo $(SRC)	
	@echo $(OBJ)	
#清除
clean:
	$(RM) $(TARGET) $(OBJ)

