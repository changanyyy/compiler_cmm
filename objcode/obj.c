#include"obj.h"

//生成目标代码，总函数
void genObjCode(){
    //再生成目标代码之前，先初始化一些数据结构

    //空闲寄存器链表（只用t0-t7）8-15号
    free_reg_list = NULL;

    //初始化目标代码双向循环链表
    OCHead.pre = OCHead.nxt = &OCHead;

    //寄存器结构 32个
    for(int i = 0; i < 32; i++){
        regs[i].no = i;
        regs[i].occupied = false;
        regs[i].op = NULL;
        regs[i].nxt = NULL;
    }

    //把t0-t7寄存器放入空闲链表
    for(int i = AVAIL_REG_MIN_IDX; i <= AVAIL_REG_MAX_IDX; i++){
        regs[i].nxt = free_reg_list;
        free_reg_list = &regs[i];
    }

    //逐行中间代码解析
    struct InterCode *ic = IRhead.nxt;
    while(ic != &IRhead){
        parseInterCode(ic);
        ic = ic->nxt;
    }

}

//变量和临时变量相对于offset的偏移，用于在栈中分配地址
int fp_offset = -8;
//实参的编号，从而存到相应寄存器和栈空间
int arg_idx = 0;
//形参编号，把相应实参对应过来（每个形参要分配对应实参的地址）
int para_idx = 0;

