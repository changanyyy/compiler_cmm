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
	gcc main.c objcode/obj.c intercode/ir.c semantic/semantic.c symtab/symtab.c util/tree_operation.c util/rbtree.c syntax.tab.c -lfl -ly -o parser

syntax: lexical syntax-c
	$(CC) -c $(YFC) -o $(YFO)

lexical: $(LFILE)
	$(FLEX) -o $(LFC) $(LFILE)

syntax-c: $(YFILE)
	$(BISON) -o $(YFC) -d -v $(YFILE)

-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: clean test t
test:
	make
	./parser Test/test1.cmm Test/test1.s
	./parser Test/test2.cmm Test/test2.s
	./parser Test/A-0.cmm Test/A-0.s
	./parser Test/A-1.cmm Test/A-1.s
	./parser Test/A-2.cmm Test/A-2.s
	./parser Test/A-3.cmm Test/A-3.s
	./parser Test/A-4.cmm Test/A-4.s
	./parser Test/A-5.cmm Test/A-5.s




t:
	make
	./parser Test/test.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
