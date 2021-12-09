#ifndef _OBJ_H_
#define _OBJ_H_

#include"../intercode/ir.h"
#include<time.h>



struct ObjCode{
    enum
    {
        LABELO, LIO, MOVEO, 
        ADDIO, ADDO, SUBO, MULO, DIVO, MFLO,
        LWO, SWO, JO, JALO, JRO, 
        BEQO, BNEO, BGTO, BLTO, BGEO, BLEO,
        FUNCO
    } kind;
    union
    {
        struct {int x;} label;
        struct {int reg1, imm;} li;
        struct {int reg1, reg2;} move;
        struct {int reg1, reg2, imm;} addi;
        struct {int reg1, reg2, reg3;} binaryop;//add, sub, mul ,div
        struct {int reg1, reg2;} div;
        struct {int reg1;} mflo;
        struct {int reg1, reg2, imm;} ls;
        struct {int x;} j;
        struct {STE *f; } jal;
        struct {} jr;
        struct {int reg1, reg2, x;} b;
        struct {STE *f; }func;
    };

    struct ObjCode *pre;
    struct ObjCode *nxt;
} OCHead;


struct RegStruct{
    bool occupied;
    int no;
    Operand op;//如果被占用了，存储的是什么变量
    struct RegStruct *nxt;//空闲链表里面的下一个
} regs[32];

struct RegStruct *free_reg_list;


void genObjCode();

void parseInterCode(struct InterCode *ir);

void newObjCode(int kind, ...);

int getReg(Operand op);

bool freeReg(int i);


bool allocateAddr(Operand op);
int allocateReg(Operand op);

void printObjCode();
void printObjCodes();



#define AVAIL_REG_MIN_IDX 8
#define AVAIL_REG_MAX_IDX 15

#define REG_V0 2
#define REG_V1 3

#define REG_A0 4
#define REG_A1 5
#define REG_A2 6
#define REG_A3 7

#define REG_SP 29
#define REG_FP 30
#define REG_RA 31

#define ARG_OFFSET 256

#endif