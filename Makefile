# 编译器设定和编译选项
CC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

# 编译目标：src目录下的所有.c文件
CFILES = $(shell find ./ -name "*.c")
OBJS = $(CFILES:.c=.o)
LFILE = $(shell find ./ -name "*.l")
YFILE = $(shell find ./ -name "*.y")
LFC = $(shell find ./ -name "*.l" | sed s/[^/]*\\.l/lex.yy.c/)
YFC = $(shell find ./ -name "*.y" | sed s/[^/]*\\.y/syntax.tab.c/)
LFO = $(LFC:.c=.o)
YFO = $(YFC:.c=.o)

parser: syntax $(CFILES)
	gcc main.c  semantic/semantic.c symtab/symtab.c util/tree_operation.c util/rbtree.c syntax.tab.c -lfl -ly -o parser

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test
test:
	make
	./parser ./Test/test1.cmm
	./parser ./Test/test2.cmm
	./parser ./Test/test3.cmm
	./parser ./Test/test4.cmm
	./parser ./Test/test5.cmm
	./parser ./Test/test6.cmm
	./parser ./Test/test7.cmm
	./parser ./Test/test8.cmm
	./parser ./Test/test9.cmm
	./parser ./Test/test10.cmm
	./parser ./Test/test11.cmm
	./parser ./Test/test12.cmm
	./parser ./Test/test13.cmm
	./parser ./Test/test14.cmm
	./parser ./Test/test15.cmm
	./parser ./Test/test16.cmm
	./parser ./Test/test17.cmm
	
	#./parser ./Test/test-1.cmm
	#./parser ./Test/test-2.cmm
	#./parser ./Test/test-3.cmm
	#./parser ./Test/test-4.cmm
	#./parser ./Test/test-5.cmm
	#./parser ./Test/test-6.cmm



	./parser ./Tests_2_Normal/Tests/tests/A-1.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-2.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-3.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-4.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-5.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-6.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-7.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-8.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-9.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-10.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-11.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-12.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-13.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-14.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-15.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-16.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-17.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-18.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-19.cmm
	./parser ./Tests_2_Normal/Tests/tests/A-20.cmm
	./parser ./Tests_2_Normal/Tests/tests/B-1.cmm
	./parser ./Tests_2_Normal/Tests/tests/B-2.cmm
	./parser ./Tests_2_Normal/Tests/tests/C-1.cmm
	./parser ./Tests_2_Normal/Tests/tests/C-2.cmm
	./parser ./Tests_2_Normal/Tests/tests/D-2.cmm
	./parser ./Tests_2_Normal/Tests/tests/D-3.cmm
	./parser ./Tests_2_Normal/Tests/tests/E-3.cmm

	./parser ./Test/test.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
