#包括底层库和服务器的头文件和库文件
include Makefile.comm

#gmock的主目录
GMOCK_DIR = /usr/local/gmock-1.7.0

#被测试程序的主目录，例如，/opt/p2p_server/branches/matrix/server/vod/mxfileproxy
TESTED_DIR = ..

#生成测试报告目录
HTML_DIR = /opt/data/gtest_report

#########################################################
#							#
#		一般只需要更改上面的变量		#
#							#
#########################################################

#gteset的主目录
GTEST_DIR = $(GMOCK_DIR)/gtest

#当前目录、gtest、gmock和被测试程序的头文件目录
INCLUDE += -I. -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include -I$(TESTED_DIR)/include

#编译选项，-fprofile-arcs -ftest-coverage为gcov需要，以便统计代码覆盖率
CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage

#libgmock.a的所在目录,该静态库包含gtest-all.o
LIBS += -lgmock -L$(GMOCK_DIR)/make

#当前目录所有的目标文件
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#被测试类的OBJ对象，过滤掉main.o
TESTEDOBJS = $(filter-out $(TESTED_DIR)/obj/main.o,$(wildcard $(TESTED_DIR)/obj/*.o)) 
#TESTEDOBJS = $(TESTED_DIR)/obj/sessionmgr.o 

#生成的目标文件
TARGET = gtest

all : $(TARGET)

#生成目标文件之后，将gcda文件和测试覆盖率文件删除
#lcov -d ./ -z 将当前目录下的gcda覆盖率文件清空
$(TARGET) : $(BINARY) $(TESTEDOBJS)
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
