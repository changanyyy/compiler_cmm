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
	gcc main.c intercode/ir.c semantic/semantic.c symtab/symtab.c util/tree_operation.c util/rbtree.c syntax.tab.c -lfl -ly -o parser

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
	./parser ./Test/test1.cmm > ../irsim/test1.ir
	./parser ./Test/test2.cmm > ../irsim/test2.ir
	./parser ./Test/A-1.cmm   > ../irsim/A-1.ir
	./parser ./Test/A-2.cmm   > ../irsim/A-2.ir
	./parser ./Test/A-3.cmm   > ../irsim/A-3.ir
	./parser ./Test/A-4.cmm   > ../irsim/A-4.ir
	./parser ./Test/A-5.cmm   > ../irsim/A-5.ir
	./parser ./Test/B-1.cmm   > ../irsim/B-1.ir
	./parser ./Test/B-2.cmm   > ../irsim/B-2.ir
	./parser ./Test/B-3.cmm   > ../irsim/B-3.ir
	./parser ./Test/C-1.cmm   > ../irsim/C-1.ir
	./parser ./Test/C-2.cmm   > ../irsim/C-2.ir
	./parser ./Test/D-1.cmm   > ../irsim/D-1.ir
	#./parser ./Test/E1-1.cmm  > ../irsim/E1-1.ir
	#./parser ./Test/E1-2.cmm  > ../irsim/E1-2.ir
	#./parser ./Test/E1-3.cmm  > ../irsim/E1-3.ir
	./parser ./Test/E2-1.cmm  > ../irsim/E2-1.ir
	#./parser ./Test/E2-2.cmm  > ../irsim/E2-2.ir
	#./parser ./Test/E2-3.cmm  > ../irsim/E2-3.ir


t:
	make
	./parser ./Test/E2-1.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
