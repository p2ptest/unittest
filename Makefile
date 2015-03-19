GTEST_DIR = /usr/local/gtest-1.7.0

TESTED_DIR = ..

TESTED_NAME = sessionmgr

INCLUDE = -I$(GTEST_DIR)/include -I$(TESTED_DIR)/include

CXXFLAGS += -g -Wall -Wextra -pthread -fprofile-arcs -ftest-coverage

LIB = -L$(GTEST_DIR)/make

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
