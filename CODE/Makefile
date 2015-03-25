include ../Makefile.comm

TESTED_DIR = ../..

#源文件，包括头文件和cpp文件
SOURCECODE = $(wildcard $(TESTED_DIR)/src/*/*.cpp $(TESTED_DIR)/src/*/*.h)

#notdir 去除路径
CODE = $(notdir $(SOURCECODE))

#目标文件，去除头文件和main.o文件
BINARY = $(filter-out %.h main.o,$(patsubst %.cpp,%.o,$(CODE)))

all:$(BINARY)

#将源文件复制到当前目录
$(CODE):$(SOURCECODE)
	@cp -p $^ .
	
%.o:%.cpp
	$(CXX) $< -o $@ -c $(INCLUDE) $(CFLAGS)

clean:
	@rm -rf ${BINARY} *.gcno *.gcda *.cpp *.h


#################################
#				#
#	缺点；会复制多次	#
#				#
#################################