//解析每一条中间代码
void parseInterCode(struct InterCode *ir){
    int reg1, reg2, reg3, imm;
    switch(ir->kind){
    case ASSIGNIR:
        reg1 = getReg(ir->u.assign.lhs);
        if(ir->u.assign.rhs->kind == CONSTANT){
            imm = ir->u.assign.rhs->u.int_value;
            newObjCode(LIO, reg1, imm);  
        } else{
            reg2 = getReg(ir->u.assign.rhs);
            newObjCode(MOVEO, reg1, reg2);
        }
        break;
    case ADDIR:
    //res
        reg1 = getReg(ir->u.binop.res);
    //op1    
        if(ir->u.binop.op1->kind == CONSTANT){
            //如果第一个操作数就是constant，先存在v0寄存器里
            newObjCode(LIO, REG_V0, ir->u.binop.op1->u.int_value);
            reg2 = REG_V0;
        }else{
            reg2 = getReg(ir->u.binop.op1);
        }
    //op2    
        if(ir->u.binop.op2->kind == CONSTANT){
            imm = ir->u.binop.op2->u.int_value;
            newObjCode(ADDIO, reg1, reg2, imm);  
        } else{
            reg3 = getReg(ir->u.binop.op2);
            newObjCode(ADDO, reg1, reg2, reg3);
        }
        break;
    case SUBIR:
    //res
        reg1 = getReg(ir->u.binop.res);
    //op1    
        if(ir->u.binop.op1->kind == CONSTANT){
            //如果第一个操作数就是constant，先存在v0寄存器里
            newObjCode(LIO, REG_V0, ir->u.binop.op1->u.int_value);
            reg2 = REG_V0;
        }else{
            reg2 = getReg(ir->u.binop.op1);
        }
    //op2
        if(ir->u.binop.op2->kind == CONSTANT){
            imm = ir->u.binop.op2->u.int_value;
            newObjCode(ADDIO, reg1, reg2, -imm);  
        } else{
            reg3 = getReg(ir->u.binop.op2);
            newObjCode(SUBO, reg1, reg2, reg3);
        }
        break;
    case MULIR:
    //res
        reg1 = getReg(ir->u.binop.res);
    //op1    
        if(ir->u.binop.op1->kind == CONSTANT){
            //如果第一个操作数就是constant，先存在v0寄存器里
            newObjCode(LIO, REG_V0, ir->u.binop.op1->u.int_value);
            reg2 = REG_V0;
        }else{
            reg2 = getReg(ir->u.binop.op1);
        }
    //op2
        if(ir->u.binop.op2->kind == CONSTANT){
            //如果第二个操作数就是constant，先存在v1寄存器里
            newObjCode(LIO, REG_V1, ir->u.binop.op2->u.int_value);
            reg3 = REG_V1;
        }else{
            reg3 = getReg(ir->u.binop.op2);
        }
        newObjCode(MULO, reg1, reg2, reg3);
        break;
    case DIVIR://加减乘除 
    //res
        reg1 = getReg(ir->u.binop.res);
    //op1
        if(ir->u.binop.op1->kind == CONSTANT){
            //如果第一个操作数就是constant，先存在v0寄存器里
            newObjCode(LIO, REG_V0, ir->u.binop.op1->u.int_value);
            reg2 = REG_V0;
        }else{
            reg2 = getReg(ir->u.binop.op1);
        }
    //op2
        if(ir->u.binop.op2->kind == CONSTANT){
            //如果第二个操作数就是constant，先存在v1寄存器里
            newObjCode(LIO, REG_V1, ir->u.binop.op2->u.int_value);
            reg3 = REG_V1;
        }else{
            reg3 = getReg(ir->u.binop.op2);
        }
        newObjCode(DIVO, reg2, reg3);
        newObjCode(MFLO, reg1);
        break;
    case LABELIR: 
        newObjCode(LABELO, ir->u.label->u.labelnum);
        break;
    case GOTOIR: 
        newObjCode(JO, ir->u.gotolabel->u.labelnum);
        break;
    case CONDJMPIR://标号，无条件跳转，有条件跳转
        reg1 = getReg(ir->u.cjmp.lhs);
        reg2 = getReg(ir->u.cjmp.rhs);
        if(ir->u.cjmp.relop == G)newObjCode(BGTO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        else if(ir->u.cjmp.relop ==GE)newObjCode(BGEO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        else if(ir->u.cjmp.relop ==EQ)newObjCode(BEQO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        else if(ir->u.cjmp.relop ==L)newObjCode(BLTO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        else if(ir->u.cjmp.relop ==LE)newObjCode(BLEO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        else if(ir->u.cjmp.relop ==NE)newObjCode(BNEO, reg1, reg2, ir->u.cjmp.label->u.labelnum);
        break;
    case READIR: 
    //TODO
        break;
    case WRITEIR: //read和write函数
    //TODO
        break;
    case CALLIR: 
        //先把sp的值确定，在现有sp基础上减去256(给出256的空间给参数)
        newObjCode(ADDIO, REG_SP, REG_FP, fp_offset - ARG_OFFSET);
        newObjCode(JALO, ir->u.call.fun->name);
        arg_idx = 0;
        break;
    case ARGIR: 
        reg1 = getReg(ir->u.arg);
        if(arg_idx >= 4){
            //在fp_idx下面256
            newObjCode(SWO, reg1, REG_FP, fp_offset - ARG_OFFSET + (arg_idx-4) * 4);
        }
        else{
            //小于4个的参数存进a0-a3
            newObjCode(MOVEO, REG_A0 + arg_idx, reg1);
        }
        arg_idx++;
        break;
    case RETURNIR://函数调用和返回
        para_idx = 0;
        reg1 = REG_V0;//v0
        reg2 = getReg(ir->u.ret);
        newObjCode(MOVEO, reg1, reg2);
        newObjCode(JRO);
        break;
    case DECIR:
        fp_offset -= ir->u.dec.size;
        ir->u.dec.name->fp_offset = fp_offset;
        ir->u.dec.name->have_addr = true;
        break;
    case FUNIR:
        newObjCode(FUNCO, ir->u.fun);
        //先把fp的offset重置
        fp_offset = -8;
        //保存旧的fp值到新fp-8处（sp-8）
        newObjCode(SWO, REG_FP, REG_SP, -8);
        //把新fp的值放入fp
        newObjCode(MOVEO, REG_FP, REG_SP);
        //把ra存在栈中
        newObjCode(SWO, REG_RA, REG_FP, -4);

        break;
    case PARAIR://变量空间申请，函数名，函数参数
        //当参数已经小于4号，已经在寄存器里面了。
        if(para_idx >=4){
            ir->u.para->have_addr = true;
            ir->u.para->fp_offset = (para_idx-4) * 4;
        }
        para_idx++;

        break;
    case ASSIGNADDRIR:
        //TODO 
        reg1 = getReg(ir->u.aa.res);
        newObjCode(ADDIO, reg1, REG_FP, ir->u.aa.var->u.addr->fp_offset);
        break;
    case ASSIGNSTARIR:
        reg1 = getReg(ir->u.as.res);
        reg2 = getReg(ir->u.as.var);
        newObjCode(LWO, reg1, reg2, 0);
        break;
    case STARASSIGNIR:
        reg1 = getReg(ir->u.sa.var);
        reg2 = getReg(ir->u.sa.res);
        newObjCode(SWO, reg1, reg2, 0);
        break;
    default: break;
    }
}


void newObjCode(int kind, ...){
    struct ObjCode *oc = malloc(sizeof(struct ObjCode));
    oc->kind = kind;
    
    //变长参数
    va_list argptr;
    va_start(argptr, kind);

    switch(kind){
    case LABELO:
        oc->label.x = va_arg(argptr, int);
        break;
    case LIO:
        oc->li.reg1 = va_arg(argptr, int);
        oc->li.imm = va_arg(argptr, int);
        break;
    case MOVEO:
        oc->move.reg1 = va_arg(argptr, int);
        oc->move.reg2 = va_arg(argptr, int);
        break; 
    case ADDIO:
        oc->addi.reg1 = va_arg(argptr, int);
        oc->addi.reg2 = va_arg(argptr, int);
        oc->addi.imm = va_arg(argptr, int);
        break;
    case ADDO:
    case SUBO:
    case MULO:
        oc->binaryop.reg1 = va_arg(argptr, int);
        oc->binaryop.reg2 = va_arg(argptr, int);
        oc->binaryop.reg3 = va_arg(argptr, int);
        break;
    case DIVO:
        oc->div.reg1 = va_arg(argptr, int);
        oc->div.reg2 = va_arg(argptr, int);
        break; 
    case MFLO:
        oc->mflo.reg1 = va_arg(argptr, int);
        break;
    case LWO:
    case SWO: 
        oc->ls.reg1 = va_arg(argptr, int);
        oc->ls.reg2 = va_arg(argptr, int);
        oc->ls.imm = va_arg(argptr, int);
        break;
    case JO:
        oc->j.x = va_arg(argptr, int);
        break;
    case JALO:
        oc->jal.f = va_arg(argptr, STE *);
        break;
    case JRO:
        break; 
    case BEQO:
    case BNEO:
    case BGTO:
    case BLTO:
    case BGEO:
    case BLEO:
        oc->b.reg1 = va_arg(argptr, int);
        oc->b.reg2 = va_arg(argptr, int);
        oc->b.x = va_arg(argptr, int);
        break;
    case FUNCO:
        oc->func.f = va_arg(argptr, STE *);
    }

    OCHead.pre->nxt = oc;
    oc->pre = OCHead.pre;
    oc->nxt = &OCHead;
    OCHead.pre = oc;

}

//获得某变量对应的寄存器
int getReg(Operand op){
    if(op->kind == VARIABLE){//如果是变量看看有没有寄存器
        if(op->u.ste->have_addr){
            op->have_addr = true;
            op->fp_offset = op->u.ste->fp_offset;
        }
    }

    int res = -1;
    if(op->regidx >= AVAIL_REG_MIN_IDX && op->regidx <= AVAIL_REG_MAX_IDX){
        //如果已经有对应寄存器，就返回
        res = op->regidx;
    }
    else{
        //对于临时变量，先分配地址再分配寄存器（如果已经有地址了就直接分配寄存器）
        allocateAddr(op);
        res = allocateReg(op);
    }
    return res;
}

bool freeReg(int i){
    if(regs[i].occupied){
        regs[i].occupied = false;
        regs[i].op->regidx = -1;
        regs[i].op = NULL;
        return true;
    }
    else return false;
}

bool allocateAddr(Operand op){
    if(op->have_addr == false){
        fp_offset-=4;
        op->fp_offset = fp_offset;
        op->have_addr = true;
        return true;
    }
    return false;
}


//分配一个寄存器，如果没有空闲的，就随机选一个，然后把原来的踢出去
int allocateReg(Operand op){
    int regidx;
    srand((unsigned)time(NULL));
    if(free_reg_list != NULL){//把空闲链表的第一个分配出去
        regidx = free_reg_list->no;//空闲链表截掉这个reg
        free_reg_list = free_reg_list->nxt;
        newObjCode(LWO, regidx, 30, op->fp_offset);
        
        //把reg分配给op
        regs[regidx].occupied = true;
        regs[regidx].op = op;
        op->regidx = regidx;
    }
    else{
        //随机选择一个寄存器
        regidx = rand() % (AVAIL_REG_MAX_IDX - AVAIL_REG_MIN_IDX + 1) + AVAIL_REG_MIN_IDX;
        //spilling溢出到内存,相对于fp(30号)偏移offset
        newObjCode(SWO, regidx, 30, regs[regidx].op->fp_offset);
        newObjCode(LWO, regidx, 30, op->fp_offset);

        //原来的op溢出，op->regidx = -1
        //新的op放进去
        regs[regidx].op->regidx = -1;
        regs[regidx].op = op;
        op->regidx = regidx;
    }
    return regidx;
}


void printObjCode(struct ObjCode *oc){
    switch(oc->kind){
    case LABELO: printf("label%d:", oc->label.x); break;
    case LIO:    printf("li $%d, %d", oc->li.reg1, oc->li.imm); break;
    case MOVEO:  printf("move $%d, $%d", oc->move.reg1, oc->move.reg2); break;
    case ADDIO:  printf("addi $%d, $%d, %d", oc->addi.reg1, oc->addi.reg2, oc->addi.imm); break; 
    case ADDO:   printf("add $%d, $%d, $%d", oc->binaryop.reg1, oc->binaryop.reg2, oc->binaryop.reg3); break;
    case SUBO:   printf("sub $%d, $%d, $%d", oc->binaryop.reg1, oc->binaryop.reg2, oc->binaryop.reg3); break; 
    case MULO:   printf("mul $%d, $%d, $%d", oc->binaryop.reg1, oc->binaryop.reg2, oc->binaryop.reg3); break;
    case DIVO:   printf("div $%d, $%d", oc->div.reg1, oc->div.reg2); break;
    case MFLO:   printf("mflo $%d", oc->mflo.reg1); break;
    case LWO:    printf("lw $%d, %d($%d)", oc->ls.reg1, oc->ls.imm, oc->ls.reg2); break;
    case SWO:    printf("sw $%d, %d($%d)", oc->ls.reg1, oc->ls.imm, oc->ls.reg2); break;
    case JO:     printf("j label%d", oc->j.x); break;
    case JALO:   printf("jal %s", oc->jal.f->name); break;
    case JRO:    printf("jr $ra"); break;
    case BEQO:   printf("beq $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case BNEO:   printf("bne $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case BGTO:   printf("bgt $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case BLTO:   printf("blt $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case BGEO:   printf("bge $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case BLEO:   printf("ble $%d, $%d, label%d", oc->b.reg1, oc->b.reg2, oc->b.x); break;
    case FUNCO:  printf("\n%s:", oc->func.f->name);
    default: break;
    }
    printf("\n");
    return;
}


void printObjCodes(){
    //输出开头部分
    printf(".data\n_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n.globl main\n.text\n");
    printf("read:\nli $v0, 4\nla $a0, _prompt\nsyscall\nli $v0, 5\nsyscall\njr $ra\n");
    printf("write:\nli $v0, 1\nsyscall\nli $v0, 4\nla $a0, _ret\nsyscall\nmove $v0, $0\njr $ra\n");
    struct ObjCode *p =  OCHead.nxt;
    while(p != &OCHead){
        printObjCode(p);
        p = p->nxt;
    }
}