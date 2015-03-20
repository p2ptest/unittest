#包括底层库和服务器的头文件和库文件
include Makefile.comm

#gteset的主目录
GTEST_DIR = /usr/local/gtest-1.7.0

#被测试程序的主目录，例如，/opt/p2p_server/branches/matrix/server/vod/mxfileproxy
TESTED_DIR = ..

#被测试类的源码名称
TESTED_NAME = sessionmgr

#gtest和被测试程序的头文件目录
INCLUDE += -I$(GTEST_DIR)/include -I$(TESTED_DIR)/include

#编译选项，-fprofile-arcs -ftest-coverage为gcov需要，以便统计代码覆盖率
CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage

#libgtest.a的所在目录
LIBS += -lgtest -L$(GTEST_DIR)/make

#生成测试报告目录
HTML_DIR = /opt/data/gtest_report

#当前目录所有的目标文件
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#生成的目标文件
TARGET = gtest

all : $(TARGET)

#生成目标文件之后，将gcda文件和测试覆盖率文件删除
$(TARGET) : $(BINARY) $(TESTED_DIR)/obj/$(TESTED_NAME).o
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	lcov -d ./ -z
	cd $(TESTED_DIR)/obj;lcov -d ./ -z;cd -
	rm -rf report.info $(HTML_DIR)/*

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $^ -o $@

#生成代码覆盖率文件，先将源码文件拷贝到obj目录中
report :
	-cp -p $(TESTED_DIR)/src/*/*.cpp $(TESTED_DIR)/obj
	lcov -d ../ -c -o report.info
	genhtml report.info -o $(HTML_DIR)


#delete all gcda files to reset all execution counts to zero
#delete outputfile.info and report
#reset : 
#	lcov -d ./ -z
#	cd $(TESTED_DIR)/obj;lcov -d ./ -z;cd -
#	rm -rf report.info $(HTML_DIR)/*

#删除所有生成的文件
clean :
	rm -rf $(TARGET) *.o *.gcno *.gcda *.gcov report.info $(TESTED_DIR)/obj/*.gcda $(HTML_DIR)/* $(TESTED_DIR)/obj/*.cpp

#忽略文件名为clean的文件
.PHONY : clean

#
#gcov :
#	gcov foo.c
#
#allgcov : 
#	gcov foo.c gtest.c
