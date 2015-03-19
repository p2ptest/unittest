#gteset的主目录
GTEST_DIR = /usr/local/gtest-1.7.0

#被测试程序的主目录，例如，/opt/p2p_server/branches/matrix/server/vod/mxfileproxy
TESTED_DIR = ..

#被测试类的源码名称
TESTED_NAME = sessionmgr

#gtest和被测试程序的头文件目录
INCLUDE = -I$(GTEST_DIR)/include -I$(TESTED_DIR)/include

#编译选项，-fprofile-arcs -ftest-coverage为gcov需要
CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage

#libgtest.a的所在目录
LIB = -L$(GTEST_DIR)/make

#生成测试报告目录
HTML_DIR = /opt/data/gtest_report


BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET = gtest

all : $(TARGET)

$(TARGET) : $(BINARY) $(TESTED_DIR)/obj/$(TESTED_NAME).o
	lcov -d ./ -z
	cd $(TESTED_DIR)/obj;lcov -d ./ -z;cd -
	rm -rf report.info $(HTML_DIR)/*
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ -lgtest $(LIB)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) $^ -o $@

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
clean :
	rm -rf $(TARGET) *.o *.gcno *.gcda *.gcov report.info $(TESTED_DIR)/obj/*.gcda $(HTML_DIR)/* $(TESTED_DIR)/obj/*.cpp

.PHONY : clean

#
#gcov :
#	gcov foo.c
#
#allgcov : 
#	gcov foo.c gtest.c
