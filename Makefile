#包括底层库和服务器的头文件和库文件
include Makefile.comm

TARGET = server_test unit_test
#生成可执行文件后清除文件覆盖数据
#生成目标文件之后，将gcda文件和测试覆盖率文件删除
all : copy $(TARGET)
#	make clear

copy : $(SOURCECODE)
	@mkdir -p $(CODE)
	@cp -rp $(SERVER_ROOT)/src/* $(CODE)
	@rm -rf $(CODE)/make* $(CODE)/*/make*

#链接生成可执行文件，此为程序可执行文件
server_test : $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

#单元测试可执行文件
unit_test : $(OBJS_WITHOUT_MAIN) $(OBJS_TEST)
	$(CXX) $(CFLAGS) $^ -o $@ $(LIBS)

#编译类
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(INCLUDE) $^ -o $@

#生成代码覆盖率文件
report :
	lcov -d ./ -c -o report.info
	genhtml report.info -o $(HTML_DIR)

#lcov -d ./ -z 将当前目录下的gcda覆盖率文件清空
clear:
	lcov -b ./ -d ./ -z
	rm -rf report.info $(HTML_DIR)/*
	
#删除所有生成的文件
clean :
	@rm -rf $(TARGET) $(CODE_TEST)/*.o $(CODE_TEST)/*.gcno $(CODE_TEST)/*.gcda $(CODE_TEST)/*.gcov $(CODE)
	@rm -rf report.info $(HTML_DIR)/*

#忽略文件名为clean的文件
.PHONY : clean

#检查内存泄漏
#valgrind -v --tool=memcheck --leak-check=full ./server_test -l >> valReport 2>&1
