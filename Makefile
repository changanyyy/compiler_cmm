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
	./parser ./Test/test4.cmm > ../irsim/test4.ir
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
	./parser ./Test/E2-2.cmm  > ../irsim/E2-2.ir
	./parser ./Test/E2-3.cmm  > ../irsim/E2-3.ir
	./parser ./Test_Advance/A-0.cmm > ../irsim/Adv_A-0.ir
	./parser ./Test_Advance/A-1.cmm > ../irsim/Adv_A-1.ir
	./parser ./Test_Advance/A-2.cmm > ../irsim/Adv_A-2.ir
	./parser ./Test_Advance/A-3.cmm > ../irsim/Adv_A-3.ir
	./parser ./Test_Advance/A-4.cmm > ../irsim/Adv_A-4.ir
	./parser ./Test_Advance/A-5.cmm > ../irsim/Adv_A-5.ir
	./parser ./Test_Advance/A-6.cmm > ../irsim/Adv_A-6.ir
	./parser ./Test_Advance/A-7.cmm > ../irsim/Adv_A-7.ir
	./parser ./Test_Advance/A-8.cmm > ../irsim/Adv_A-8.ir
	./parser ./Test_Advance/A-9.cmm > ../irsim/Adv_A-9.ir
	./parser ./Test_Advance/A-10.cmm > ../irsim/Adv_A-10.ir
	./parser ./Test_Advance/A-11.cmm > ../irsim/Adv_A-11.ir
	./parser ./Test_Advance/A-12.cmm > ../irsim/Adv_A-12.ir
	./parser ./Test_Advance/A-13.cmm > ../irsim/Adv_A-13.ir
	./parser ./Test_Advance/A-14.cmm > ../irsim/Adv_A-14.ir
	./parser ./Test_Advance/A-15.cmm > ../irsim/Adv_A-15.ir
	./parser ./Test_Advance/A-16.cmm > ../irsim/Adv_A-16.ir
	./parser ./Test_Advance/A-17.cmm > ../irsim/Adv_A-17.ir
	./parser ./Test_Advance/A-18.cmm > ../irsim/Adv_A-18.ir
	./parser ./Test_Advance/A-19.cmm > ../irsim/Adv_A-19.ir
	./parser ./Test_Advance/A-20.cmm > ../irsim/Adv_A-20.ir
	./parser ./Test_Advance/A-21.cmm > ../irsim/Adv_A-21.ir
	./parser ./Test_Advance/A-22.cmm > ../irsim/Adv_A-22.ir
	./parser ./Test_Advance/A-23.cmm > ../irsim/Adv_A-23.ir
	./parser ./Test_Advance/A-24.cmm > ../irsim/Adv_A-24.ir
	./parser ./Test_Advance/A-25.cmm > ../irsim/Adv_A-25.ir
	./parser ./Test_Advance/A-26.cmm > ../irsim/Adv_A-26.ir
	./parser ./Test_Advance/A-27.cmm > ../irsim/Adv_A-27.ir
	./parser ./Test_Advance/A-28.cmm > ../irsim/Adv_A-28.ir
	./parser ./Test_Advance/A-29.cmm > ../irsim/Adv_A-29.ir
	./parser ./Test_Advance/A-30.cmm > ../irsim/Adv_A-30.ir
	./parser ./Test_Advance/A-31.cmm > ../irsim/Adv_A-31.ir
	./parser ./Test_Advance/A-32.cmm > ../irsim/Adv_A-32.ir
	./parser ./Test_Advance/A-33.cmm > ../irsim/Adv_A-33.ir
	./parser ./Test_Advance/A-34.cmm > ../irsim/Adv_A-34.ir
	./parser ./Test_Advance/A-35.cmm > ../irsim/Adv_A-35.ir
	./parser ./Test_Advance/A-36.cmm > ../irsim/Adv_A-36.ir
	./parser ./Test_Advance/A-37.cmm > ../irsim/Adv_A-37.ir
	./parser ./Test_Advance/A-38.cmm > ../irsim/Adv_A-38.ir
	./parser ./Test_Advance/A-39.cmm > ../irsim/Adv_A-39.ir
	./parser ./Test_Advance/A-40.cmm > ../irsim/Adv_A-40.ir
	./parser ./Test_Advance/A-41.cmm > ../irsim/Adv_A-41.ir
	./parser ./Test_Advance/A-42.cmm > ../irsim/Adv_A-42.ir
	./parser ./Test_Advance/A-43.cmm > ../irsim/Adv_A-43.ir
	./parser ./Test_Advance/A-44.cmm > ../irsim/Adv_A-44.ir
	./parser ./Test_Advance/A-45.cmm > ../irsim/Adv_A-45.ir
	./parser ./Test_Advance/A-46.cmm > ../irsim/Adv_A-46.ir
	./parser ./Test_Advance/A-47.cmm > ../irsim/Adv_A-47.ir
	./parser ./Test_Advance/A-48.cmm > ../irsim/Adv_A-48.ir
	./parser ./Test_Advance/A-49.cmm > ../irsim/Adv_A-49.ir
	./parser ./Test_Advance/A-50.cmm > ../irsim/Adv_A-50.ir
	./parser ./Test_Advance/A-51.cmm > ../irsim/Adv_A-51.ir
	./parser ./Test_Advance/A-52.cmm > ../irsim/Adv_A-52.ir
	./parser ./Test_Advance/A-53.cmm > ../irsim/Adv_A-53.ir
	./parser ./Test_Advance/A-54.cmm > ../irsim/Adv_A-54.ir
	./parser ./Test_Advance/A-55.cmm > ../irsim/Adv_A-55.ir
	./parser ./Test_Advance/A-56.cmm > ../irsim/Adv_A-56.ir
	./parser ./Test_Advance/A-57.cmm > ../irsim/Adv_A-57.ir
	./parser ./Test_Advance/A-58.cmm > ../irsim/Adv_A-58.ir
	./parser ./Test_Advance/A-59.cmm > ../irsim/Adv_A-59.ir
	./parser ./Test_Advance/A-60.cmm > ../irsim/Adv_A-60.ir
	./parser ./Test_Advance/A-60.cmm > ../irsim/Adv_A-60.ir
	./parser ./Test_Advance/A-61.cmm > ../irsim/Adv_A-61.ir
	./parser ./Test_Advance/A-62.cmm > ../irsim/Adv_A-62.ir
	./parser ./Test_Advance/A-63.cmm > ../irsim/Adv_A-63.ir
	./parser ./Test_Advance/A-64.cmm > ../irsim/Adv_A-64.ir
	./parser ./Test_Advance/A-65.cmm > ../irsim/Adv_A-65.ir
	./parser ./Test_Advance/A-66.cmm > ../irsim/Adv_A-66.ir
	./parser ./Test_Advance/A-67.cmm > ../irsim/Adv_A-67.ir
	./parser ./Test_Advance/A-68.cmm > ../irsim/Adv_A-68.ir
	./parser ./Test_Advance/A-69.cmm > ../irsim/Adv_A-69.ir
	./parser ./Test_Advance/A-70.cmm > ../irsim/Adv_A-70.ir
	./parser ./Test_Advance/A-71.cmm > ../irsim/Adv_A-71.ir
	./parser ./Test_Advance/A-72.cmm > ../irsim/Adv_A-72.ir
	./parser ./Test_Advance/A-73.cmm > ../irsim/Adv_A-73.ir
	./parser ./Test_Advance/A-74.cmm > ../irsim/Adv_A-74.ir
	./parser ./Test_Advance/A-75.cmm > ../irsim/Adv_A-75.ir
	./parser ./Test_Advance/A-76.cmm > ../irsim/Adv_A-76.ir
	./parser ./Test_Advance/A-77.cmm > ../irsim/Adv_A-77.ir
	./parser ./Test_Advance/A-78.cmm > ../irsim/Adv_A-78.ir
	./parser ./Test_Advance/A-79.cmm > ../irsim/Adv_A-79.ir
	./parser ./Test_Advance/A-80.cmm > ../irsim/Adv_A-80.ir
	./parser ./Test_Advance/A-81.cmm > ../irsim/Adv_A-81.ir
	./parser ./Test_Advance/A-82.cmm > ../irsim/Adv_A-82.ir
	./parser ./Test_Advance/A-83.cmm > ../irsim/Adv_A-83.ir
	./parser ./Test_Advance/A-84.cmm > ../irsim/Adv_A-84.ir
	./parser ./Test_Advance/A-85.cmm > ../irsim/Adv_A-85.ir
	./parser ./Test_Advance/A-86.cmm > ../irsim/Adv_A-86.ir
	./parser ./Test_Advance/A-87.cmm > ../irsim/Adv_A-87.ir
	./parser ./Test_Advance/A-88.cmm > ../irsim/Adv_A-88.ir
	./parser ./Test_Advance/A-89.cmm > ../irsim/Adv_A-89.ir
	./parser ./Test_Advance/A-90.cmm > ../irsim/Adv_A-90.ir
	./parser ./Test_Advance/A-91.cmm > ../irsim/Adv_A-91.ir
	./parser ./Test_Advance/A-92.cmm > ../irsim/Adv_A-92.ir
	./parser ./Test_Advance/A-93.cmm > ../irsim/Adv_A-93.ir
	./parser ./Test_Advance/A-94.cmm > ../irsim/Adv_A-94.ir
	./parser ./Test_Advance/A-95.cmm > ../irsim/Adv_A-95.ir
	./parser ./Test_Advance/A-96.cmm > ../irsim/Adv_A-96.ir
	./parser ./Test_Advance/A-97.cmm > ../irsim/Adv_A-97.ir
	./parser ./Test_Advance/A-98.cmm > ../irsim/Adv_A-98.ir
	./parser ./Test_Advance/A-99.cmm > ../irsim/Adv_A-99.ir
	./parser ./Test_Advance/A-100.cmm > ../irsim/Adv_A-100.ir
	./parser ./Test_Advance/A-101.cmm > ../irsim/Adv_A-101.ir
	./parser ./Test_Advance/A-102.cmm > ../irsim/Adv_A-102.ir
	./parser ./Test_Advance/A-103.cmm > ../irsim/Adv_A-103.ir
	./parser ./Test_Advance/A-104.cmm > ../irsim/Adv_A-104.ir
	./parser ./Test_Advance/A-105.cmm > ../irsim/Adv_A-105.ir
	./parser ./Test_Advance/A-106.cmm > ../irsim/Adv_A-106.ir
	./parser ./Test_Advance/A-107.cmm > ../irsim/Adv_A-107.ir
	./parser ./Test_Advance/A-108.cmm > ../irsim/Adv_A-108.ir
	./parser ./Test_Advance/A-109.cmm > ../irsim/Adv_A-109.ir
	./parser ./Test_Advance/A-110.cmm > ../irsim/Adv_A-110.ir
	./parser ./Test_Advance/A-111.cmm > ../irsim/Adv_A-111.ir
	./parser ./Test_Advance/A-112.cmm > ../irsim/Adv_A-112.ir
	./parser ./Test_Advance/A-113.cmm > ../irsim/Adv_A-113.ir
	./parser ./Test_Advance/A-114.cmm > ../irsim/Adv_A-114.ir
	./parser ./Test_Advance/A-115.cmm > ../irsim/Adv_A-115.ir
	./parser ./Test_Advance/A-116.cmm > ../irsim/Adv_A-116.ir
	./parser ./Test_Advance/A-117.cmm > ../irsim/Adv_A-117.ir
	./parser ./Test_Advance/A-118.cmm > ../irsim/Adv_A-118.ir
	./parser ./Test_Advance/A-119.cmm > ../irsim/Adv_A-119.ir
	./parser ./Test_Advance/A-120.cmm > ../irsim/Adv_A-120.ir
	./parser ./Test_Advance/A-121.cmm > ../irsim/Adv_A-121.ir
	./parser ./Test_Advance/A-122.cmm > ../irsim/Adv_A-122.ir
	./parser ./Test_Advance/A-123.cmm > ../irsim/Adv_A-123.ir
	./parser ./Test_Advance/A-124.cmm > ../irsim/Adv_A-124.ir
	./parser ./Test_Advance/A-125.cmm > ../irsim/Adv_A-125.ir
	./parser ./Test_Advance/A-126.cmm > ../irsim/Adv_A-126.ir
	./parser ./Test_Advance/A-127.cmm > ../irsim/Adv_A-127.ir
	./parser ./Test_Advance/A-128.cmm > ../irsim/Adv_A-128.ir
	./parser ./Test_Advance/A-129.cmm > ../irsim/Adv_A-129.ir
	./parser ./Test_Advance/A-130.cmm > ../irsim/Adv_A-130.ir
	./parser ./Test_Advance/A-131.cmm > ../irsim/Adv_A-131.ir
	./parser ./Test_Advance/A-132.cmm > ../irsim/Adv_A-132.ir
	./parser ./Test_Advance/A-133.cmm > ../irsim/Adv_A-133.ir
	./parser ./Test_Advance/A-134.cmm > ../irsim/Adv_A-134.ir
	./parser ./Test_Advance/A-135.cmm > ../irsim/Adv_A-135.ir
	./parser ./Test_Advance/A-136.cmm > ../irsim/Adv_A-136.ir
	./parser ./Test_Advance/A-137.cmm > ../irsim/Adv_A-137.ir
	./parser ./Test_Advance/A-138.cmm > ../irsim/Adv_A-138.ir
	./parser ./Test_Advance/A-139.cmm > ../irsim/Adv_A-139.ir
	./parser ./Test_Advance/A-140.cmm > ../irsim/Adv_A-140.ir
	./parser ./Test_Advance/A-141.cmm > ../irsim/Adv_A-141.ir
	./parser ./Test_Advance/A-142.cmm > ../irsim/Adv_A-142.ir
	./parser ./Test_Advance/A-143.cmm > ../irsim/Adv_A-143.ir
	./parser ./Test_Advance/A-144.cmm > ../irsim/Adv_A-144.ir
	./parser ./Test_Advance/A-145.cmm > ../irsim/Adv_A-145.ir
	./parser ./Test_Advance/A-146.cmm > ../irsim/Adv_A-146.ir
	./parser ./Test_Advance/A-147.cmm > ../irsim/Adv_A-147.ir
	./parser ./Test_Advance/A-148.cmm > ../irsim/Adv_A-148.ir
	./parser ./Test_Advance/A-149.cmm > ../irsim/Adv_A-149.ir
	./parser ./Test_Advance/A-150.cmm > ../irsim/Adv_A-150.ir
	./parser ./Test_Advance/A-151.cmm > ../irsim/Adv_A-151.ir
	./parser ./Test_Advance/A-152.cmm > ../irsim/Adv_A-152.ir
	./parser ./Test_Advance/A-153.cmm > ../irsim/Adv_A-153.ir
	./parser ./Test_Advance/A-154.cmm > ../irsim/Adv_A-154.ir
	./parser ./Test_Advance/A-155.cmm > ../irsim/Adv_A-155.ir
	./parser ./Test_Advance/A-156.cmm > ../irsim/Adv_A-156.ir
	./parser ./Test_Advance/A-157.cmm > ../irsim/Adv_A-157.ir
	./parser ./Test_Advance/A-158.cmm > ../irsim/Adv_A-158.ir
	./parser ./Test_Advance/A-159.cmm > ../irsim/Adv_A-159.ir
	./parser ./Test_Advance/A-160.cmm > ../irsim/Adv_A-160.ir
	./parser ./Test_Advance/A-161.cmm > ../irsim/Adv_A-161.ir
	./parser ./Test_Advance/A-162.cmm > ../irsim/Adv_A-162.ir
	./parser ./Test_Advance/A-163.cmm > ../irsim/Adv_A-163.ir
	./parser ./Test_Advance/A-164.cmm > ../irsim/Adv_A-164.ir
	./parser ./Test_Advance/A-165.cmm > ../irsim/Adv_A-165.ir
	./parser ./Test_Advance/A-166.cmm > ../irsim/Adv_A-166.ir
	./parser ./Test_Advance/A-167.cmm > ../irsim/Adv_A-167.ir
	./parser ./Test_Advance/A-168.cmm > ../irsim/Adv_A-168.ir
	./parser ./Test_Advance/A-169.cmm > ../irsim/Adv_A-169.ir
	./parser ./Test_Advance/A-170.cmm > ../irsim/Adv_A-170.ir
	./parser ./Test_Advance/A-171.cmm > ../irsim/Adv_A-171.ir
	./parser ./Test_Advance/A-172.cmm > ../irsim/Adv_A-172.ir
	./parser ./Test_Advance/A-173.cmm > ../irsim/Adv_A-173.ir
	./parser ./Test_Advance/A-174.cmm > ../irsim/Adv_A-174.ir
	./parser ./Test_Advance/A-175.cmm > ../irsim/Adv_A-175.ir
	./parser ./Test_Advance/A-176.cmm > ../irsim/Adv_A-176.ir
	./parser ./Test_Advance/A-177.cmm > ../irsim/Adv_A-177.ir
	./parser ./Test_Advance/A-178.cmm > ../irsim/Adv_A-178.ir
	./parser ./Test_Advance/A-179.cmm > ../irsim/Adv_A-179.ir
	./parser ./Test_Advance/A-180.cmm > ../irsim/Adv_A-180.ir
	./parser ./Test_Advance/A-181.cmm > ../irsim/Adv_A-181.ir
	./parser ./Test_Advance/A-182.cmm > ../irsim/Adv_A-182.ir
	./parser ./Test_Advance/A-183.cmm > ../irsim/Adv_A-183.ir
	./parser ./Test_Advance/A-184.cmm > ../irsim/Adv_A-184.ir
	./parser ./Test_Advance/A-185.cmm > ../irsim/Adv_A-185.ir
	./parser ./Test_Advance/A-186.cmm > ../irsim/Adv_A-186.ir
	./parser ./Test_Advance/A-187.cmm > ../irsim/Adv_A-187.ir
	./parser ./Test_Advance/A-188.cmm > ../irsim/Adv_A-188.ir
	./parser ./Test_Advance/A-189.cmm > ../irsim/Adv_A-189.ir
	./parser ./Test_Advance/A-190.cmm > ../irsim/Adv_A-190.ir
	./parser ./Test_Advance/A-191.cmm > ../irsim/Adv_A-191.ir
	./parser ./Test_Advance/A-192.cmm > ../irsim/Adv_A-192.ir
	./parser ./Test_Advance/A-193.cmm > ../irsim/Adv_A-193.ir
	./parser ./Test_Advance/A-194.cmm > ../irsim/Adv_A-194.ir
	./parser ./Test_Advance/A-195.cmm > ../irsim/Adv_A-195.ir
	./parser ./Test_Advance/A-196.cmm > ../irsim/Adv_A-196.ir
	./parser ./Test_Advance/A-197.cmm > ../irsim/Adv_A-197.ir
	./parser ./Test_Advance/A-198.cmm > ../irsim/Adv_A-198.ir
	./parser ./Test_Advance/A-199.cmm > ../irsim/Adv_A-199.ir





t:
	make
	./parser ./Test_Advance/A-35.cmm

clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f $(OBJS) $(OBJS:.o=.d)
	rm -f $(LFC) $(YFC) $(YFC:.c=.h)
	rm -f *~
