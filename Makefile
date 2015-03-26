#包括底层库和服务器的头文件和库文件
include Makefile.comm

#被测试程序的代码转移目录
CODE = CODE

#gteset的主目录
GTEST_DIR = $(GMOCK_DIR)/gtest

#gtest和gmock头文件目录,默认包括当前目录
INCLUDE += -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include -I./$(CODE)

#编译选项，-fprofile-arcs -ftest-coverage为gcov需要，以便统计代码覆盖率
CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage

#libgmock.a的所在目录,该静态库包含gtest-all.o
LIBS += -lgmock -L$(GMOCK_DIR)/make

#当前目录所有的目标文件
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#被测试类的OBJ对象
TESTEDOBJS = $(addprefix $(CODE)/,$(OBJS)) 

#生成的目标文件
TARGET = gtest

#生成gtest文件后清除文件覆盖数据
all : $(TARGET)
	make clear

#生成目标文件之后，将gcda文件和测试覆盖率文件删除
$(TARGET) : $(TESTEDOBJS) $(BINARY)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)

#编译待测试类
$(TESTEDOBJS) : $(SOURCECODE)
	@cp -p $^ $(CODE)
	cd $(CODE);make;cd -

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $^ -o $@

#生成代码覆盖率文件
report :
	lcov -d ./ -c -o report.info
	genhtml report.info -o $(HTML_DIR)

#lcov -d ./ -z 将当前目录下的gcda覆盖率文件清空
clear:
	lcov -b ./ -d ./ -z
	cd $(CODE);make clear;cd -
	rm -rf report.info $(HTML_DIR)/*
	
#删除所有生成的文件
clean :
	@rm -rf $(TARGET) *.o *.gcno *.gcda *.gcov report.info $(HTML_DIR)/*
	cd $(CODE);make clean;cd -

#忽略文件名为clean的文件
.PHONY : clean

#################################################
#						#
#缺点:可能会多次进入$(CODE)目录编译		#
#						#
#原因：	$(TESTEDOBJS) : $(SOURCECODE)		#
#	一旦$(SOURCECODE)中有一个文件更新，	#
#	$(TESTEDOBJS)中的所有文件都会被认	#
#	为比$(TESTEDOBJS)旧，从而这个规则	#
#	会触发多次				#
#						#
#################################################
