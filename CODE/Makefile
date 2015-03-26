include ../Makefile.comm

all:$(OBJS)

%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS)

#将当前目录下的gcda覆盖率文件清空
clear:
	lcov -b ./ -d ./ -z

#删除除了Makefile之外的所有文件
clean:
	@rm -rf ${OBJS} *.gcno *.gcda *.cpp *.h

