#包括底层库和服务器的头文件和库文件
include Makefile.comm

#当前目录所有的目标文件
BINARY = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#被测试类的OBJ对象
TESTEDOBJS = $(addprefix $(CODE)/,$(OBJS)) 

#生成的目标文件
TARGET = gtest

#生成gtest文件后清除文件覆盖数据
#生成目标文件之后，将gcda文件和测试覆盖率文件删除
all : $(TARGET)
	make clear

$(TARGET) : $(TESTEDOBJS) $(BINARY)
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

#编译待测试类
$(TESTEDOBJS) : $(SOURCECODE)
	@cp -p $^ $(CODE)
	cd $(CODE);make;cd -

copy : $(SOURCECODE)
	@cp -p $^ $(CODE)

%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDE) $^ -o $@

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
